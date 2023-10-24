SUMMARY = "Speech dispatcher client"
LICENSE = "LGPL-2.1-or-later"
LIC_FILES_CHKSUM = "file://COPYING.LGPL;md5=4fbd65380cdd255951079008b364516c"

DEPENDS = "dotconf glib-2.0 libsndfile1"

inherit autotools gettext pkgconfig

SRC_URI = "git://github.com/brailcom/speechd.git;protocol=https;nobranch=1 \
           file://0001-Only-build-C-API-client-library.patch \
           "
S = "${WORKDIR}/git"

SRCREV = "0680c0f92c43bd43aa2ac111c8ec478d8a4f94cc"

EXTRA_OECONF = "--disable-python --with-nas=no"

do_install:append() {
    cp -R ${D}${includedir}/speech-dispatcher/*.h ${D}${includedir}/
    rm -rf ${D}${datadir}/speech-dispatcher
}