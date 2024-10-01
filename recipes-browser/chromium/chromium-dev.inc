require chromium-upstream-tarball.inc

PV = "131.0.6738.0"
# This should be obtained running tools/rust/update_rust.py --print-package-version
# in the tarball release
RUST_PACKAGE_VERSION = "009e73825af0e59ad4fc603562e038b3dbd6593a-2-llvmorg-20-init-3847-g69c43468"

SRC_URI[main.sha256sum] = "ee1dc8da263d2697a7123ae842aedfc1929765a00492b95b4467afbf50d009df"
SRC_URI[rust-toolchain.sha256sum] = "043bc520520424fad9fdfc87102e3af7c8f1ca4da3f7885e18f54fd29fc783e4"

# Chromium itself is licensed under the 3-clause BSD license. However, it
# depends upon several other projects whose copyright files are listed in
# LIC_FILES_CHKSUM. The licenses in those files vary, but none of them should
# cause the main Chromium license to change. The list of copyright files was
# generated with the aid of Chromium's own tools/licenses.py module. As such,
# it contains more items than we actually use in our build (e.g. there are
# components specific to Android, OS X, iOS etc, or bundled dependencies that
# we replace with system-wide versions). For more information about each
# project, you can consult README.chromium in each directory listed below.
# LICENSE does not cover all the licenses in LIC_FILES_CHKSUM; instead, it
# currently contains Chromium's license (BSD-3-Clause) as well as Blink's
# (LGPL2.0, LGPL2.1 and BSD-2-Clause).
LICENSE = "BSD-3-Clause & LGPL-2.0-only & LGPL-2.1-only"
LIC_FILES_CHKSUM = "\
  file://${S}/LICENSE;md5=c408a301e3407c3803499ce9290515d6 \
  file://${S}/LICENSE.chromium_os;md5=775979d3e1e3bf163e4882e231727de5 \
  file://${S}/base/third_party/cityhash/COPYING;md5=f714c995c5bd9b175938acdd69226b60 \
  file://${S}/base/third_party/double_conversion/LICENSE;md5=1ea35644f0ec0d9767897115667e901f \
  file://${S}/base/third_party/nspr/LICENSE;md5=0806173dfb7e6d060074595e968fd02e \
  file://${S}/base/third_party/superfasthash/LICENSE;md5=c66981f8ad23c9f279a5b9e07385128c \
  file://${S}/base/third_party/symbolize/LICENSE;md5=17ae3b22fe8fa438966625593e2eea85 \
  file://${S}/base/third_party/xdg_user_dirs/LICENSE;md5=d998f250c491c329a8254dd1ca62c647 \
  file://${S}/chrome/installer/mac/third_party/bsdiff/LICENSE;md5=0dbe7a50f028269750631fcbded3846a \
  file://${S}/chrome/installer/mac/third_party/xz/LICENSE;md5=84982e6bf3ed99ef2647e48626ffa984 \
  file://${S}/chrome/third_party/mozilla_security_manager/LICENSE;md5=0c259b853bbf067b361100ce560adce7 \
  file://${S}/ios/third_party/blink/LICENSE;md5=9fdb47308c4e0a2b6d07c5af39e5da1a \
  file://${S}/ios/third_party/material_components_ios/LICENSE;md5=3b83ef96387f14655fc854ddc3c6bd57 \
  file://${S}/ios/third_party/material_font_disk_loader_ios/LICENSE;md5=3b83ef96387f14655fc854ddc3c6bd57 \
  file://${S}/ios/third_party/material_internationalization_ios/LICENSE;md5=3b83ef96387f14655fc854ddc3c6bd57 \
  file://${S}/ios/third_party/material_roboto_font_loader_ios/LICENSE;md5=3b83ef96387f14655fc854ddc3c6bd57 \
  file://${S}/ios/third_party/material_sprited_animation_view_ios/LICENSE;md5=175792518e4ac015ab6696d16c4f607e \
  file://${S}/ios/third_party/material_text_accessibility_ios/LICENSE;md5=3b83ef96387f14655fc854ddc3c6bd57 \
  file://${S}/ios/third_party/motion_animator_objc/LICENSE;md5=3b83ef96387f14655fc854ddc3c6bd57 \
  file://${S}/ios/third_party/motion_interchange_objc/LICENSE;md5=3b83ef96387f14655fc854ddc3c6bd57 \
  file://${S}/ios/third_party/motion_transitioning_objc/LICENSE;md5=3b83ef96387f14655fc854ddc3c6bd57 \
  file://${S}/native_client_sdk/src/libraries/third_party/newlib-extras/README;md5=e944d73ca3817b7ca8656eafb9497fed \
  file://${S}/net/third_party/nss/LICENSE;md5=3b1e88e1b9c0b5a4b2881d46cce06a18 \
  file://${S}/net/third_party/quiche/src/LICENSE;md5=0fca02217a5d49a14dfe2d11837bb34d \
  file://${S}/net/third_party/uri_template/LICENSE;md5=3b83ef96387f14655fc854ddc3c6bd57 \
  file://${S}/third_party/abseil-cpp/LICENSE;md5=df52c6edb7adc22e533b2bacc3bd3915 \
  file://${S}/third_party/accessibility-audit/LICENSE;md5=3b83ef96387f14655fc854ddc3c6bd57 \
  file://${S}/third_party/afl/src/docs/COPYING;md5=3b83ef96387f14655fc854ddc3c6bd57 \
  file://${S}/third_party/android_media/LICENSE;md5=3b83ef96387f14655fc854ddc3c6bd57 \
  file://${S}/third_party/android_opengl/LICENSE;md5=d10e92761a860d4113a7a525c78daf13 \
  file://${S}/third_party/android_swipe_refresh/LICENSE;md5=3b83ef96387f14655fc854ddc3c6bd57 \
  file://${S}/third_party/angle/LICENSE;md5=7abdb66a6948f39c2f469140db5184e2 \
  file://${S}/third_party/apple_apsl/LICENSE;md5=c8c14a9f985552dba8d3edbd76660088 \
  file://${S}/third_party/ashmem/LICENSE;md5=3b83ef96387f14655fc854ddc3c6bd57 \
  file://${S}/third_party/blink/LICENSE_FOR_ABOUT_CREDITS;md5=11e90d553b211de885f245900c4ccf89 \
  file://${S}/third_party/boringssl/src/LICENSE;md5=2ca501bc96ce9ed0814e2c592c3f9593 \
  file://${S}/third_party/breakpad/LICENSE;md5=828152b41153892a725f0e7f56faff73 \
  file://${S}/third_party/brotli/LICENSE;md5=941ee9cd1609382f946352712a319b4b \
  file://${S}/third_party/bspatch/LICENSE;md5=3e837ede9697ce4c789c3ca32aabe003 \
  file://${S}/third_party/ced/LICENSE;md5=175792518e4ac015ab6696d16c4f607e \
  file://${S}/third_party/chromevox/LICENSE;md5=45739090b454125d851ac7bb1bbd23a2 \
  file://${S}/third_party/cld_3/LICENSE;md5=d8b32ba83f8b11e4badd979f4319e706 \
  file://${S}/third_party/closure_compiler/LICENSE;md5=3b83ef96387f14655fc854ddc3c6bd57 \
  file://${S}/third_party/crashpad/crashpad/LICENSE;md5=3b83ef96387f14655fc854ddc3c6bd57 \
  file://${S}/third_party/d3/src/LICENSE;md5=fcc83e5a00c3ea5885051b46163bd453 \
  file://${S}/third_party/decklink/LICENSE;md5=85b197f7b7b1ef270b01a1b6377be50e \
  file://${S}/third_party/dom_distiller_js/LICENSE;md5=6cd778870ce5c0c08c8e16fbdf378532 \
  file://${S}/third_party/expat/src/expat/COPYING;md5=7b3b078238d0901d3b339289117cb7fb \
  file://${S}/third_party/ffmpeg/CREDITS.chromium;md5=a0156e1de86eb34eb5d75486b539aa58 \
  file://${S}/third_party/flac/COPYING.Xiph;md5=3d6da238b5b57a0965d6730291119f65 \
  file://${S}/third_party/flatbuffers/LICENSE;md5=a873c5645c184d51e0f9b34e1d7cf559 \
  file://${S}/third_party/fontconfig/src/COPYING;md5=00252fd272bf2e722925613ad74cb6c7 \
  file://${S}/third_party/freetype/src/docs/FTL.TXT;md5=9f37b4e6afa3fef9dba8932b16bd3f97 \
  file://${S}/third_party/gif_player/LICENSE;md5=86d3f3a95c324c9479bd8986968f4327 \
  file://${S}/third_party/google_input_tools/LICENSE;md5=ff0b0f0202337c9d4ae2d869797049d3 \
  file://${S}/third_party/google_toolbox_for_mac/LICENSE;md5=3b83ef96387f14655fc854ddc3c6bd57 \
  file://${S}/third_party/harfbuzz-ng/src/COPYING;md5=b98429b8e8e3c2a67cfef01e99e4893d \
  file://${S}/third_party/hunspell/COPYING.MPL;md5=bfe1f75d606912a4111c90743d6c7325 \
  file://${S}/third_party/iaccessible2/LICENSE;md5=43bbd0bfb581347ec10def720000a645 \
  file://${S}/third_party/iccjpeg/LICENSE;md5=26834d132689a03abf860f4572705494 \
  file://${S}/third_party/icu/LICENSE;md5=08dc3852df8fffa807301902ad899ff8 \
  file://${S}/third_party/inspector_protocol/LICENSE;md5=db90dcb101891a7285d25f2fbceca752 \
  file://${S}/third_party/isimpledom/LICENSE;md5=822502c0e010ccdbb6a3e681dd47888e \
  file://${S}/third_party/jsoncpp/LICENSE;md5=c56ee55c03a55f8105b969d8270632ce \
  file://${S}/third_party/jstemplate/COPYING;md5=3b83ef96387f14655fc854ddc3c6bd57 \
  file://${S}/third_party/khronos/LICENSE;md5=1d2ef853a9ae7ace4e16fda0d48f597b \
  file://${S}/third_party/lcov/COPYING;md5=751419260aa954499f7abaabaa882bbe \
  file://${S}/third_party/leveldatabase/src/LICENSE;md5=92d1b128950b11ba8495b64938fc164d \
  file://${S}/third_party/libc++/src/LICENSE.TXT;md5=55d89dd7eec8d3b4204b680e27da3953 \
  file://${S}/third_party/libc++abi/src/LICENSE.TXT;md5=7b9334635b542c56868400a46b272b1e \
  file://${S}/third_party/libevent/LICENSE;md5=a2b0302a710150293fb85d2bfabd3de6 \
  file://${S}/third_party/libFuzzer/LICENSE.TXT;md5=4c0bc17c954e99fd547528d938832bfa \
  file://${S}/third_party/libaddressinput/LICENSE;md5=3b83ef96387f14655fc854ddc3c6bd57 \
  file://${S}/third_party/libjingle_xmpp/LICENSE;md5=ad296492125bc71530d06234d9bfebe0 \
  file://${S}/third_party/libjpeg_turbo/LICENSE.md;md5=2a8e0d8226a102f07ab63ed7fd6ce155 \
  file://${S}/third_party/liblouis/LICENSE;md5=8ab69863de0d3a0e00c4f97a4d78dd4a \
  file://${S}/third_party/libphonenumber/LICENSE;md5=e23fadd6ceef8c618fc1c65191d846fa \
  file://${S}/third_party/libpng/LICENSE;md5=b0085051bf265bac2bfc38bc89f50000 \
  file://${S}/third_party/libprotobuf-mutator/src/LICENSE;md5=3b83ef96387f14655fc854ddc3c6bd57 \
  file://${S}/third_party/libsecret/LICENSE;md5=23c2a5e0106b99d75238986559bb5fc6 \
  file://${S}/third_party/libsrtp/LICENSE;md5=2909fcf6f09ffff8430463d91c08c4e1\
  file://${S}/third_party/libsync/LICENSE;md5=3b83ef96387f14655fc854ddc3c6bd57 \
  file://${S}/third_party/libusb/src/COPYING;md5=fbc093901857fcd118f065f900982c24 \
  file://${S}/third_party/libvpx/source/libvpx/LICENSE;md5=d5b04755015be901744a78cc30d390d4 \
  file://${S}/third_party/libwebm/source/LICENSE.TXT;md5=6e8dee932c26f2dab503abf70c96d8bb \
  file://${S}/third_party/libwebp/LICENSE;md5=72dceabd78c6f435bb304047aff1745a \
  file://${S}/third_party/libxml/src/Copyright;md5=f437ed9058e8e5135e47c01e973376ba \
  file://${S}/third_party/libxslt/src/Copyright;md5=0cd9a07afbeb24026c9b03aecfeba458 \
  file://${S}/third_party/libyuv/LICENSE;md5=464282cfb405b005b9637f11103a7325 \
  file://${S}/third_party/logilab/logilab/common/LICENSE.txt;md5=751419260aa954499f7abaabaa882bbe \
  file://${S}/third_party/lzma_sdk/LICENSE;md5=eceed1d308734c8051664bc3d1ca175e \
  file://${S}/third_party/minigbm/LICENSE;md5=fcd558ab2fd7f53408826e80eb0c11eb \
  file://${S}/third_party/modp_b64/LICENSE;md5=eb7e2e0af1d4971360553aedadee8d86 \
  file://${S}/third_party/openh264/src/LICENSE;md5=bb6d3771da6a07d33fd50d4d9aa73bcf \
  file://${S}/third_party/opus/src/COPYING;md5=e304cdf74c2a1b0a33a5084c128a23a3 \
  file://${S}/third_party/ots/LICENSE;md5=e06eff2aa65b917034a81599bea73dc4 \
  file://${S}/third_party/pdfium/LICENSE;md5=c93507531cc9bb8e24a05f2a1a4036c7 \
  file://${S}/third_party/ply/LICENSE;md5=f298aa3b0cfe0b0ea96acabf944529c0 \
  file://${S}/third_party/polymer/LICENSE.polymer;md5=324f45ce459ffd97e41d175a4e95a4be \
  file://${S}/third_party/protobuf/LICENSE;md5=37b5762e07f0af8c74ce80a8bda4266b \
  file://${S}/third_party/pylint/pylint/LICENSE.txt;md5=4325afd396febcb659c36b49533135d4 \
  file://${S}/third_party/qcms/src/COPYING;md5=65636fa951328d3be95f6047ec3936ce \
  file://${S}/third_party/re2/LICENSE;md5=3b5c31eb512bdf3cb11ffd5713963760 \
  file://${S}/third_party/simplejson/LICENSE.txt;md5=8a9f8dcfcd28c34daa443e5b1d29813b \
  file://${S}/third_party/skia/LICENSE;md5=822f02cc7736281816581cd064afbb1c \
  file://${S}/third_party/smhasher/LICENSE;md5=0d948322ab524e2b74f55eb8ef57c74a \
  file://${S}/third_party/snappy/src/COPYING;md5=f62f3080324a97b3159a7a7e61812d0c \
  file://${S}/third_party/sqlite/LICENSE;md5=7256769d4613c0af5a72be32ba20ccf7 \
  file://${S}/third_party/swiftshader/LICENSE.txt;md5=d273d63619c9aeaf15cdaf76422c4f87 \
  file://${S}/third_party/usb_ids/LICENSE;md5=8f694e3758e3e57a5223bfa2bf29d452 \
  file://${S}/third_party/v4l-utils/COPYING.libv4l;md5=d749e86a105281d7a44c2328acebc4b0 \
  file://${S}/third_party/wayland-protocols/src/COPYING;md5=c7b12b6702da38ca028ace54aae3d484 \
  file://${S}/third_party/wayland/src/COPYING;md5=b31d8f53b6aaf2b4985d7dd7810a70d1 \
  file://${S}/third_party/webrtc/LICENSE;md5=ad296492125bc71530d06234d9bfebe0 \
  file://${S}/third_party/woff2/LICENSE;md5=027c71da9e4664fdf192e6ec615f4d18 \
  file://${S}/third_party/wtl/Ms-PL.txt;md5=1d2c3cdd7e748a6bc4213321d9157c5a \
  file://${S}/third_party/xdg-utils/LICENSE;md5=a5367a90934098d6b05af3b746405014 \
  file://${S}/third_party/zlib/LICENSE;md5=8c75f2b4df47a77f9445315a9500cd1c \
  file://${S}/url/third_party/mozilla/LICENSE.txt;md5=437ced1e9b232651b0912a9594da43b2 \
  file://${S}/v8/LICENSE;md5=f38a2942edcfe4abb45e9a83a1ad2f82 \
  file://${S}/v8/LICENSE.fdlibm;md5=b17367d6c97e638599071d99a3049dfe \
  file://${S}/v8/LICENSE.strongtalk;md5=956a43818dd13f2e93088fc2c93589d2 \
"

# Upstreamable patches
SRC_URI += " \
  file://0001-GCC-do-not-add-redundant-template-spec-in-constructo.patch \
  file://0002-IWYU-missing-include-for-usage-of-std-variant-in-sav.patch \
  file://0003-IWYU-missing-include-for-usage-of-uint64_t-in-visual.patch \
  file://0001-Add-namespace-for-usage-of-std-isfinite.patch;patchdir=third_party/ink/src \
  file://0001-Reapply-IWYU-fix-for-std-string-usage-in-instrumenta.patch;patchdir=third_party/ruy/src \
"

# Upstream patches
SRC_URI += " \
  file://0004-PA-Fix-include-for-memcpy-memset.patch \
"

# Follow target_os in cros_toolchain
SRC_URI += " file://0001-Fix-Linux-cross-build-using-cros_toolchain.patch "

# ARM build should not use Clang MTE flags
SRC_URI += " file://0001-GCC-do-not-pass-Clang-specific-MTE-flags.patch "

# Disable SVE extensions in libvpx, as they require GCC 14+
SRC_URI += " file://0001-Drop-SVE-intrinsics-in-libvpx-for-GCC.patch "

# Fix for GCC 11/12 to avoid breaking DPF compilation reported at
# https://gcc.gnu.org/bugzilla/show_bug.cgi?id=109505
SRC_URI += " file://0001-GCC-disable-SVE-to-avoid-breaking-compilation-with-D.patch;patchdir=third_party/highway/src "

# XNNPACK fails to build ARM64 intrinsics in our build. Disable meanwhile.
GN_ARGS:append:raspberrypi4-64 = " build_tflite_with_xnnpack=false build_webnn_with_xnnpack=false"

# Building with static roots fails on GCC 13. As recent hosts can ship with
# that version of, disable it.
GN_ARGS:append = " v8_enable_static_roots=false "

# Using Skia Fontations breaks Chromium linking because of missing Rust symbols
GN_ARGS:append = " use_typeface_fontations=false "

# Rust fails to generate the bindings for crabbyavif with libstdc++
GN_ARGS:append = " enable_av1_decoder=false "

# GCC ARM64 SME support is only available from GCC 14+
GN_ARGS:append = " libyuv_use_sme=false "

do_install:append() {
        install -m 0755 chrome_crashpad_handler ${D}${libdir}/chromium/chrome_crashpad_handler
}
