# ------------------------------------------------------------------------------
# autotools

SHELL = bash
ACLOCAL_AMFLAGS = -I build/m4 ${ACLOCAL_FLAGS}
AUTOMAKE_OPTIONS = color-tests parallel-tests
AM_MAKEFLAGS = --no-print-directory
AM_TESTS_ENVIRONMENT = LD_LIBRARY_PATH=$(abs_builddir)
check_PROGRAMS = $(default_tests)
TESTS = $(default_tests)
CLEANFILES =
EXTRA_DIST =

# remove targets if the command fails
.DELETE_ON_ERROR:

# keep intermediate files
.SECONDARY:

# keep the test-suite.log and Makefile around at all times
.PRECIOUS: $(TEST_SUITE_LOG) Makefile

AM_CPPFLAGS = \
	-include $(top_builddir)/build/config.h \
	-I $(top_srcdir)/src \
	-I $(top_builddir)/src

AM_CFLAGS = \
	-fdata-sections \
	-ffunction-sections \
	-fvisibility=hidden \
	-fno-common \
	-fPIC \
	-g \
	-O2 \
	-Wall \
	-Wundef \
	-Wextra \
	-Wshadow \
	-Winit-self \
	-Wformat=2 \
	-Wformat-security \
	-Wformat-nonliteral \
	-Wmissing-prototypes \
	-Wmissing-declarations \
	-Wdeclaration-after-statement \
	-Wredundant-decls \
	-Wno-missing-field-initializers \
	-Wno-unused-parameter \
	-Wno-inline

AM_LDFLAGS = \
	-Wl,--as-needed \
	-Wl,--no-undefined \
	-Wl,--gc-sections \
	-Wl,-z,relro \
	-Wl,-z,now \
	-pie

# ------------------------------------------------------------------------------
# test-api

test_api_SOURCES = src/test-api.c

# ------------------------------------------------------------------------------
# targets

include_HEADERS = src/c-list.h
default_tests = test-api

%.pc: %.pc.in
	$(AM_V_GEN)$(SED) \
		-e 's,@includedir\@,$(includedir),g' \
		-e 's,@libdir\@,$(libdir),g' \
		-e 's,@VERSION\@,$(VERSION),g' \
		$< > $@

pkgconfiglibdir=$(libdir)/pkgconfig
pkgconfiglib_DATA = src/libclist.pc
EXTRA_DIST += src/libclist.pc.in
CLEANFILES += src/libclist.pc
