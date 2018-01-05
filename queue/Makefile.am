## Makefile.am -- Process this file with automake to produce Makefile.in
AM_CPPFLAGS     = -I$(top_builddir)/src -I$(top_srcdir)/src

ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = 
check_PROGRAMS = 
man1_MANS = 

bin_PROGRAMS += queue
queue_CXXFLAGS = $(HTTPD_CFLAGS) $(STXXL_CFLAGS) -DDATAROOTDIR=\"$(datarootdir)\"
queue_LDADD = $(HTTPD_LIBS) $(STXXL_LIBS)
queue_SOURCES = src/queue_main.cpp 

bin_PROGRAMS += simplepost
simplepost_CXXFLAGS = $(HTTPD_CFLAGS) $(STXXL_CFLAGS) -DDATAROOTDIR=\"$(datarootdir)\"
simplepost_LDADD = $(HTTPD_LIBS) $(STXXL_LIBS)
simplepost_SOURCES = src/simplepost.c 


TESTS = $(check_PROGRAMS)
DISTCLEANFILES = $(man1_MANS)

%.1: %
	$(patsubst %.1,./%,$@) --manpage > $@

package: changelog distdir
	tardir=$(distdir) && $(am__tar) | GZIP=$(GZIP_ENV) gzip -c >$(distdir).tar.gz
	cd $(distdir) && \
	dh_make --multi -e '$(PACKAGE_BUGREPORT)' -f ../$(distdir).tar.gz && \
	cp "$(abs_top_builddir)/ChangeLog" "debian/changelog" && \
	echo 'usr/bin/*' >> "debian/$(PACKAGE).install" && \
	echo 'usr/share/*' >> "debian/$(PACKAGE).install" && \
	echo 'usr/share/man/*/*' >> "debian/$(PACKAGE).install" && \
	sed -i 's|^Section: ogs|Section: ogs|g' "debian/control" && \
	sed -i 's|Package: ${PACKAGE}-doc|Package: ${PACKAGE}-doc\nSection: doc|g' "debian/control" && \
	echo -e "\nPackage: ${PACKAGE}-dbg\nSection: libdevel\nArchitecture: any\nDepends: ${PACKAGE} (= \$${binary:Version})\nDescription: <insert up to 60 chars description>\n <insert long description, indented with spaces>" >> debian/control && \
	echo -e "override_dh_strip:\n\tdh_strip --dbg-package=${PACKAGE}-dbg\n" >> debian/rules && \
	dpkg-buildpackage -sa -rfakeroot -us -uc

package-lint: package
	lintian -iIE --pedantic *.deb

changelog:
	dch -c ChangeLog -i
