/* -*- Mode: C++; indent-tabs-mode: nil; tab-width: 4 -*-
 * -*- coding: utf-8 -*-
 *
 * Copyright (C) 2020 KylinSoft Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "xrandr-config.h"
#include <QFile>
#include <QStandardPaths>
#include <QRect>
#include <QJsonDocument>
#include <QDir>

#include <QHBoxLayout>
#include <QtXml>

QString xrandrConfig::mFixedConfigFileName = QStringLiteral("fixed-config");
QString xrandrConfig::mConfigsDirName = QStringLiteral("" /*"configs/"*/); // TODO: KDE6 - move these files into the subfolder

QString xrandrConfig::configsDirPath()
{
    QString dirPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) %
            QStringLiteral("/kscreen/");
    return dirPath % mConfigsDirName;
}

xrandrConfig::xrandrConfig(KScreen::ConfigPtr config, QObject *parent)
    : QObject(parent)
{
    mConfig = config;
}

QString xrandrConfig::filePath() const
{
    if (!QDir().mkpath(configsDirPath())) {
        return QString();
    }
    return configsDirPath() % id();
}

QString xrandrConfig::id() const
{
    if (!mConfig) {
        return QString();
    }
    return mConfig->connectedOutputsHash();
}

bool xrandrConfig::fileExists() const
{
    return (QFile::exists(configsDirPath() % id()) || QFile::exists(configsDirPath() % mFixedConfigFileName));
}

std::unique_ptr<xrandrConfig> xrandrConfig::readFile()
{
    bool res = false;
    if (res){//Device::self()->isLaptop() && !Device::self()->isLidClosed()) {
        // We may look for a config that has been set when the lid was closed, Bug: 353029
        const QString lidOpenedFilePath(filePath() % QStringLiteral("_lidOpened"));
        const QFile srcFile(lidOpenedFilePath);

        if (srcFile.exists()) {
            QFile::remove(filePath());
            if (QFile::copy(lidOpenedFilePath, filePath())) {
                QFile::remove(lidOpenedFilePath);
                qDebug() << "Restored lid opened config to" << id();
            }
        }
    }
    return readFile(id());
}

std::unique_ptr<xrandrConfig> xrandrConfig::readOpenLidFile()
{
    const QString openLidFile = id() % QStringLiteral("_lidOpened");
    auto config = readFile(openLidFile);
    QFile::remove(configsDirPath() % openLidFile);
    return config;
}

std::unique_ptr<xrandrConfig> xrandrConfig::readFile(const QString &fileName)
{
    if (!mConfig) {
        return nullptr;
    }
    auto config = std::unique_ptr<xrandrConfig>(new xrandrConfig(mConfig->clone()));
    config->setValidityFlags(mValidityFlags);

    QFile file;
    if (QFile::exists(configsDirPath() % mFixedConfigFileName)) {
        file.setFileName(configsDirPath() % mFixedConfigFileName);
        qDebug() << "found a fixed config, will use " << file.fileName();
    } else {
        file.setFileName(configsDirPath() % fileName);
    }
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "failed to open file" << file.fileName();
        return nullptr;
    }

    QJsonDocument parser;
    QVariantList outputs = parser.fromJson(file.readAll()).toVariant().toList();
    xrandrOutput::readInOutputs(config->data(), outputs); //不可用

    QSize screenSize;
    for (const auto &output : config->data()->outputs()) {
        if (!output->isPositionable()) {
            continue;
        }

        const QRect geom = output->geometry();
        if (geom.x() + geom.width() > screenSize.width()) {
            screenSize.setWidth(geom.x() + geom.width());
        }
        if (geom.y() + geom.height() > screenSize.height()) {
            screenSize.setHeight(geom.y() + geom.height());
        }
    }
    config->data()->screen()->setCurrentSize(screenSize);

    if (!canBeApplied(config->data())) {
        return nullptr;
    }
    return config;
}

bool xrandrConfig::canBeApplied() const
{
    return canBeApplied(mConfig);
}

bool xrandrConfig::canBeApplied(KScreen::ConfigPtr config) const
{
    return KScreen::Config::canBeApplied(config, mValidityFlags);
}

void xrandrConfig::log()
{
    if (!mConfig) {
        return;
    }
    const auto outputs = mConfig->outputs();
    for (const auto &o : outputs) {
        if (o->isConnected()) {
            qDebug() << o;
        }
    }
}