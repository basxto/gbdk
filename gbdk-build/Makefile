TOPDIR = $(shell pwd)

VER = 2.96

TOOLSPREFIX =

TARGETCC = $(TOOLSPREFIX)gcc
TARGETRANLIB = $(TOOLSPREFIX)ranlib
TARGETAR = $(TOOLSPREFIX)ar
TARGETCXX = $(TOOLSPREFIX)g++
TARGETSTRIP = $(TOOLSPREFIX)strip
TARGETCXXFLAGS =

SDCCDIR = $(TOPDIR)/sdcc
GBDKLIBDIR = $(TOPDIR)/gbdk-lib
GBDKSUPPORTDIR = $(TOPDIR)/gbdk-support

# Base setup
EXEEXTENSION = 
HOSTOS = ppc-unknown-linux2.2
TARGETOS = ppc-unknown-linux2.2

TARGETDIR = /opt/gbdk
BUILDDIR = $(TOPDIR)/build/$(TARGETOS)/gbdk

NOISELOG = $(TOPDIR)/noise.log

all: native-build

clean: sdcc-clean gbdk-support-clean gbdk-lib-clean

distclean: clean build-dir-clean

# Cross-compilation targets
FIXUPMASKS = *.c *.h .bat *.s ChangeLog README

native-build: gbdk-build gbdk-install

cross-clean: sdcc-clean gbdk-support-clean

cross-build: gbdk-build gbdk-install cross-cleanup

cross-cleanup:
ifdef MSDOSLINEENDS
	for i in $(FIXUPMASKS); do \
		find $(BUILDDIR) -name $$i -exec unix2dos {} \; ; \
	done
endif

cross-linux-mingw32-build:
	$(MAKE) TARGETOS=i586-mingw32msvc \
		TOOLSPREFIX=i586-mingw32msvc- \
		EXEEXTENSION=.exe \
		CROSSCOMPILING=1 \
		MSDOSLINEENDS=1 \
		TARGETDIR=/sdcc \
		ARCHIVETYPE=zip \
		cross-build binary

# Base rules
gbdk-build: sdcc-build gbdk-support-build gbdk-lib-build 

gbdk-install: sdcc-install gbdk-support-install gbdk-lib-install

# Directories
build-bin-dir:
	mkdir -p $(BUILDDIR)/bin

build-dir-clean:
	rm -r $(BUILDDIR)

# Rules for sdcc
SDCCCONFIGUREFLAGS = \
		--disable-mcs51-port \
		--disable-avr-port \
		--disable-ds390-port \
		--disable-pic-port \
		--disable-i186-port \
		--disable-tlcs900h-port \
		--disable-ucsim \
		--disable-device-lib-build \
		--disable-packihx

SDCCCONFIGUREFLAGS += \
		--prefix=$(TARGETDIR) \
		--program-suffix=$(EXEEXTENSION)

sdcc-build: $(SDCCDIR)/sdccconf.h
	$(MAKE) -C $(SDCCDIR)

$(SDCCDIR)/sdccconf.h:
	cd $(SDCCDIR); CC=$(TARGETCC) CXX=$(TARGETCXX) STRIP=$(TARGETSTRIP) RANLIB=$(TARGETRANLIB) CXXFLAGS=$(TARGETCXXFLAGS) ./configure $(SDCCCONFIGUREFLAGS) --host=$(HOSTOS)

# > $(NOISELOG)

sdcc-install: sdcc-build build-bin-dir
	rm -rf tmp
	$(MAKE) -C $(SDCCDIR) install prefix=$(TOPDIR)/tmp
	cp tmp/bin/sdcc* tmp/bin/sdcpp* tmp/bin/link-* tmp/bin/as-* $(BUILDDIR)/bin

# PENDING: Hack below
sdcc-clean:
	$(MAKE) -C $(SDCCDIR) distclean
	touch $(SDCCDIR)/doc/holder.pdf

# Rules for gbdk-support
gbdk-support-build:
	$(MAKE) -C $(GBDKSUPPORTDIR)/lcc TOOLSPREFIX=$(TOOLSPREFIX) TARGETDIR=$(TARGETDIR)/

gbdk-support-install: gbdk-support-build build-bin-dir
	cp $(GBDKSUPPORTDIR)/lcc/lcc $(BUILDDIR)/bin/lcc$(EXEEXTENSION)
	$(TARGETSTRIP) $(BUILDDIR)/bin/lcc*
	cp $(GBDKSUPPORTDIR)/README $(GBDKSUPPORTDIR)/ChangeLog $(BUILDDIR)

gbdk-support-clean:
	$(MAKE) -C $(GBDKSUPPORTDIR)/lcc clean

# Rules for gbdk-lib
gbdk-lib-build:
ifndef CROSSCOMPILING
	$(MAKE) -C $(GBDKLIBDIR)/libc PORTS=gbz80 PLATFORMS=gb SDCCLIB=$(SDCCDIR)
endif

gbdk-lib-install: gbdk-lib-build
	cp -r $(GBDKLIBDIR)/libc $(GBDKLIBDIR)/include $(GBDKLIBDIR)/examples $(GBDKLIBDIR)/tools $(BUILDDIR)
	rm -rf $(BUILDDIR)/lib
	cp -r $(GBDKLIBDIR)/build $(BUILDDIR)/lib

gbdk-lib-clean:
	$(MAKE) -C $(GBDKLIBDIR) clean

# Final binary
binary: binary-tidyup
ifeq ($(ARCHIVETYPE),zip)
	cd $(BUILDDIR)/..; zip -9Xrq $(TOPDIR)/gbdk-$(VER)-$(TARGETOS).zip gbdk
else
	cd $(BUILDDIR)/..; tar czf $(TOPDIR)/gbdk-$(VER)-$(TARGETOS).tar.gz gbdk
endif

binary-tidyup:
	rm -rf `find $(BUILDDIR) -name CVS`
