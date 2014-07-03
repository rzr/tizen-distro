DESCRIPTION = "Notification Library"
HOMEPAGE = "http://nohomepage.org"
SECTION = "System/API"
LICENSE = "Apache-2.0"

SRC_URI = ""

S = "${WORKDIR}/git"

inherit autotools-brokensep

BBCLASSEXTEND = ""
PROVIDES = ""

#PROVIDES by notification-service-dev


#PROVIDES by notification


#PROVIDES by notification-dev


RDEPENDS = ""
#RDEPENDS of notification-service-dev (${PN}-service-dev)
RDEPENDS_${PN}-service-dev += "notification"

#RDEPENDS of notification (${PN})
RDEPENDS_${PN} += "sqlite3"
RDEPENDS_${PN} += "eglibc"
RDEPENDS_${PN} += "libtzplatform-config"

#RDEPENDS of notification-dev (${PN}-dev)
RDEPENDS_${PN}-dev += "notification"


DEPENDS = ""
#DEPENDS of notification
DEPENDS_append_class-native = " cmake-native"
DEPENDS_append_class-target = " cmake-native"
DEPENDS += "eina"
DEPENDS += "dbus"
DEPENDS += "app-svc"
DEPENDS += "tizen-platform-config"
DEPENDS += "aul"
DEPENDS += "bundle"
DEPENDS += "com-core"
DEPENDS += "ail"
DEPENDS += "dlog"
DEPENDS += "ecore"
DEPENDS += "sqlite3"
DEPENDS += "heynoti"
DEPENDS += "vconf"
DEPENDS += "libslp-db-util"

do_prep() {
 cd ${S}
 chmod -Rf a+rX,u+w,g-w,o-w ${S}
 #setup -q
 cp ${S}/packaging/notification.manifest .
 
 
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
 
 
   CFLAGS="${CFLAGS:--O2 -g -m64 -fmessage-length=0 -D_FORTIFY_SOURCE=2 -fstack-protector -funwind-tables -fasynchronous-unwind-tables}" ; export CFLAGS ; 
   CXXFLAGS="${CXXFLAGS:--O2 -g -m64 -fmessage-length=0 -D_FORTIFY_SOURCE=2 -fstack-protector -funwind-tables -fasynchronous-unwind-tables}" ; export CXXFLAGS ; 
   FFLAGS="${FFLAGS:--O2 -g -m64 -fmessage-length=0 -D_FORTIFY_SOURCE=2 -fstack-protector -funwind-tables -fasynchronous-unwind-tables}" ; export FFLAGS ; 
   cmake \
         -DCMAKE_VERBOSE_MAKEFILE=ON \
         -DCMAKE_INSTALL_PREFIX:PATH=${prefix} \
         -DCMAKE_INSTALL_LIBDIR:PATH=${prefix}/lib \
         -DINCLUDE_INSTALL_DIR:PATH=${prefix}/include \
         -DLIB_INSTALL_DIR:PATH=${prefix}/lib \
         -DSYSCONF_INSTALL_DIR:PATH=${sysconfdir} \
         -DSHARE_INSTALL_PREFIX:PATH=${prefix}/share \
         -DCMAKE_SKIP_RPATH:BOOL=ON \
         -DBUILD_SHARED_LIBS:BOOL=ON .
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
 
 rm -rf ${D}
 
   oe_runmake \
         DESTDIR=${D} \
         INSTALL_ROOT=${D} \
         BINDIR=${prefix}/bin \
   install  
   rm -f ${D}${infodir}/dir 
   find ${D} -regex ".*\.la$" | xargs rm -f -- 
   find ${D} -regex ".*\.a$" | xargs rm -f --
 
 mkdir -p ${D}/usr/share/license
 cp -f LICENSE.APLv2.0 ${D}/usr/share/license/notification
 
 
}

pkg_postinst_${PN}() {
    #!/bin/sh -e

    ldconfig
    ${D}${prefix}/share/notification/notification_DB_init.sh

}

pkg_postrm_${PN}() {
    #!/bin/sh -e

    ldconfig
}

PACKAGES = "${PN}-dbg ${PN}-doc ${PN}-locale"
PACKAGES += " notification-service-dev "
PACKAGES += " notification "
PACKAGES += " notification-dev "

notification-service-dev_files = ""
notification-service-dev_files += "${prefix}/include/notification/service/notification_ipc.h"
notification-service-dev_files += "${prefix}/include/notification/service/notification_noti.h"
notification-service-dev_files += "${prefix}/include/notification/service/notification_setting_service.h"
notification-service-dev_files += "${prefix}/lib/pkgconfig/notification-service.pc"

notification_files = ""
notification_files += "notification.manifest"
notification_files += "${prefix}/lib/libnotification.so*"
notification_files += "${prefix}/share/notification/notification_DB_init.sh"
notification_files += "/usr/share/license/notification"

notification-dev_files = ""
notification-dev_files += "${prefix}/include/notification/notification.h"
notification-dev_files += "${prefix}/include/notification/notification_error.h"
notification-dev_files += "${prefix}/include/notification/notification_type.h"
notification-dev_files += "${prefix}/include/notification/notification_list.h"
notification-dev_files += "${prefix}/include/notification/notification_status.h"
notification-dev_files += "${prefix}/include/notification/notification_setting.h"
notification-dev_files += "${prefix}/lib/pkgconfig/notification.pc"

FILES_${PN}-service-dev = "${notification-service-dev_files}"
FILES_${PN} = "${notification_files}"
FILES_${PN}-dev = "${notification-dev_files}"

PKG_notification-service-dev= "notification-service-dev"
PKG_notification= "notification"
PKG_notification-dev= "notification-dev"

require notification-extraconf.inc
