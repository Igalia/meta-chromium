require chromium-x11.inc

SRC_URI[md5sum] = "8c7f450d2c432f664038006013765e76"
SRC_URI[sha256sum] = "ab210326029213eda0290db71f6ddbdf5c42e2d94c3461dee094637a34571219"

LIC_FILES_CHKSUM += "\
  file://${S}/third_party/breakpad/LICENSE;md5=828152b41153892a725f0e7f56faff73 \
  file://${S}/third_party/expat/files/COPYING;md5=5b8620d98e49772d95fc1d291c26aa79 \
  file://${S}/third_party/ffmpeg/CREDITS.chromium;md5=3adeb0fd8df4d2f0d81a7e4db49e4a37 \
  file://${S}/third_party/libsrtp/LICENSE;md5=2909fcf6f09ffff8430463d91c08c4e1\
  file://${S}/third_party/yasm/source/patched-yasm/COPYING;md5=a12d8903508fb6bfd49d8d82c6170dd9 \
"

SRC_URI += " \
 file://0001-IWYU-Include-math.h-for-round-3.patch;patchdir=third_party/webrtc \
 file://0001-Use-v8-qemu-wrapper.sh-on-v8-context-snapshot-creati.patch \
"
