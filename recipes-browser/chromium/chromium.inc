DESCRIPTION = "Chromium is an open-source browser project that aims to build a safer, faster, and more stable way for all users to experience the web."
HOMEPAGE = "https://www.chromium.org/Home"

DEPENDS = "\
    ca-certificates-native \
    curl \
    gn-native \
    gtk+3 \
    ninja-native \
    nodejs-native \
    nspr \
    nspr-native \
    nss \
    nss-native \
    pciutils \
    pkgconfig-native \
    virtual/libgl \
"

inherit gtk-icon-cache

# Chromium expects python3 by default for building, but it did not complete the migration
# so some scripts also require python2 in the path (see python2_action in codebase).
inherit python3native setuptools
DEPENDS += "python-native"
EXTRANATIVEPATH += "python-native"
# We need to define this to allow using Python2. Yes, I swear we are on it. Hopefully
# soon Chromium will drop python2_action. So, if you want build to work, add this to your
# local.conf or equivalent
# I_SWEAR_TO_MIGRATE_TO_PYTHON3 = "yes"

inherit mime-xdg


B = "${WORKDIR}/build"

SRC_URI += " \
        file://wrapper-extra-flags.patch \
        "

# At the moment, this recipe has only been tested on i586, x86-64, ARMv6,
# ARMv7a and aarch64.
COMPATIBLE_MACHINE = "(-)"
COMPATIBLE_MACHINE_aarch64 = "(.*)"
COMPATIBLE_MACHINE_armv6 = "(.*)"
COMPATIBLE_MACHINE_armv7a = "(.*)"
COMPATIBLE_MACHINE_x86 = "(.*)"
COMPATIBLE_MACHINE_x86-64 = "(.*)"
COMPATIBLE_MACHINE_raspberrypi4 = "(.*)"
COMPATIBLE_MACHINE_raspberrypi3 = "(.*)"
COMPATIBLE_MACHINE_raspberrypi2 = "(.*)"

# The wrapper script we use from upstream requires bash.
RDEPENDS_${PN} = "bash"

PACKAGECONFIG ??= "\
  ftp \
  use-gold \
  ${@bb.utils.filter('DISTRO_FEATURES', 'pulseaudio wayland x11', d)} \
"

# ftp: Whether to build Chromium with support for the FTP protocol.
PACKAGECONFIG[ftp] = "disable_ftp_support=false,disable_ftp_support=true"
# proprietary-codecs: If enabled, this option will build Chromium with support
# for additional codecs in FFMPEG (such as the MPEG-LA ones). It is your
# responsibility to ensure you are complying with all required licenses.
PACKAGECONFIG[proprietary-codecs] = '\
        ffmpeg_branding="Chrome" proprietary_codecs=true, \
        ffmpeg_branding="Chromium" proprietary_codecs=false \
        '
# use-gold: Whether to link Chromium with ld-gold or not.
PACKAGECONFIG[use-gold] = "use_gold=true,use_gold=false"

# Use media acceleration provided by libVA
PACKAGECONFIG[vaapi] = "use_vaapi=true,use_vaapi=false,libva"

# Build with pulseaudio support
PACKAGECONFIG[pulseaudio] = "use_pulseaudio=true,use_pulseaudio=false,pulseaudio"

# Compile with wayland support
PACKAGECONFIG[wayland] = "ozone_platform_wayland=true,ozone_platform_wayland=false,virtual/egl wayland libxkbcommon"

# Compile with X11 support
PACKAGECONFIG[x11] = "\
    use_x11=true ozone_platform_x11=true,\
    use_x11=false ozone_platform_x11=false,\
    libx11 libxcomposite libxcursor libxdamage libxext libxfixes libxi libxrandr libxrender libxscrnsaver libxtst\
"

# Base GN arguments, mostly related to features we want to enable or disable.
GN_ARGS = "\
        ${PACKAGECONFIG_CONFARGS} \
        use_cups=false \
        use_gconf=false \
        use_gnome_keyring=false \
        use_kerberos=false \
        use_system_libjpeg=false \
        use_system_libwayland=true \
        use_system_minigbm=true \
        use_system_libdrm=true \
        use_xkbcommon=true \
"

# From Chromium's BUILDCONFIG.gn:
# Set to enable the official build level of optimization. This has nothing
# to do with branding, but enables an additional level of optimization above
# release (!is_debug). This might be better expressed as a tri-state
# (debug, release, official) but for historical reasons there are two
# separate flags.
# See also: https://groups.google.com/a/chromium.org/d/msg/chromium-dev/hkcb6AOX5gE/PPT1ukWoBwAJ
GN_ARGS += "is_debug=false is_official_build=true"

# By default, passing is_official_build=true to GN causes its symbol_level
# variable to be set to "2". This means the compiler will be passed "-g2" and
# we will end up with a very large chrome binary (around 5Gb as of M58)
# regardless of whether DEBUG_BUILD has been set or not. In addition, binutils,
# file and other utilities are unable to read a 32-bit binary this size, which
# causes it not to be stripped.
# The solution is:
# 1. Make sure -g is not passed via DEBUG_FLAGS. -g is the same as -g2. We
#    do not want to override the explicit debug level setting we are adding
#    through Chromium GN symbol-level variable.
# 2. Optimizations are dropped, as Chromium will select the ones it needs.
# 3. Explicitly pass symbol_level=1 to GN. This causes -g1 to be passed
#    instead. This reduces around 800MB of the binary size, and that can
#    be more easily handled.
# 4. Provide "debug" PACKAGECONFIG that enables full debugging.
# 5. Drop debug fission explicitely. Yocto will provide its own splitting
#    of debugging information. WARNING: do not set it debugging for ELF 32
#    bits platforms as it will cause issues compiling, linking and
#    packaging.
DEBUG_FLAGS_remove = "-g"
SELECTED_OPTIMIZATION = "${DEBUG_FLAGS}"
PACKAGECONFIG[debug] = "symbol_level=2, symbol_level=1"
GN_ARGS += "use_debug_fission=false"

# By default we discard adding debug symbols for Blink. In case debug-blink is
# added to PACKAGECONFIG, then the debugging level will match the one set for
# the whole package (2 if "debug" variable is added, 1 if not).
PACKAGECONFIG[debug-blink] = ",blink_symbol_level=0"

# Disable Chrome Remote Desktop (aka Chromoting) support. Building host support
# (so that the machine running this recipe can be controlled remotely from
# another machine) requires additional effort to build some extra binaries,
# whereas connecting to other machines requires building and installing a
# Chrome extension (it is not clear how to do that automatically).
GN_ARGS += "enable_remoting=false"

# NaCl support depends on the NaCl toolchain that needs to be built before NaCl
# itself. The whole process is quite cumbersome so we just disable the whole
# thing for now.
GN_ARGS += "enable_nacl=false"

# We do not want to use Chromium's own Debian-based sysroots, it is easier to
# just let Chromium's build system assume we are not using a sysroot at all and
# let Yocto handle everything.
GN_ARGS += "use_sysroot=false"

# Upstream Chromium uses clang on Linux, and GCC is not regularly tested. This
# means new GCC releases can introduce build failures as Chromium uses "-Wall
# -Werror" by default and we do not have much control over which warnings GCC
# decides to include in -Wall.
GN_ARGS += "treat_warnings_as_errors=false"

# Disable activation of field trial tests that can cause problems in
# production.
# See https://groups.google.com/a/chromium.org/d/msg/chromium-packagers/ECWC57W7E0k/9Kc5UAmyDAAJ
GN_ARGS += "fieldtrial_testing_like_official_build=true"

# Disable compiling closures, as they require java runtime
GN_ARGS += "enable_js_type_check=false"

# Libaom fails to build in 
GN_ARGS += "enable_libaom=false"

# Ozone and X11 backends
GN_ARGS += "\
    ozone_auto_platforms=false \
    ozone_platform_headless=true \
"

# API keys for accessing Google services. By default, we use an invalid key
# only to prevent the "you are missing an API key" infobar from being shown on
# startup.
# See https://dev.chromium.org/developers/how-tos/api-keys for more information
# on how to obtain your own keys. You can then set the variables below in
# local.conf or a .bbappend file.
GOOGLE_API_KEY ??= "invalid-api-key"
GOOGLE_DEFAULT_CLIENT_ID ??= "invalid-client-id"
GOOGLE_DEFAULT_CLIENT_SECRET ??= "invalid-client-secret"
GN_ARGS += '\
        google_api_key="${GOOGLE_API_KEY}" \
        google_default_client_id="${GOOGLE_DEFAULT_CLIENT_ID}" \
        google_default_client_secret="${GOOGLE_DEFAULT_CLIENT_SECRET}" \
        '
def gn_arch_name(yocto_arch):
    """Translates between Yocto's architecture values and the corresponding
    ones used by GN."""
    translation_table = {
        'aarch64': 'arm64',
        'arm': 'arm',
        'i586': 'x86',
        'i686': 'x86',
        'x86_64': 'x64',
    }
    try:
        return translation_table[yocto_arch]
    except KeyError:
        bb.msg.fatal('"%s" is not a supported architecture.' % yocto_arch)

# Use ChromeOS toolchains, as they are also working for cross compiling
# Chrome. They should allow to use GCC toolchains accordingly.
GN_ARGS += '\
  host_toolchain="//build/toolchain/cros:host" \
  cros_host_is_clang=false \
  use_custom_libcxx_for_host=false \
  cros_host_ar="${BUILD_AR}" \
  cros_host_cc="${BUILD_CC}" \
  cros_host_cxx="${BUILD_CXX}" \
  cros_host_extra_ldflags="${BUILD_LDFLAGS}" \
  custom_toolchain="//build/toolchain/cros:target" \
  is_clang=false \
  use_custom_libcxx=false \
  target_cpu="${@gn_arch_name('${TUNE_ARCH}')}" \
  cros_target_ar="${AR}" \
  cros_target_cc="${CC}" \
  cros_target_cxx="${CXX}" \
  cros_target_extra_cflags="${CFLAGS}" \
  cros_target_extra_cxxflags="${CXXFLAGS}" \
  cros_target_extra_cppflags="${CPPFLAGS}" \
  cros_target_extra_ldflags="${LDFLAGS}" \
  v8_snapshot_toolchain="//build/toolchain/cros:v8_snapshot" \
  cros_v8_snapshot_is_clang=false \
  cros_v8_snapshot_ar="${BUILD_AR}" \
  cros_v8_snapshot_cc="${BUILD_CC}" \
  cros_v8_snapshot_cxx="${BUILD_CXX}" \
  gold_path="" \
  v8_enable_embedded_builtins=false \
  use_v8_context_snapshot=false \
'

# As we use GCC, disable cfi and thin LTO
GN_ARGS += '\
  is_cfi=false \
  use_thin_lto=false \
'

# ARM builds need special additional flags (see ${S}/build/config/arm.gni).
ARM_FLOAT_ABI = "${@bb.utils.contains('TUNE_FEATURES', 'callconvention-hard', 'hard', 'softfp', d)}"
GN_ARGS_append_armv6 = ' arm_version=6 arm_float_abi="${ARM_FLOAT_ABI}"'
GN_ARGS_append_armv7a = ' arm_version=7 arm_float_abi="${ARM_FLOAT_ABI}"'
# tcmalloc's atomicops-internals-arm-v6plus.h uses the "dmb" instruction that
# is not available on (some?) ARMv6 models, which causes the build to fail.
GN_ARGS_append_armv6 += 'use_allocator="none"'
# The WebRTC code fails to build on ARMv6 when NEON is enabled.
# https://bugs.chromium.org/p/webrtc/issues/detail?id=6574
GN_ARGS_append_armv6 += 'arm_use_neon=false'

# For Raspberry PI, default arm_arch setting is not valid and caused warnings
GN_ARGS_append_armv7ve += 'arm_arch="armv7ve"'

# Crashpad uses zlib internally, but it still does not handle properly the
# NEON codepath for GCC.
GN_ARGS_append_armv7ve += 'arm_use_neon=false'

# Libaom fails to build on ARM because of missing aom_arm_cpu_caps symbol
GN_ARGS_append_arm = ' enable_libaom=false'

# Right now GBM buffers are not working properly on v3d driver used in RPI4
GN_ARGS_append_raspberrypi4 = " use_wayland_gbm=false"
GN_ARGS_append_raspberrypi4-64 = " use_wayland_gbm=false"

# JXL decoder depends on highway, that fails to build NEON instrinsics in GCC
GN_ARGS_append_aarch64 = " enable_jxl_decoder=false"

# Temporary fix while libyuv ARM64 neon support is broken on GCC
SRC_URI += " file://0001-GCC-allow-to-disable-NEON-with-GN.patch;patchdir=third_party/libyuv "
GN_ARGS_append_aarch64 = " libyuv_use_neon=false "

CHROMIUM_EXTRA_ARGS ?= "\
  --enable-features=UseOzonePlatform \
"

do_add_nodejs_symlink () {
	# Adds a symlink to the node binary to the location expected by
	# Chromium's build system.
	chromium_node_dir="${S}/third_party/node/linux/node-linux-x64/bin"
	nodejs_native_path="${STAGING_BINDIR_NATIVE}/node"
	mkdir -p "${chromium_node_dir}"
	if [ ! -f "${nodejs_native_path}" ]; then
		echo "${nodejs_native_path} does not exist."
		exit 1
	fi
	ln -sf "${nodejs_native_path}" "${chromium_node_dir}/node"
}
addtask add_nodejs_symlink after do_configure before do_compile

do_configure() {
	cd ${S}
	gn gen --args='${GN_ARGS}' "${B}"
}

do_compile[progress] = "outof:^\[(\d+)/(\d+)\]\s+"
do_compile() {
        # We need to do this because curl by default will try to fetch ca-certificates.crt
        # from the native sysroot used to compile curl-native
        export CURL_CA_BUNDLE=${STAGING_DIR_NATIVE}/etc/ssl/certs/ca-certificates.crt
	ninja "${PARALLEL_MAKE}" chrome chrome_sandbox
}

PRODUCT_LOGO_SIZES = "24 48 64 128 256"

do_install() {
	install -d ${D}${bindir}
	install -d ${D}${datadir}
	install -d ${D}${datadir}/applications
	install -d ${D}${datadir}/icons
	install -d ${D}${datadir}/icons/hicolor
	install -d ${D}${libdir}/chromium
	install -d ${D}${libdir}/chromium/locales

	# Process and install Chromium's template .desktop file.
	sed -e "s,@@MENUNAME@@,Chromium Browser,g" \
	    -e "s,@@PACKAGE@@,chromium,g" \
	    -e "s,@@USR_BIN_SYMLINK_NAME@@,chromium,g" \
	    ${S}/chrome/installer/linux/common/desktop.template > chromium.desktop
	install -m 0644 chromium.desktop ${D}${datadir}/applications

	# Install icons.
	for size in ${PRODUCT_LOGO_SIZES}; do
		install -d ${D}${datadir}/icons/hicolor/${size}x${size}
		install -d ${D}${datadir}/icons/hicolor/${size}x${size}/apps
		install -m 0644 \
			${S}/chrome/app/theme/chromium/product_logo_${size}.png \
			${D}${datadir}/icons/hicolor/${size}x${size}/apps/chromium.png
	done

	# A wrapper for the proprietary Google Chrome version already exists.
	# We can just use that one instead of reinventing the wheel.
	WRAPPER_FILE=${S}/chrome/installer/linux/common/wrapper
	sed -e "s,@@CHANNEL@@,stable,g" \
		-e "s,@@PROGNAME@@,chromium-bin,g" \
		${WRAPPER_FILE} > chromium-wrapper
	install -m 0755 chromium-wrapper ${D}${libdir}/chromium/chromium-wrapper
	ln -s ${libdir}/chromium/chromium-wrapper ${D}${bindir}/chromium

	# Add extra command line arguments to the chromium-wrapper script by
	# modifying the dummy "CHROME_EXTRA_ARGS" line
	sed -i "s/^CHROME_EXTRA_ARGS=\"\"/CHROME_EXTRA_ARGS=\"${CHROMIUM_EXTRA_ARGS}\"/" ${D}${libdir}/chromium/chromium-wrapper

	install -m 4755 chrome_sandbox ${D}${libdir}/chromium/chrome-sandbox
	install -m 0755 chrome ${D}${libdir}/chromium/chromium-bin
	install -m 0644 *.bin ${D}${libdir}/chromium/
	install -m 0644 chrome_*.pak ${D}${libdir}/chromium/
	install -m 0644 icudtl.dat ${D}${libdir}/chromium/icudtl.dat
	install -m 0644 resources.pak ${D}${libdir}/chromium/resources.pak

	install -m 0644 locales/*.pak ${D}${libdir}/chromium/locales/

        install -d ${D}${libdir}/chromium/swiftshader
        install -m 0755 swiftshader/* ${D}${libdir}/chromium/swiftshader
}

# Swiftshader libraries installed in chromium libdir do not have versioning
INSANE_SKIP_${PN} = "dev-elf"

FILES_${PN} = " \
        ${bindir}/chromium \
        ${datadir}/applications/chromium.desktop \
        ${datadir}/icons/hicolor/*x*/apps/chromium.png \
        ${libdir}/chromium/* \
        "
FILES_${PN}-dbg = "${libdir}/${PN}/.debug/"
PACKAGE_DEBUG_SPLIT_STYLE = "debug-without-src"

# There is no need to ship empty -dev packages.
ALLOW_EMPTY_${PN}-dev = "0"
