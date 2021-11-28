CFLAGS = -ansi -pedantic -Wall -Wextra -std=c99
if COND_GCOV
CFLAGS += -O0 -g -fprofile-arcs -ftest-coverage
else
CFLAGS += -O3 -g3
endif

lib_LTLIBRARIES = libbuf.la

gcov_lines:
	gcov -o .libs libbuf -t -k -b | egrep -v '[[:digit:]]:[ /][*]'

gcov_summary:
	gcov -o .libs libbuf -j

