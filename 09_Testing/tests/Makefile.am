TESTS_ENVIRONMENT = CK_VERBOSITY=$(CK_VERBOSITY)
all_my_tests = libbuf_tests
TESTS = $(all_my_tests)
check_PROGRAMS = $(all_my_tests)

.ts.c:
	checkmk $< > $@

AM_CFLAGS = -I$(top_builddir)/src @CHECK_CFLAGS@
LDADD = $(top_builddir)/src/libbuf.la @CHECK_LIBS@

