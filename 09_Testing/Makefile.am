SUBDIRS = src tests

gitclean: maintainer-clean
	git clean -fd

gcov: check
	$(MAKE) $(MAKEFLAGS) -C src gcov_summary

