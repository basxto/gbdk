# Makefile to get and build all the parts of GBDK

CONFIG_SHELL := $(shell if [ -x "$$BASH" ]; then echo $$BASH; \
	else if [ -x /bin/bash ]; then echo /bin/bash; \
	else echo sh; fi ; fi)

TOPDIR	:= $(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi)

BUILD = $(TOPDIR)/gbdk
SDCCLIB = $(BUILD)
CVSFLAGS = -z5
CVS = cvs
DIR = .
VER = 2.90
ROOT_GBDK = :pserver:anonymous@cvs.gbdk.sourceforge.net:/cvsroot/gbdk
ROOT_SDCC = :pserver:anonymous@cvs.sdcc.sourceforge.net:/cvsroot/sdcc

GBDK_ROOT = /usr/lib/gbdk

all: logged_in dist

clean:
	for i in sdcc gbdk-lib gbdk-support/lcc; do make -C $$i clean; done
	rm -f *~
	rm -rf $(BUILD) gbdk-lib gbdk-support sdcc logged_in

update:
	cd $(DIR); cvs $(CVSFLAGS) -d$(ROOT_SDCC) co sdcc
	cd $(DIR); cvs $(CVSFLAGS) -d$(ROOT_GBDK) co gbdk-lib
	cd $(DIR); cvs $(CVSFLAGS) -d$(ROOT_GBDK) co gbdk-support

_sdcc: sdcc/sdccconf.h
	cd sdcc; \
	for i in support/gc support/cpp src as/z80 link/z80; \
	do make -C $$i; done
	mkdir -p $(BUILD)/bin
	for i in sdcc sdcpp link-gbz80 as-gbz80; \
	do cp sdcc/bin/$$i $(BUILD)/bin; done

sdcc/sdccconf.h: sdcc/configure
	cd sdcc; \
	./configure --datadir=$(GBDK_ROOT);

_gbdk-lib: _sdcc build-lcc
	cp -r gbdk-lib/include $(BUILD)
	make -C gbdk-lib SDCCLIB=$(SDCCLIB) PORTS=gbz80 PLATFORMS=gb

build-lcc:
	make -C gbdk-support/lcc clean
	make -C gbdk-support/lcc SDCCLIB=$(SDCCLIB)/
	mkdir -p $(BUILD)/bin
	cp gbdk-support/lcc/lcc $(BUILD)/bin

_gbdk-support:
	make -C gbdk-support/lcc clean
	make -C gbdk-support/lcc SDCCLIB=$(GBDK_ROOT)/
	mkdir -p $(BUILD)/bin
	cp gbdk-support/lcc/lcc $(BUILD)/bin

dist: _sdcc _gbdk-lib _gbdk-support
	mkdir -p $(BUILD)/bin
	mkdir -p $(BUILD)/lib
	cp -r gbdk-lib/build/gbz80 $(BUILD)/lib
	cp -r gbdk-lib/build/gb $(BUILD)/lib
	cp -r gbdk-lib/examples $(BUILD)
	cp -r gbdk-lib/libc $(BUILD)
	cp -r sdcc/doc $(BUILD)
	cp gbdk-support/README $(BUILD)

zdist: dist
	tar czf gbdk-$(VER).tar.gz gbdk

logged_in:
	cvs -d$(ROOT_GBDK) login
	cvs -d$(ROOT_SDCC) login
	touch logged_in
	make -f update.mak update
