from distutils.core import setup, Extension
import os
import sys

pwd = os.getcwd()

lib_dir = os.getenv('LIB_DIR')
if lib_dir is None:
    lib_dir = pwd + '/../tmp-target/build/lib'

include_dir = os.getenv('INCLUDE_DIR')
if include_dir is None:
    include_dir = pwd + '/../include'

ccflags = os.getenv('CCFLAGS')
if ccflags is None:
    ccflags = ''

ifcmgkernel = Extension(
    'ifcmgkernel',
    include_dirs = [ include_dir ],
    library_dirs = [ lib_dir ],
    libraries = [ 'ifcmgkernel' ],
    sources = ['ifcmgkernel.cpp'],
    )

setup (name = 'ifcmgkernel',
       version = '3.0',
       description = 'Internet Filter Kernel For Contextual Media Group',
       ext_modules = [ifcmgkernel])

