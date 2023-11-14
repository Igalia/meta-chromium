require gn.inc

PV = "121.0.6115.2"

SRCREV = "e4702d7409069c4f12d45ea7b7f0890717ca3f4b"

# Upstreamable fixes
SRC_URI += " file://0001-GCC-fix-redundant-move-on-return-statement.patch "