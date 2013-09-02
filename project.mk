PROJECT=if3kernel
PROJECT_NAME=if3kernel
PROJECT_MAINTAINER=jeffk
PROJECT_COPYRIGHT=2008
PROJECT_EMAIL=jeffk@internetfilter.com
PROJECT_LICENSE=closed
PROJECT_LICENSE_FILE=$(PROJECT_TOP_DIR)/LICENSE.txt
PROJECT_README_FILE=$(PROJECT_TOP_DIR)/README.txt
PROJECT_COMPANY=Turner and Sons Productions, Inc.
PROJECT_WEBSITE=https://dev.internetfilter.com/project/projects/test-if3kernel
PROJECT_DESCRIPTION=if3kernel
PROJECT_VERSION=3.0
PROJECT_IDENTIFIER=com.contextualmediagroup.if3kernel
TOP_LIB_DIRS+=.
SUBLIBS=
INCLUDES+=
DEFINES+=_REENTRANT JDKSBOOST_ENABLE_POSIX JDKSBOOST_ENABLE_PTHREADS
COMPILE_FLAGS+=-Wall -pthread
LINK_FLAGS+=
PKGCONFIG_PACKAGES+=
CONFIG_TOOLS+=

ENABLE_PRECOMPILED_HEADERS?=1
PRECOMPILED_HEADER=$(PROJECT_TOP_DIR)/include/if3_world_precompiled.hpp

COMPILE_FLAGS+=-fPIC

ENABLE_SOCI=0
ENABLE_SOCI_POSTGRESQL=0

ifeq ($(ENABLE_SOCI)$(ENABLE_SOCI_POSTGRESQL),11)
SOCI_CONFIG?=$(PREFIX)/bin/soci-config
POSTGRESQL_CONFIG?=pg_config
POSTGRESQL_INCLUDES:=$(shell $(POSTGRESQL_CONFIG) --includedir)
POSTGRESQL_LDFLAGS:=-L$(shell $(POSTGRESQL_CONFIG) --libdir)
INCLUDES+=$(POSTGRESQL_INCLUDES)
LDFLAGS+=$(POSTGRESQL_LDFLAGS)
#LDLIBS+=-lpgport -lpgtypes -lpq -lpam -lz
LDLIBS+=-lpgport -lpq -lpam -lz
endif

#BOOST_CONFIG?=$(PREFIX)/bin/jdksboost-config
#JDKS2K9_CONFIG?=$(PREFIX)/bin/jdks2k9-config

LDLIBS_POSIX+=-pthread
LDLIBS_POSIX+=-lssl -lcrypto

CONFIG_TOOLS+=$(BOOST_CONFIG) $(JDKS2K9_CONFIG) $(SOCI_CONFIG)

PREFIX?=/opt/if3

ifeq ($(PROJECT_VARIANT),debian)
#TARGET_DIR_STYLE=debian
#PACKAGE_CHOWN?=root:root
PACKAGE_CHMOD?=go-rw,u+rwX
endif

ifeq ($(PROJECT_VARIANT),ubuntu)
#TARGET_DIR_STYLE=ubuntu
#PACKAGE_CHOWN?=root:root
PACKAGE_CHMOD?=go-rw,u+rwX
endif

ifeq ($(PROJECT_VARIANT),macosx)
TARGET_DIR_STYLE=macosx-library
PACKAGE_CHOWN=
PACKAGE_CHMOD=
#PREFIX=/Library/contextualmediagroup/$(PROJECT)-$(VERSION)
endif


PYTHON_MODULES_DIR ?= $(PROJECT_TOP_DIR)/python

TARGET_PYTHON_DIR ?= $(TARGET_BIN_DIR)/
OUTPUT_PYTHON_DIR ?= $(OUTPUT_DIR)/python
OUTPUT_PYTHON_BUILD_DIR ?= $(OUTPUT_PYTHON_DIR)/build
OUTPUT_PYTHON_BDIST_DIR ?= $(OUTPUT_PYTHON_DIR)/bdist

ALL_OUTPUT_DIRS += $(OUTPUT_PYTHON_BUILD_DIR) $(OUTPUT_PYTHON_BDIST_DIR) $(OUTPUT_PYTHON_DIST_DIR)

ALL_TARGETS += python-build

.PHONY : python-build python-bdist python-install

python-build : lib
	@( echo Python Modules: && \
	cd $(PYTHON_MODULES_DIR) && \
	PROJECT_TOP_DIR=$(PROJECT_TOP_DIR) LIB_DIR=$(OUTPUT_LIB_DIR) $(PYTHON) setup.py build -f -b $(OUTPUT_PYTHON_BUILD_DIR) $(PYTHON_MODULES_BUILD_OPTIONS) )

PREINSTALL_MAIN_DEPS += python-preinstall

.PHONY : python-preinstall

python-preinstall : python-build
	$(CP) -rp $(OUTPUT_PYTHON_BUILD_DIR)/lib.*/* $(LOCAL_INSTALL_BIN_DIR)/

python-bdist : lib
	@(cd $(PYTHON_MODULES_DIR) && \
	LIB_DIR=$(OUTPUT_LIB_DIR) $(PYTHON) setup.py bdist -b $(OUTPUT_PYTHON_BDIST_DIR) $(PYTHON_MODULES_BDIST_OPTIONS) )

python-install : lib
	@(cd $(PYTHON_MODULES_DIR) && \
	LIB_DIR=$(OUTPUT_LIB_DIR) $(PYTHON) setup.py install $(PYTHON_MODULES_INSTALL_OPTIONS) )


COMPILE_FLAGS+=-Wall -W
COMPILE_FLAGS+=-std=c++11

ifeq ($(TARGET_PLATFORM_MACOSX),1)
COMPILE_FLAGS+=-stdlib=libc++
LINK_FLAGS+=-stdlib=libc++
COMPILER=clang
MAC_OSX_VERSION?=10.8
MAC_SDK?=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX$(MAC_OSX_VERSION).sdk
COMPILE_FLAGS+=-mmacosx-version-min=$(MAC_OSX_VERSION)
LINK_FLAGS+=-mmacosx-version-min=$(MAC_OSX_VERSION) -Wl,-syslibroot,$(MAC_SDK)
endif




