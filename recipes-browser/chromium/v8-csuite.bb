DESCRIPTION = "V8 CSuite tests"
HOMEPAGE = "https://v8.dev/"

require v8-upstream.inc
inherit allarch

# Tests data
SRC_URI += "git://chromium.googlesource.com/v8/deps/third_party/benchmarks;name=data;protocol=https;destsuffix=git/test/benchmarks/data;branch=master"
SRCREV_data = "05d7188267b4560491ff9155c5ee13e207ecd65f"
SRCREV_FORMAT= "main_common"

# Upstreamable patches
SRC_URI += " \
  file://0001-Add-system-memory-usage-to-csuite.patch \
  file://0001-csuite-invoke-python3-explicitely.patch \
"

V8_CSUITE_DIR="${datadir}/v8-csuite"

do_install() {
    install -d ${D}${V8_CSUITE_DIR}

    install -d ${D}${V8_CSUITE_DIR}/data
    for f in kraken octane sunspider; do
        cp -r ${S}/test/benchmarks/data/${f} ${D}${V8_CSUITE_DIR}/data/${f}
    done

    install -d ${D}${V8_CSUITE_DIR}/csuite
    cp -r ${S}/test/benchmarks/csuite/* ${D}/${V8_CSUITE_DIR}/csuite/
}

RDEPENDS:${PN} += "chromium-dev-d8 python3-core"

