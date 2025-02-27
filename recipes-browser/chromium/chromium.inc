DESCRIPTION = "Chromium is an open-source browser project that aims to build a safer, faster, and more stable way for all users to experience the web."
HOMEPAGE = "https://www.chromium.org/Home"

DEPENDS = "\
    ca-certificates-native \
    curl \
    gn-native \
    gperf-native \
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
inherit python3native setuptools3
inherit mime-xdg

B = "${WORKDIR}/build"

SRC_URI += " \
        file://wrapper-extra-flags.patch \
        "

# At the moment, this recipe is being maintained for x86-64 and aarch64.
COMPATIBLE_MACHINE = "(-)"
COMPATIBLE_MACHINE:aarch64 = "(.*)"
COMPATIBLE_MACHINE:x86-64 = "(.*)"

# The wrapper script we use from upstream requires bash.
RDEPENDS:${PN} = "bash"

PACKAGECONFIG ??= "\
  use-gold \
  ${@bb.utils.filter('DISTRO_FEATURES', 'pulseaudio wayland x11', d)} \
"

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
    ozone_platform_x11=true,\
    ozone_platform_x11=false,\
    libx11 libxcomposite libxcursor libxdamage libxext libxfixes libxi libxrandr libxrender libxscrnsaver libxtst\
"

# Base GN arguments, mostly related to features we want to enable or disable.
GN_ARGS = "\
        ${PACKAGECONFIG_CONFARGS} \
        use_cups=false \
        use_kerberos=false \
        use_system_libjpeg=false \
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
DEBUG_FLAGS:remove = "-g"
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
GN_ARGS += "disable_fieldtrial_testing_config=true"

# Ozone and X11 backends
GN_ARGS += "\
    ozone_auto_platforms=false \
    ozone_platform_headless=true \
"

# Disable QT support
# TODO(jdapena): make this a PACKAGECONFIG
GN_ARGS += "use_qt=false"

# Disable updater build as a Yocto basic build does not require it and provides
# other means for updating
GN_ARGS += "enable_updater=false"

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

HOST_LINKER_LDFLAGS = " ${@bb.utils.contains('PACKAGECONFIG', 'use-gold', '-fuse-ld=gold', '', d)}"

# Use ChromeOS toolchains, as they are also working for cross compiling
# Chrome. They should allow to use GCC toolchains accordingly.
GN_ARGS += '\
  host_toolchain="//build/toolchain/cros:host" \
  use_custom_libcxx_for_host=false \
  cros_host_ar="${BUILD_AR}" \
  cros_host_cc="${BUILD_CC}" \
  cros_host_cxx="${@d.getVar('BUILD_CXX', True).strip()}" \
  cros_host_extra_ldflags="${BUILD_LDFLAGS} ${HOST_LINKER_LDFLAGS}" \
  custom_toolchain="//build/toolchain/cros:target" \
  is_clang=false \
  use_custom_libcxx=false \
  target_cpu="${@gn_arch_name('${TUNE_ARCH}')}" \
  target_os="linux" \
  cros_target_ar="${AR}" \
  cros_target_cc="${CC}" \
  cros_target_cxx="${CXX}" \
  cros_target_extra_cflags="${CFLAGS}" \
  cros_target_extra_cxxflags="${CXXFLAGS}" \
  cros_target_extra_cppflags="${CPPFLAGS}" \
  cros_target_extra_ldflags="${LDFLAGS}" \
  v8_snapshot_toolchain="//build/toolchain/cros:v8_snapshot" \
  cros_v8_snapshot_ar="${BUILD_AR}" \
  cros_v8_snapshot_cc="${BUILD_CC}" \
  cros_v8_snapshot_cxx="${BUILD_CXX}" \
  cros_v8_snapshot_extra_ldflags="${HOST_LINKER_LDFLAGS}" \
  gold_path="" \
  use_v8_context_snapshot=false \
'

GN_ARGS += " chrome_pgo_phase=0 "

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

do_rust_qa_unpack () {
        cp ${S}/third_party/rust-toolchain/VERSION ${S}/third_party/rust-toolchain/INSTALLED_VERSION
        ${S}/tools/rust/update_rust.py --print-package-version
}
addtask rust_qa_unpack after do_unpack before do_patch

do_configure() {
	cd ${S}
	gn gen --args='${GN_ARGS}' "${B}"
}

do_compile[progress] = "outof:^\[(\d+)/(\d+)\]\s+"
do_compile() {
        # We need to do this because curl by default will try to fetch ca-certificates.crt
        # from the native sysroot used to compile curl-native
        export CURL_CA_BUNDLE=${STAGING_DIR_NATIVE}/etc/ssl/certs/ca-certificates.crt
	ninja "${PARALLEL_MAKE}" chrome chrome_sandbox d8
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
        install -m 0755 lib*.so ${D}${libdir}/chromium/
	install -m 0644 icudtl.dat ${D}${libdir}/chromium/icudtl.dat
	install -m 0644 resources.pak ${D}${libdir}/chromium/resources.pak

	install -m 0644 locales/*.pak ${D}${libdir}/chromium/locales/

        install -m 0755 d8 ${D}${libdir}/chromium/d8
}

# Swiftshader libraries installed in chromium libdir do not have versioning
INSANE_SKIP:${PN} = "dev-elf"

PACKAGES =+ "${PN}-d8"

FILES:${PN}-d8 = " \
        ${libdir}/chromium/d8 \
"

FILES:${PN} = " \
        ${bindir}/chromium \
        ${datadir}/applications/chromium.desktop \
        ${datadir}/icons/hicolor/*x*/apps/chromium.png \
        ${libdir}/chromium/* \
        "
FILES:${PN}-dbg = "${libdir}/${PN}/.debug/"
PACKAGE_DEBUG_SPLIT_STYLE = "debug-without-src"

# There is no need to ship empty -dev packages.
ALLOW_EMPTY:${PN}-dev = "0"
