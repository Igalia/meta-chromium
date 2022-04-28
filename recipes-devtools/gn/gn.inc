SUMMARY = "Meta-build system from Google"
LICENSE = "BSD-3-Clause"
LIC_FILES_CHKSUM = "file://LICENSE;md5=0fca02217a5d49a14dfe2d11837bb34d"

DEPENDS = "ninja-native"

inherit python3native

SRC_URI = "git://gn.googlesource.com/gn;procotol=https;branch=main"

S = "${WORKDIR}/git"
B = "${WORKDIR}/build"

do_configure () {
  cd ${S}
  echo ${S}
  ${PYTHON} build/gen.py --out-path=${B} --no-strip
}

do_compile () {
  echo
  ninja -C ${B}
}

do_install () {
  install -d ${D}${bindir}
  install -m 0755 ${B}/gn ${D}${bindir}/gn
}

BBCLASSEXTEND = "native nativesdk"
