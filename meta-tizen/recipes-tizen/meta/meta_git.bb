require meta.inc

PRIORITY = "10"

LIC_FILES_CHKSUM ??= "file://${COMMON_LICENSE_DIR}/GPL-2.0;md5=801f80980d171dd6425610833a22dbe6"

SRC_URI += "git://review.tizen.org/profile/common/meta;tag=d820e6948fd73b11caa830142088e82a3cf04b7c;nobranch=1"

BBCLASSEXTEND += " native "
