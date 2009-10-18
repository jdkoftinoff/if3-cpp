PROJECT=ifcmgkernel
PROJECT_NAME=ifcmgkernel
PROJECT_MAINTAINER=jeffk
PROJECT_COPYRIGHT=2008
PROJECT_EMAIL=jeffk@contextualmediagroup.com
PROJECT_LICENSE=closed
PROJECT_LICENSE_FILE=$(PROJECT_TOP_DIR)/LICENSE.txt
PROJECT_README_FILE=$(PROJECT_TOP_DIR)/README.txt
PROJECT_COMPANY=Contextual Media Group, Inc.
PROJECT_WEBSITE=https://dev.contextualmediagroup.com/project/projects/test-ifcmgkernel
PROJECT_DESCRIPTION=ifcmgkernel
PROJECT_VERSION=3.0
PROJECT_IDENTIFIER=com.contextualmediagroup.ifcmgkernel
TOP_LIB_DIRS+=.
SUBLIBS=
INCLUDES+=
DEFINES+=_REENTRANT JDKSBOOST_ENABLE_POSIX JDKSBOOST_ENABLE_PTHREADS
COMPILE_FLAGS+=-Wall -pthread
LINK_FLAGS+=
PKGCONFIG_PACKAGES+=
CONFIG_TOOLS+=

ENABLE_SOCI_POSTGRESQL=1


BOOST_CONFIG?=$(PREFIX)/bin/jdksboost-config
JDKS2K9_CONFIG?=$(PREFIX)/bin/jdks2k9-config
SOCI_CONFIG?=$(PREFIX)/bin/soci-config

LDLIBS_POSIX+=-pthread
LDLIBS_POSIX+=-lssl -lcrypto

CONFIG_TOOLS+=$(BOOST_CONFIG) $(JDKS2K9_CONFIG) $(SOCI_CONFIG)

ifeq ($(ENABLE_SOCI_POSTGRESQL),1)
POSTGRESQL_CONFIG?=pg_config
POSTGRESQL_INCLUDES:=$(shell $(POSTGRESQL_CONFIG) --includedir)
POSTGRESQL_LDFLAGS:=-L$(shell $(POSTGRESQL_CONFIG) --libdir)
INCLUDES+=$(POSTGRESQL_INCLUDES)
LDFLAGS+=$(POSTGRESQL_LDFLAGS)
#LDLIBS+=-lpgport -lpgtypes -lpq -lpam -lz
LDLIBS+=-lpgport -lpq -lpam -lz
endif

ifeq ($(PROJECT_VARIANT),debian)
TARGET_DIR_STYLE=debian
#PACKAGE_CHOWN?=root:root
PACKAGE_CHMOD?=go-rw,u+rwX
endif

ifeq ($(PROJECT_VARIANT),ubuntu)
TARGET_DIR_STYLE=ubuntu
#PACKAGE_CHOWN?=root:root
PACKAGE_CHMOD?=go-rw,u+rwX
endif

ifeq ($(PROJECT_VARIANT),macosx)
TARGET_DIR_STYLE=macosx-library
PACKAGE_CHOWN=
PACKAGE_CHMOD=
PREFIX=/Library/contextualmediagroup/$(PROJECT)-$(VERSION)
endif




