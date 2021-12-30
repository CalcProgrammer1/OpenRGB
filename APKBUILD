# Contributor: Bart Ribbers <bribbers@disroot.org>
# Maintainer: Bart Ribbers <bribbers@disroot.org>
pkgname=openrgb
pkgver=0.7
pkgrel=0
pkgdesc="Open source RGB lighting control that doesn't depend on manufacturer software"
url="https://gitlab.com/CalcProgrammer1/OpenRGB"
arch="all !ppc64le" # Build fails and not supported on ppc64le
license="GPL-2.0-only"
makedepends="
	hidapi-dev
	libusb-dev
	mbedtls-dev
	qt5-qtbase-dev
	"
source="https://gitlab.com/CalcProgrammer1/OpenRGB/-/archive/release_$pkgver/OpenRGB-release_$pkgver.tar.gz
	0001-fix-build.patch
	modules-load.conf
	"
install="$pkgname.post-install"
builddir="$srcdir/OpenRGB-release_$pkgver"

build() {
	qmake-qt5 PREFIX=/usr
	make
}

check() {
	make check
}

package() {
	INSTALL_ROOT="$pkgdir" make install

	install -Dm644 "$srcdir"/modules-load.conf "$pkgdir"/usr/lib/modules-load.d/openrgb.conf
}

sha512sums="
d08ef4f1b1d890858b37a0b76145da13031b35fca0b09bc243121323f4f7fc2760319981da6071acfe0ad020fdacf8663038f7fa0f4e596283a278905f1a010b  OpenRGB-release_0.7.tar.gz
9070b8520b7ead8796c45ff90c9775ce235ae4fd818092998600cfe31c7c0b4393e10731fc727f0f29bee1aa093be5236779c38a164bf10bc1f218f53243c0a4  0001-fix-build.patch
6fc01a649fcdecd2168292ae383a5af61be8c3d3fb8b98026a779f09dc9c9e1643e3c55290abd3b262bfea55e9a6cacab902f0fae5c9396dc20028a37f566555  modules-load.conf
"
