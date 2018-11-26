# Maintainer : Sepehrdad Sh <sepehrdad.dev@gmail.com>

pkgname=Xerxes
pkgver=2.0beta;
pkgrel=1
pkgdesc="Xerxes Dos Tool Rewritten in C++"
arch=('any')
url="https://github.com/sepehrdaddev/Xerxes"
license=('GPL3')
makedepends=('cmake' 'git')
source=("git+http://github.com/sepehrdaddev/Xerxes")
sha512sums=("SKIP")

pkgver(){
    cd $pkgname/src

    grep '__version__ =' main.cpp | cut -d '=' -f 2 | tr -d "\"" | 
    sed 's/ //g'
}

build() {
    cd $pkgname
    cmake -DSTATIC=TRUE .
    make -j 9
}

package() {
    cd $pkgname

    install -dm 755 "$pkgdir/usr/bin"
    install -dm 755 "$pkgdir/usr/share/$pkgname"
    
    install -Dm 644 -t "$pkgdir/usr/share/doc/$pkgname/" README.md
    install -Dm 644 LICENSE.md "$pkgdir/usr/share/licenses/$pkgname/LICENSE"

    rm README.md LICENSE.md

    cp -a bin/* "$pkgdir/usr/share/$pkgname/"

    cat > "$pkgdir/usr/bin/$pkgname" << EOF
#!/bin/sh
exec /usr/share/$pkgname/$pkgname "\$@"
EOF
    chmod +x "$pkgdir/usr/bin/$pkgname"
}
