DESCRIPTION="SoX is the Swiss Army knife of sound processing tools. \
It converts audio files among various standard audio file formats \
and can apply different effects and filters to the audio data."
HOMEPAGE = "http://sox.sourceforge.net"
SECTION = "audio"

DEPENDS = "libpng libav libsndfile1"

PR = "r1"

PACKAGECONFIG ??= "${@base_contains('DISTRO_FEATURES', 'pulseaudio', 'pulseaudio', '', d)} \
                   ${@base_contains('DISTRO_FEATURES', 'alsa', 'alsa', '', d)} \
"
PACKAGECONFIG[pulseaudio] = "--with-pulseaudio=dyn,--with-pulseaudio=no,pulseaudio,"
PACKAGECONFIG[alsa] = "--with-alsa=dyn,--with-alsa=no,alsa-lib,"

LICENSE = "GPLv2 & LGPLv2.1"
LIC_FILES_CHKSUM = "file://LICENSE.GPL;md5=751419260aa954499f7abaabaa882bbe \
                    file://LICENSE.LGPL;md5=fbc093901857fcd118f065f900982c24"

SRC_URI = "${SOURCEFORGE_MIRROR}/sox/sox-${PV}.tar.gz"
SRC_URI[md5sum] = "b0c15cff7a4ba0ec17fdc74e6a1f9cf1"
SRC_URI[sha256sum] = "3ee34b14dd267de378e8a117aae81ec4cae330772342e6a55bbf6520a0a88aa3"

inherit autotools

BBCLASSEXTEND = "native"

