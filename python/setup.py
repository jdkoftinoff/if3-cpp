#! /usr/bin/env python

from distutils.core import setup, Extension
import os
import sys

pwd = os.getcwd()

project_top_dir = os.getenv('PROJECT_TOP_DIR')
if project_top_dir is None:
    project_top_dir = os.path.join(pwd, '/../')

include_dir = os.path.join( project_top_dir, 'include' )

src_python_dir = os.path.join( project_top_dir, 'python' )

lib_dir = os.getenv('LIB_DIR')
if lib_dir is None:
    lib_dir = os.path.join( project_top_dir, 'tmp-target/build/lib' )

ccflags = os.getenv('CCFLAGS')

if ccflags is None:
    ccflags = ''

module_sources = [ 'if3_kernel.cpp' ]

if3kernel = Extension(
    'if3_kernel',
    include_dirs = [ include_dir ],
    library_dirs = [ lib_dir ],
    libraries = [ 'if3kernel' ],
    sources = [ os.path.join(src_python_dir,z) for z in module_sources ],
    )

setup (name = 'if3_kernel',
       version = '3.0',
       description = 'Internet Filter Kernel by Turner and Sons Productions, Inc.',
       ext_modules = [if3kernel])

