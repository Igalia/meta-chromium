SUMMARY = "dot.conf configuration file parser"
LICENSE = "LGPL-2.1"
LIC_FILES_CHKSUM = "file://COPYING;md5=155b66f2dc258f05886f9886a60fd870"

inherit autotools

SRC_URI = "git://github.com/williamh/dotconf.git;protocol=https;branch=master"
S = "${WORKDIR}/git"

SRCREV = "4cd7b3a6e89b7f1269851caea8374a8f137ea1c0"
