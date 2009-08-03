PROJECT=if2kkernel
PROJECT_NAME=if2kkernel
PROJECT_MAINTAINER=jeffk
PROJECT_COPYRIGHT=2008
PROJECT_EMAIL=jeffk@internetfilter.com
PROJECT_LICENSE=closed
PROJECT_LICENSE_FILE=$(PROJECT_TOP_DIR)/LICENSE.txt
PROJECT_README_FILE=$(PROJECT_TOP_DIR)/README.txt
PROJECT_COMPANY=Turner and Sons Productions, Inc.
PROJECT_WEBSITE=www.internetfilter.com
PROJECT_DESCRIPTION=if2kkernel
PROJECT_VERSION=3.0
TOP_LIB_DIRS+=.
SUBLIBS=
INCLUDES+=
DEFINES+=
COMPILE_FLAGS+=-Wall
LINK_FLAGS+=
PKGCONFIG_PACKAGES+=
CONFIG_TOOLS+=

LDLIBS_POSIX+=-lpthread -lssl -lcrypto

#SWIG_PYTHON_OUTPUTS=if2k_py_swig_wrap.cpp if2kkernel.py

#$(OUTPUT_SWIG_PYTHON_DIR)/if2k_py_swig_wrap.cpp : $(PROJECT_TOP_DIR)/swig/if2k_swig.i
#	$(SWIG) $(addprefix -I,$(INCLUDES)) $(addprefix -D,$(DEFINES)) -DIF2K_CONFIG_POSIX -Wall -classic -python -c++ -module if2kkernel -o $(OUTPUT_SWIG_PYTHON_DIR)/if2k_swig_wrap.cpp -outdir $(OUTPUT_SWIG_PYTHON_DIR) $(PROJECT_TOP_DIR)/swig/if2k_swig.i


#ALL+=$(OUTPUT_SWIG_PYTHON_DIR)/if2k_py_swig_wrap.cpp


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
PROJECT_IDENTIFIER=com.internetfilter.if2kkernel
PREFIX=/Library/internetfilter/$(PROJECT)-$(VERSION)
endif




