DESCRIPTION = "Multimedia Framework Time Analysis Lib"
HOMEPAGE = "http://nohomepage.org"
SECTION = "System/Libraries"
LICENSE = "Apache-2.0"

SRC_URI = ""

S = "${WORKDIR}/git"

inherit autotools-brokensep

BBCLASSEXTEND = ""
PROVIDES = ""

#PROVIDES by libmm-ta


#PROVIDES by libmm-ta-dev


RDEPENDS = ""
#RDEPENDS of libmm-ta-dev (${PN}-dev)
RDEPENDS_${PN}-dev += "libmm-ta"


DEPENDS = ""

do_prep() {
 cd ${S}
 chmod -Rf a+rX,u+w,g-w,o-w ${S}
 #setup -q 
 cp ${S}/packaging/libmm-ta.manifest .
 
 
 
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
         --infodir=${infodir} --disable-static
 
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
         --infodir=${infodir} --disable-static
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
PACKAGES += " libmm-ta "
PACKAGES += " libmm-ta-dev "

libmm-ta_files = ""
libmm-ta_files += "libmm-ta.manifest"
libmm-ta_files += "${prefix}/lib/libmm_ta.so.*"

libmm-ta-dev_files = ""
libmm-ta-dev_files += "libmm-ta.manifest"
libmm-ta-dev_files += "${prefix}/include/mm_ta/*.h"
libmm-ta-dev_files += "${prefix}/lib/*.so"
libmm-ta-dev_files += "${prefix}/lib/pkgconfig/*.pc"

FILES_${PN} = "${libmm-ta_files}"
FILES_${PN}-dev = "${libmm-ta-dev_files}"

PKG_libmm-ta= "libmm-ta"
PKG_libmm-ta-dev= "libmm-ta-dev"

require libmm-ta-extraconf.inc
