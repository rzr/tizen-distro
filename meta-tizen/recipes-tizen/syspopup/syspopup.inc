DESCRIPTION = "Syspopup package"
HOMEPAGE = "http://nohomepage.org"
SECTION = "System/Libraries"
LICENSE = "Apache-2.0"

SRC_URI = ""

S = "${WORKDIR}/git"

inherit autotools-brokensep

BBCLASSEXTEND = ""
PROVIDES = ""

#PROVIDES by syspopup-dev


#PROVIDES by syspopup


#PROVIDES by syspopup-caller-dev


#PROVIDES by syspopup-caller


RDEPENDS = ""
#RDEPENDS of syspopup-dev (${PN}-dev)
RDEPENDS_${PN}-dev += "syspopup"

#RDEPENDS of syspopup-caller-dev (${PN}-caller-dev)
RDEPENDS_${PN}-caller-dev += "syspopup"

#RDEPENDS of syspopup-caller (${PN}-caller)
RDEPENDS_${PN}-caller += "eglibc"
RDEPENDS_${PN}-caller += "syspopup"


DEPENDS = ""
#DEPENDS of syspopup
DEPENDS_append_class-native = " cmake-native"
DEPENDS_append_class-target = " cmake-native"
DEPENDS += "evas"
DEPENDS += "dbus"
DEPENDS += "aul"
DEPENDS += "dlog"
DEPENDS += "app-core"
DEPENDS += "bundle"
DEPENDS += "sqlite3"
DEPENDS += "tizen-platform-config"

do_prep() {
 cd ${S}
 chmod -Rf a+rX,u+w,g-w,o-w ${S}
 #setup -q
 cp ${S}/packaging/syspopup.manifest ${S}/packaging/syspopup-devel.manifest ${S}/packaging/syspopup-caller.manifest ${S}/packaging/syspopup-caller-devel.manifest .
 
 
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
         -DBUILD_SHARED_LIBS:BOOL=ON . \
 -Dwith_wayland=TRUE \
 -DEXTRA_CFLAGS=-fPIC
 
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
 
 
   oe_runmake \
         DESTDIR=${D} \
         INSTALL_ROOT=${D} \
         BINDIR=${prefix}/bin \
   install  
   rm -f ${D}${infodir}/dir 
   find ${D} -regex ".*\.la$" | xargs rm -f -- 
   find ${D} -regex ".*\.a$" | xargs rm -f --
 mkdir -p ${D}/usr/share/license
 install LICENSE ${D}/usr/share/license/syspopup
 install LICENSE ${D}/usr/share/license/syspopup-devel
 install LICENSE ${D}/usr/share/license/syspopup-caller
 install LICENSE ${D}/usr/share/license/syspopup-caller-devel
 
 mkdir -p ${D}${prefix}/dbspace
 sqlite3 ${D}${prefix}/dbspace/.syspopup.db < ${D}/usr/share/syspopup/syspopup_db.sql
 rm -rf ${D}/usr/share/syspopup/syspopup_db.sql
 
 touch ${D}${prefix}/share/popup_noti_term
 
 
}

pkg_postinst_${PN}() {
    #!/bin/sh -e

    ldconfig
}

pkg_postinst_${PN}() {
    #!/bin/sh -e

    ldconfig
}

pkg_postrm_${PN}() {
    #!/bin/sh -e

    ldconfig
}

pkg_postrm_${PN}() {
    #!/bin/sh -e

    ldconfig
}

PACKAGES = "${PN}-dbg ${PN}-doc ${PN}-locale"
PACKAGES += " syspopup-dev "
PACKAGES += " syspopup "
PACKAGES += " syspopup-caller-dev "
PACKAGES += " syspopup-caller "

syspopup-dev_files = ""
syspopup-dev_files += "syspopup-devel.manifest"
syspopup-dev_files += "${prefix}/include/SLP_SYSPOPUP_PG.h"
syspopup-dev_files += "${prefix}/include/syspopup.h"
syspopup-dev_files += "${prefix}/lib/libsyspopup.so"
syspopup-dev_files += "${prefix}/lib/pkgconfig/syspopup.pc"
syspopup-dev_files += "/usr/share/license/syspopup-devel"

syspopup_files = ""
syspopup_files += "syspopup.manifest"
syspopup_files += "${prefix}/share/icons/default/small/org.tizen.syspopup-app.png"
syspopup_files += "${prefix}/bin/sp_test"
syspopup_files += "${prefix}/bin/syspopup-app"
syspopup_files += "${prefix}/lib/libsyspopup.so.0.1.0"
syspopup_files += "${prefix}/share/packages/org.tizen.syspopup-app.xml"
syspopup_files += "${prefix}/share/popup_noti_term"
syspopup_files += "${prefix}/dbspace/.syspopup.db"
syspopup_files += "${prefix}/dbspace/.syspopup.db-journal"
syspopup_files += "/usr/share/license/syspopup"

syspopup-caller-dev_files = ""
syspopup-caller-dev_files += "syspopup-caller-devel.manifest"
syspopup-caller-dev_files += "${prefix}/lib/libsyspopup_caller.so"
syspopup-caller-dev_files += "${prefix}/include/syspopup_caller.h"
syspopup-caller-dev_files += "${prefix}/lib/pkgconfig/syspopup-caller.pc"
syspopup-caller-dev_files += "/usr/share/license/syspopup-caller-devel"

syspopup-caller_files = ""
syspopup-caller_files += "syspopup-caller.manifest"
syspopup-caller_files += "${prefix}/lib/libsyspopup_caller.so.0.1.0"
syspopup-caller_files += "/usr/share/license/syspopup-caller"

FILES_${PN}-dev = "${syspopup-dev_files}"
FILES_${PN} = "${syspopup_files}"
FILES_${PN}-caller-dev = "${syspopup-caller-dev_files}"
FILES_${PN}-caller = "${syspopup-caller_files}"

PKG_syspopup-dev= "syspopup-dev"
PKG_syspopup= "syspopup"
PKG_syspopup-caller-dev= "syspopup-caller-dev"
PKG_syspopup-caller= "syspopup-caller"

require syspopup-extraconf.inc
