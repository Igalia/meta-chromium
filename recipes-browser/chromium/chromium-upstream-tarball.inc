SRC_URI = " \
  https://commondatastorage.googleapis.com/chromium-browser-official/chromium-${PV}.tar.xz;name=main \
  https://commondatastorage.googleapis.com/chromium-browser-clang/Linux_x64/rust-toolchain-${RUST_PACKAGE_VERSION}.tar.xz;name=rust;subdir=${S}/third_party \
"

S = "${WORKDIR}/chromium-${PV}"
