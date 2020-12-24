/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2007 William Jon McCann <mccann@jhu.edu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "config.h"

#include <glib/gi18n-lib.h>
#include <gmodule.h>

#include "ukui-settings-plugin.h"
#include "usd-xsettings-plugin.h"
#include "usd-xsettings-manager.h"

struct UkuiXSettingsPluginPrivate {
        UkuiXSettingsManager *manager;
};

#define UKUI_XSETTINGS_PLUGIN_GET_PRIVATE(object) (G_TYPE_INSTANCE_GET_PRIVATE ((object), UKUI_TYPE_XSETTINGS_PLUGIN, UkuiXSettingsPluginPrivate))

UKUI_SETTINGS_PLUGIN_REGISTER (UkuiXSettingsPlugin, ukui_xsettings_plugin)

static void
ukui_xsettings_plugin_init (UkuiXSettingsPlugin *plugin)
{
        plugin->priv = UKUI_XSETTINGS_PLUGIN_GET_PRIVATE (plugin);

        g_debug ("UkuiXSettingsPlugin initializing");

        plugin->priv->manager = ukui_xsettings_manager_new ();
}

static void
ukui_xsettings_plugin_finalize (GObject *object)
{
        UkuiXSettingsPlugin *plugin;

        g_return_if_fail (object != NULL);
        g_return_if_fail (UKUI_IS_XSETTINGS_PLUGIN (object));

        g_debug ("UkuiXSettingsPlugin finalizing");

        plugin = UKUI_XSETTINGS_PLUGIN (object);

        g_return_if_fail (plugin->priv != NULL);

        if (plugin->priv->manager != NULL) {
                g_object_unref (plugin->priv->manager);
        }

        G_OBJECT_CLASS (ukui_xsettings_plugin_parent_class)->finalize (object);
}

static void
impl_activate (UkuiSettingsPlugin *plugin)
{
        gboolean res;
        GError  *error;

        g_debug ("Activating xsettings plugin");

        error = NULL;
        res = ukui_xsettings_manager_start (UKUI_XSETTINGS_PLUGIN (plugin)->priv->manager, &error);
        if (! res) {
                g_warning ("Unable to start xsettings manager: %s", error->message);
                g_error_free (error);
        }
}

static void
impl_deactivate (UkuiSettingsPlugin *plugin)
{
        g_debug ("Deactivating xsettings plugin");
        ukui_xsettings_manager_stop (UKUI_XSETTINGS_PLUGIN (plugin)->priv->manager);
}

static void
ukui_xsettings_plugin_class_init (UkuiXSettingsPluginClass *klass)
{
        GObjectClass             *object_class = G_OBJECT_CLASS (klass);
        UkuiSettingsPluginClass *plugin_class = UKUI_SETTINGS_PLUGIN_CLASS (klass);

        object_class->finalize = ukui_xsettings_plugin_finalize;

        plugin_class->activate = impl_activate;
        plugin_class->deactivate = impl_deactivate;

        g_type_class_add_private (klass, sizeof (UkuiXSettingsPluginPrivate));
}

static void
ukui_xsettings_plugin_class_finalize (UkuiXSettingsPluginClass *klass)
{
}