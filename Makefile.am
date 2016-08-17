ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libargon2.la libargon2-arch.la
include_HEADERS = include/argon2.h include/argon2-opt.h

bin_PROGRAMS = argon2
noinst_PROGRAMS = argon2-bench argon2-bench2
check_PROGRAMS = argon2-test

TESTS = argon2-test

AM_CPPFLAGS = \
	-I$(srcdir)/include \
	-I$(srcdir)/lib \
	-I$(srcdir)/arch/@ARCH@/include \
	-I$(srcdir)/arch/@ARCH@/lib

libargon2_la_CFLAGS = @PTHREAD_CFLAGS@
libargon2_la_LIBADD = libargon2-arch.la @PTHREAD_LIBS@
libargon2_la_SOURCES = \
	lib/argon2.c \
	lib/core.c \
	lib/encoding.c \
	lib/genkat.c \
	lib/impl-select.c \
	lib/thread.c \
	lib/blake2/blake2.c \
	lib/core.h \
	lib/encoding.h \
	lib/genkat.h \
	lib/impl-select.h \
	lib/thread.h \
	lib/blake2/blake2.h \
	lib/blake2/blake2-impl.h

if ARCH_X86_64
libargon2_arch_la_CPPFLAGS = $(AM_CPPFLAGS)
libargon2_arch_la_CFLAGS =

if HAVE_SSE2
libargon2_arch_la_CPPFLAGS += -DHAVE_SSE2
libargon2_arch_la_CFLAGS += -msse2
endif # HAVE_SSE2

if HAVE_SSSE3
libargon2_arch_la_CPPFLAGS += -DHAVE_SSSE3
libargon2_arch_la_CFLAGS += -mssse3
endif # HAVE_SSSE3

libargon2_arch_la_SOURCES = \
	arch/@ARCH@/lib/argon2-arch.c \
	arch/@ARCH@/lib/argon2-sse2.c \
	arch/@ARCH@/lib/argon2-ssse3.c \
	arch/@ARCH@/lib/argon2-sse2.h \
	arch/@ARCH@/lib/argon2-ssse3.h
endif # ARCH_X86_64

if ARCH_GENERIC
libargon2_arch_la_SOURCES = \
	arch/@ARCH@/lib/argon2-arch.c
endif # ARCH_GENERIC

argon2_LDADD = libargon2.la
argon2_SOURCES = src/run.c lib/core.h

argon2_bench_LDADD = libargon2.la
argon2_bench_SOURCES = src/bench.c

argon2_bench2_LDADD = libargon2.la
argon2_bench2_SOURCES = src/bench2.c src/timing.h

argon2_test_LDADD = libargon2.la
argon2_test_SOURCES = tests/test.c
