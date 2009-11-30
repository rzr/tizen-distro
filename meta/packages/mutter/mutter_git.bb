SECTION = "x11/wm"
DESCRIPTION = "Metacity is the boring window manager for the adult in you. Mutter is metacity + clutter."
LICENSE = "GPLv2"
DEPENDS = "startup-notification gtk+ gconf clutter-1.0 gdk-pixbuf-csource-native intltool glib-2.0-native"
# gobject-introspection
PR = "r14"
PV = "2.25.1+git${SRCPV}"
inherit gnome update-alternatives

# Gnome is the upstream but moblin is under more active development atm
# git://git.gnome.org/mutter.git;protocol=git;branch=master
#
SRC_URI = "git://git.moblin.org/mutter.git;protocol=git;branch=master \
           file://nodocs.patch;patch=1 \
           file://nozenity.patch;patch=1 \
           file://crosscompile.patch;patch=1;rev=7adb574bb3fa3880eb85dbc86e580cf3452d57c4 \
           file://fix_pkgconfig-7adb574bb3fa3880eb85dbc86e580cf3452d57c4.patch;patch=1;rev=7adb574bb3fa3880eb85dbc86e580cf3452d57c4 \
           file://fix_pkgconfig.patch;patch=1;notrev=7adb574bb3fa3880eb85dbc86e580cf3452d57c4 \
           "
S = "${WORKDIR}/git"

ALTERNATIVE_NAME = "x-window-manager"
ALTERNATIVE_LINK = "${bindir}/x-window-manager"
ALTERNATIVE_PATH = "${bindir}/mutter"
ALTERNATIVE_PRIORITY = "11"

EXTRA_OECONF += "--disable-verbose	\
	         --disable-xinerama	\
	         --without-introspection \
		 --with-clutter"

#RDEPENDS_${PN} = "zenity"

FILES_${PN} += "${datadir}/themes ${libdir}/mutter/plugins/*.so ${datadir}/gnome/wm-properties/"
FILES_${PN}-dbg += "${libdir}/mutter/plugins/.debug/*"

export CC_FOR_BUILD = "${BUILD_CC}"
export CFLAGS_FOR_BUILD = "${BUILD_CFLAGS} -I${STAGING_INCDIR_NATIVE}/glib-2.0 -I${STAGING_INCDIR_NATIVE}/glib-2.0/include"
export LDFLAGS_FOR_BUILD = "${BUILD_LDFLAGS} -L${STAGING_LIBDIR_NATIVE} -lglib-2.0"

do_configure_prepend () {
        echo "EXTRA_DIST=" > ${S}/gnome-doc-utils.make
}		

do_stage () {
	 autotools_stage_all
}

pkg_postinst_${PN} () {
#!/bin/sh -e
if [ "x$D" != "x" ]; then
    exit 1
fi

. ${sysconfdir}/init.d/functions

gconftool-2 --config-source=xml::$D${sysconfdir}/gconf/gconf.xml.defaults --direct --type list --list-type string --set /apps/mutter/general/clutter_plugins '[default]'

gconftool-2 --config-source=xml::$D${sysconfdir}/gconf/gconf.xml.defaults --direct --type bool --set /apps/mutter/general/compositing_manager true
}

