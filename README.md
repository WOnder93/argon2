# Argon2
A multi-arch library implementing the Argon2 password hashing algorithm.

This project is based on the [original source code](https://github.com/P-H-C/phc-winner-argon2) by the Argon2 authors. The goal of this project is to provide efficient Argon2 implementations for various HW architectures (x86, SSE, ARM, PowerPC, ...).

# Building
## Using GNU autotools

To prepare the build environment, run:
```bash
autoreconf -i
./configure
```

After that, just run `make` to build the library.

### Running tests
After configuring the build environment, run `make check` to run the tests.

## Using QMake/Qt Creator
A [QMake](http://doc.qt.io/qt-4.8/qmake-manual.html) project is also available in the `qmake` directory. You can open it in the [Qt Creator IDE](http://wiki.qt.io/Category:Tools::QtCreator) or build it from terminal:
```bash
cd qmake
qmake
make
```
