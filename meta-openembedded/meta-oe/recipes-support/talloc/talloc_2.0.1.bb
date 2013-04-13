DESCRIPTION = "Hierarchical, reference counted memory pool system with destructors"
HOMEPAGE = "http://talloc.samba.org"
LICENSE = "LGPL-3.0"
LIC_FILES_CHKSUM = "file://NEWS;md5=5fe776b23a711c9153ee94bc87e47334"

inherit autotools pkgconfig

SRC_URI = "http://samba.org/ftp/${BPN}/${BPN}-${PV}.tar.gz \
           file://install-swig-interface-in-SWINGLIBDIR.patch \
"
SRC_URI[md5sum] = "c6e736540145ca58cb3dcb42f91cf57b"
SRC_URI[sha256sum] = "5b810527405f29d54f50efd78bf2c89e318f2cd8bed001f22f2a1412fd27c9b4"

TARGET_CC_ARCH += "${LDFLAGS}"

PR = "r2"

# autoreconf doesn't work well while reconfiguring included libreplace
do_configure () {
    gnu-configize
    oe_runconf
}

do_install_prepend() {
    # Hack the way swig interface for talloc is installed
    # This hack is accompanied by install-swig-interface-in-SWINGLIBDIR.patch
    type swig > /dev/null 2>&1 && SWIGLIBDIR=`swig -swiglib` && SWIGLIBDIR=${SWIGLIBDIR##${STAGING_DIR_NATIVE}} && export SWIGLIBDIR || echo "No swig found"
}

do_install_append() {
    install -d ${D}${libdir}
    ln -s libtalloc.so.2.0.1 ${D}${libdir}/libtalloc.so.2.0
    ln -s libtalloc.so.2.0 ${D}${libdir}/libtalloc.so.2
    ln -s libtalloc.so.2 ${D}${libdir}/libtalloc.so
}

PACKAGES += "${PN}-swig"
FILES_${PN}-swig += "${datadir}"
