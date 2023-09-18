SRC_URI = "gitsm://chromium.googlesource.com/v8/v8;protocol=https;name=v8;branch=main"
S = "${WORKDIR}/git"
LICENSE = "BSD-3-Clause & LGPL-2.0-only & LGPL-2.1-only"
LIC_FILES_CHKSUM = " \
  file://${S}/LICENSE;md5=f38a2942edcfe4abb45e9a83a1ad2f82 \
  file://${S}/LICENSE.fdlibm;md5=b17367d6c97e638599071d99a3049dfe \
  file://${S}/LICENSE.strongtalk;md5=956a43818dd13f2e93088fc2c93589d2 \
  "

SRCREV_v8 = "d1b54b6c6cac0bb5ec79ff797876f48afd6dd797"
