DESCRIPTION = "Tizen Common Weston configuration and set-up"
HOMEPAGE = "http://nohomepage.org"
SECTION = "Base/Configuration"
LICENSE = "MIT"

SRC_URI = ""

S = "${WORKDIR}/git"

inherit autotools-brokensep

BBCLASSEXTEND = ""
PROVIDES = ""

#PROVIDES by weston-common
# the PROVIDES rules is ignore "weston-startup  "
PROVIDES += "weston-startup"
RPROVIDES_weston-common += "weston-startup"


#PROVIDES by weston-common-config


#PROVIDES by weston-common-tz-launcher


RDEPENDS = ""
#RDEPENDS of weston-common (${PN})
RDEPENDS_${PN} += "weston"
RDEPENDS_${PN} += "eglibc"
RDEPENDS_${PN} += "shadow"


DEPENDS = ""
#DEPENDS of weston-common
DEPENDS += "weston"
DEPENDS += "libxkbcommon"
DEPENDS += "libjpeg-turbo"
DEPENDS += "glib-2.0"
DEPENDS_append_class-native = " pkgconfig-native"
DEPENDS_append_class-target = " pkgconfig-native"
DEPENDS_append_class-native = " automake-native"
DEPENDS_append_class-target = " automake-native"
DEPENDS += "xz"
DEPENDS += "wayland"
DEPENDS_append_class-native = " libtool"
DEPENDS_append_class-target = " libtool-cross"
DEPENDS += "cairo"
DEPENDS += "libpng"
DEPENDS += "mesa"
DEPENDS_append_class-native = " autoconf-native"
DEPENDS_append_class-target = " autoconf-native"
DEPENDS += "vconf"
DEPENDS += "pixman"

do_prep() {
 cd ${S}
 chmod -Rf a+rX,u+w,g-w,o-w ${S}
 #setup -q
 cp ${S}/packaging/weston-common.manifest .
 
 
}
do_patch_append() {
    bb.build.exec_func('do_prep', d)
}

do_configure() {
}

do_compile() {
 cd ${S}
 LANG=C
 export LANG
 unset DISPLAY
 LD_AS_NEEDED=1; export LD_AS_NEEDED ;
 
 
   autotools_do_configure --build=${TARGET_SYS} --host=${HOST_SYS} \
         --target=${TARGET_SYS} \
         --program-prefix= \
         --prefix=${prefix} \
         --exec-prefix=${prefix} \
         --bindir=${prefix}/bin \
         --sbindir=${prefix}/sbin \
         --sysconfdir=${sysconfdir} \
         --datadir=${prefix}/share \
         --includedir=${prefix}/include \
         --libdir=${prefix}/lib \
         --libexecdir=${prefix}/libexec \
         --localstatedir=${localstatedir} \
         --sharedstatedir=${prefix}/com \
         --mandir=${mandir} \
         --infodir=${infodir}
 make -j16
 
 
 
}

do_install() {
 echo export RPM_BUILD_ROOT=${D}
 cd ${S}
 LANG=C
 export LANG
 unset DISPLAY
 rm -rf ${D}
 mkdir -p ${D}
 
 
 #install tz-launcher
 
   oe_runmake \
         DESTDIR=${D} \
         INSTALL_ROOT=${D} \
         BINDIR=${prefix}/bin \
   install  
   rm -f ${D}${infodir}/dir 
   find ${D} -regex ".*\.la$" | xargs rm -f -- 
   find ${D} -regex ".*\.a$" | xargs rm -f --
 
 # install weston service as 'display-manager.service' as it's the one wanted by graphical.target
 mkdir -p ${D}/lib/systemd/system
 install -m 644 display-manager-run.service ${D}/lib/systemd/system/display-manager-run.service
 install -m 644 display-manager.service ${D}/lib/systemd/system/display-manager.service
 install -m 644 display-manager.path ${D}/lib/systemd/system/display-manager.path
 
 # install Environment file for weston service and weston-user.service
 mkdir -p ${D}${sysconfdir}/sysconfig
 install -m 0644 weston.sysconfig ${D}${sysconfdir}/sysconfig/weston
 install -m 0644 weston-user.sysconfig ${D}${sysconfdir}/sysconfig/weston-user
 
 # install tmpfiles.d(5) conf
 mkdir -p ${D}${prefix}/lib/tmpfiles.d
 install -m 0644 weston_tmpfiles.conf ${D}${prefix}/lib/tmpfiles.d/weston.conf
 
 # install weston-user service in user session
 mkdir -p ${D}/lib/systemd/user
 install -m 644 weston-user.service ${D}/lib/systemd/user/
 
 # install weston.sh
 mkdir -p ${D}${sysconfdir}/profile.d/
 install -m 0644 weston.sh ${D}${sysconfdir}/profile.d/
 
 # install weston.ini
 mkdir -p ${D}${sysconfdir}/xdg/weston
 install -m 0644 weston.ini ${D}${sysconfdir}/xdg/weston
 
 # Add a rule to ensure the 'display' user has permissions to
 # open the graphics device
 mkdir -p ${D}${sysconfdir}/udev/rules.d
 cat >${D}${sysconfdir}/udev/rules.d/99-dri.rules <<'EOF'
 SUBSYSTEM=="drm", MODE="0660", GROUP="display", SMACK="*"
EOF
 
 # user 'display' must own /dev/tty7 for weston to start correctly
 cat >${D}${sysconfdir}/udev/rules.d/99-tty.rules <<'EOF'
 SUBSYSTEM=="tty", KERNEL=="tty7", OWNER="display", SMACK="^"
EOF
 
 # user 'display' must also be able to access /dev/input/event*
 cat >${D}${sysconfdir}/udev/rules.d/99-input.rules <<'EOF'
 SUBSYSTEM=="input", KERNEL=="event*", MODE="0660", GROUP="input", SMACK="^"
EOF
 
 # install desktop file
 mkdir -p ${D}${prefix}/share/applications
 install -m 0644 weston-terminal.desktop ${D}${prefix}/share/applications
 
 
}

pkg_preinst_${PN}() {
    #!/bin/sh -e

    # create groups 'display' and 'weston-launch'
    getent group display >/dev/null || ${D}${prefix}/sbin/groupadd -r -o display
    getent group input >/dev/null || ${D}${prefix}/sbin/groupadd -r -o input
    getent group weston-launch >/dev/null || ${D}${prefix}/sbin/groupadd -r -o weston-launch
    # create user 'display'
    getent passwd display >/dev/null || ${D}${prefix}/sbin/useradd -r -g display -d /run/display -s /bin/false -c "Display daemon" display
    # add user 'display' to groups 'weston-launch', 'input' and 'video'
    groupmod -A display weston-launch
    groupmod -A display input
    groupmod -A display video
    # setup display manager service
    mkdir -p /lib/systemd/system/graphical.target.wants/
    ln -sf ../display-manager.path  /lib/systemd/system/graphical.target.wants/
    # setup display manager access (inside user session)
    mkdir -p /lib/systemd/user/default.target.requires/
    ln -sf ../weston-user.service  /lib/systemd/user/default.target.requires/

}

pkg_postrm_${PN}() {
    #!/bin/sh -e

    rm -f /lib/systemd/system/graphical.target.wants/display-manager.path
    rm -f /lib/systemd/user/default.target.requires/weston-user.service

}

PACKAGES = "${PN}-dbg ${PN}-doc ${PN}-locale"
PACKAGES += " weston-common-tz-launcher "
PACKAGES += " weston-common "
PACKAGES += " weston-common-config "

weston-common-tz-launcher_files = ""
weston-common-tz-launcher_files += "weston-common.manifest"
weston-common-tz-launcher_files += "${prefix}/bin/tz-launcher"
weston-common-tz-launcher_files += "${prefix}/bin/wl-pre"

weston-common_files = ""
weston-common_files += "weston-common.manifest"
weston-common_files += "/lib/systemd/system/display-manager-run.service"
weston-common_files += "/lib/systemd/system/display-manager.service"
weston-common_files += "/lib/systemd/system/display-manager.path"
weston-common_files += "${sysconfdir}/sysconfig/*"
weston-common_files += "${prefix}/lib/tmpfiles.d/weston.conf"
weston-common_files += "/lib/systemd/user/weston-user.service"
weston-common_files += "${sysconfdir}/profile.d/*"
weston-common_files += "${sysconfdir}/udev/rules.d/*"
weston-common_files += "${prefix}/share/applications/*.desktop"

weston-common-config_files = ""
weston-common-config_files += "weston-common.manifest"
weston-common-config_files += "${sysconfdir}/xdg/weston/weston.ini"

FILES_${PN}-tz-launcher = "${weston-common-tz-launcher_files}"
FILES_${PN} = "${weston-common_files}"
FILES_${PN}-config = "${weston-common-config_files}"

PKG_weston-common-tz-launcher= "weston-common-tz-launcher"
PKG_weston-common= "weston-common"
PKG_weston-common-config= "weston-common-config"

require weston-common-extraconf.inc
