%define debug_package %{nil}
Name:           ukui-settings-daemon
Version:        3.0.3
Release:        1
Summary:        daemon handling the UKUI session settings
License:        GPL-2.0, GPL-2+, GPL-2.1, LGPL-2.1+, GPL-3+, LGPL-2+, MIT~OldStyleWithDisclaimer+RedHat, MIT~OldStyle+RedHat
URL:            http://www.ukui.org
Source0:        %{name}-%{version}.tar.gz

BuildRequires: colord-devel
BuildRequires: geoclue2-devel
BuildRequires: gnome-desktop3-devel
BuildRequires: gsettings-qt-devel
BuildRequires: imlib2-devel
BuildRequires: intltool
BuildRequires: kf5-kconfig-devel
BuildRequires: lcms2-devel
BuildRequires: libcanberra-devel
BuildRequires: libgudev-devel
BuildRequires: libmatekbd-devel
BuildRequires: libmatemixer-devel
BuildRequires: libnotify-devel
# BuildRequires: libwayland-client
BuildRequires: libwnck3-devel
BuildRequires: libxklavier-devel
BuildRequires: libXtst-devel
BuildRequires: mate-desktop-devel
BuildRequires: pulseaudio-devel
BuildRequires: qt5-qtbase-devel
BuildRequires: qt5-qtsensors-devel
BuildRequires: qt5-qtsvg-devel
BuildRequires: qt5-qttools-devel
BuildRequires: qt5-qtx11extras-devel
BuildRequires: qtchooser
BuildRequires: xorg-x11-server-devel
 
Requires: mate-desktop-libs >= 1.18
Requires: ukui-settings-daemon-common
Requires: imwheel
Requires: ukui-polkit
Requires: xorg-x11-server-utils
Requires: xorg-x11-drv-synaptics-legacy
Requires: glib2


%description
 This package contains the daemon which is responsible for setting the
 various parameters of a UKUI session and the applications that run
 under it. It handles the following kinds of settings:
 .
  * Keyboard: layout, accessibility options, shortcuts, media keys
  * Clipboard management
  * Theming: background, icons, GTK+ applications
  * Cleanup of unused files
  * Mouse: cursors, speed, accessibility options
  * Startup of other daemons: screensaver, sound daemon
  * Typing break
 .
 It also sets various application settings through X resources and
 freedesktop.org XSETTINGS.
 
%package common
Summary:	daemon handling the UKUI session settings (common files)

%description common
 This package contains the daemon which is responsible for setting the
 various parameters of a UKUI session and the applications that run
 under it. It handles the following kinds of settings:
 .
  * Keyboard: layout, accessibility options, shortcuts, media keys
  * Clipboard management
  * Theming: background, icons, GTK+ applications
  * Cleanup of unused files
  * Mouse: cursors, speed, accessibility options
  * Startup of other daemons: screensaver, sound daemon
  * Typing break
 .
 It also sets various application settings through X resources and
 freedesktop.org XSETTINGS.
 .
 This package contains the architecture independent files.

%prep
%setup -q

%build
qmake-qt5
make

%install
rm -rf $RPM_BUILD_ROOT
make INSTALL_ROOT=%{buildroot} install

%clean
rm -rf $RPM_BUILD_ROOT

%post
set -e
glib-compile-schemas /usr/share/glib-2.0/schemas/

%files
%{_sysconfdir}/xdg/autostart/ukui-settings-daemon.desktop
%{_prefix}/lib/udev/rules.d/01-touchpad-state-onmouse.rules
%{_bindir}/touchpad-state
%{_bindir}/ukui-settings-daemon
%{_bindir}/usd-locate-pointer
%{_libdir}/ukui-settings-daemon/a11y-keyboard.ukui-settings-plugin
%{_libdir}/ukui-settings-daemon/a11y-settings.ukui-settings-plugin
%{_libdir}/ukui-settings-daemon/background.ukui-settings-plugin
%{_libdir}/ukui-settings-daemon/clipboard.ukui-settings-plugin
%{_libdir}/ukui-settings-daemon/color.ukui-settings-plugin
%{_libdir}/ukui-settings-daemon/housekeeping.ukui-settings-plugin
%{_libdir}/ukui-settings-daemon/keybindings.ukui-settings-plugin
%{_libdir}/ukui-settings-daemon/keyboard.ukui-settings-plugin
%{_libdir}/ukui-settings-daemon/liba11y-keyboard.so
%{_libdir}/ukui-settings-daemon/liba11y-settings.so
%{_libdir}/ukui-settings-daemon/libbackground.so
%{_libdir}/ukui-settings-daemon/libclipboard.so
%{_libdir}/ukui-settings-daemon/libcolor.so
%{_libdir}/ukui-settings-daemon/libhousekeeping.so
%{_libdir}/ukui-settings-daemon/libkeybindings.so
%{_libdir}/ukui-settings-daemon/libkeyboard.so
%{_libdir}/ukui-settings-daemon/libmedia-keys.so
%{_libdir}/ukui-settings-daemon/libmouse.so
%{_libdir}/ukui-settings-daemon/libmpris.so
%{_libdir}/ukui-settings-daemon/libsound.so
%{_libdir}/ukui-settings-daemon/libtablet-mode.so
%{_libdir}/ukui-settings-daemon/libxrandr.so
%{_libdir}/ukui-settings-daemon/libxrdb.so
%{_libdir}/ukui-settings-daemon/libxsettings.so
%{_libdir}/ukui-settings-daemon/media-keys.ukui-settings-plugin
%{_libdir}/ukui-settings-daemon/mouse.ukui-settings-plugin
%{_libdir}/ukui-settings-daemon/mpris.ukui-settings-plugin
%{_libdir}/ukui-settings-daemon/sound.ukui-settings-plugin
%{_libdir}/ukui-settings-daemon/tablet-mode.ukui-settings-plugin
%{_libdir}/ukui-settings-daemon/xrandr.ukui-settings-plugin
%{_libdir}/ukui-settings-daemon/xrdb.ukui-settings-plugin
%{_libdir}/ukui-settings-daemon/xsettings.ukui-settings-plugin
%{_datadir}/dbus-1/services/org.ukui.SettingsDaemon.service

%files common
%doc debian/changelog debian/copyright
%{_datadir}/glib-2.0/
%{_datadir}/locale/
%{_datadir}/ukui-settings-daemon/

%changelog
* Tue Feb 2 2021 lvhan <lvhan@kylinos.cn> - 3.0.3-1
- update to upstream version 3.0.3-1

* Mon Oct 26 2020 douyan <douyan@kylinos.cn> - 3.0.1-1
- update to upstream version 3.0.0-1+1026

* Thu Jul 9 2020 douyan <douyan@kylinos.cn> - 1.2.1-1
- Init package for openEuler
