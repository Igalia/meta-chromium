require gn.inc

PV = "127.0.6510.4"

SRCREV = "e11af6727dd38fdd0e8cbbab2e207c369522ac7a"

# Upstreamable fixes
SRC_URI += " file://0001-GCC-fix-redundant-move-on-return-statement.patch "