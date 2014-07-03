DESCRIPTION = "A Location Manager library in Tizen Native API"
HOMEPAGE = "http://nohomepage.org"
SECTION = "Location/API"
LICENSE = "Apache-2.0"

SRC_URI = ""

S = "${WORKDIR}/git"

inherit autotools-brokensep

BBCLASSEXTEND = ""
PROVIDES = ""

#PROVIDES by capi-location-manager-dev


#PROVIDES by capi-location-manager


RDEPENDS = ""
#RDEPENDS of capi-location-manager-dev (capi-${PN}-dev)
RDEPENDS_capi-${PN}-dev += "capi-location-manager"


DEPENDS = ""
#DEPENDS of capi-location-manager
DEPENDS_append_class-native = " cmake-native"
DEPENDS_append_class-target = " cmake-native"
DEPENDS += "common"
DEPENDS += "dlog"
DEPENDS += "libslp-location"

do_prep() {
 cd ${S}
 chmod -Rf a+rX,u+w,g-w,o-w ${S}
 #setup -q
 cp ${S}/packaging/capi-location-manager.manifest .
 
 
 
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
 
 MAJORVER=`echo 0.1.11 | awk 'BEGIN {FS="."}{print $1}'`
 
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
         -DBUILD_SHARED_LIBS:BOOL=ON . -DFULLVER=0.1.11 -DMAJORVER=${MAJORVER}
 
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
 
 
}

pkg_postinst_${PN}() {
    #!/bin/sh -e

    ldconfig
}

pkg_postrm_${PN}() {
    #!/bin/sh -e

    ldconfig
}

PACKAGES = "${PN}-dbg ${PN}-doc ${PN}-locale"
PACKAGES += " capi-location-manager-dev "
PACKAGES += " capi-location-manager "

capi-location-manager-dev_files = ""
capi-location-manager-dev_files += "capi-location-manager.manifest"
capi-location-manager-dev_files += "${prefix}/include/location/*.h"
capi-location-manager-dev_files += "${prefix}/lib/pkgconfig/*.pc"
capi-location-manager-dev_files += "${prefix}/lib/libcapi-location-manager.so"

capi-location-manager_files = ""
capi-location-manager_files += "capi-location-manager.manifest"
capi-location-manager_files += "capi-location-manager.manifest"
capi-location-manager_files += "${prefix}/lib/libcapi-location-manager.so.*"

FILES_capi-${PN}-dev = "${capi-location-manager-dev_files}"
FILES_capi-${PN} = "${capi-location-manager_files}"

PKG_capi-location-manager-dev= "capi-location-manager-dev"
PKG_capi-location-manager= "capi-location-manager"

require location-manager-extraconf.inc
