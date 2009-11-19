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

module_sources = [ 'ifcmgkernel.cpp' ]

ifcmgkernel = Extension(
    'ifcmgkernel',
    include_dirs = [ include_dir ],
    library_dirs = [ lib_dir ],
    libraries = [ 'ifcmgkernel' ],
    sources = [ os.path.join(src_python_dir,z) for z in module_sources ],
    )

setup (name = 'ifcmgkernel',
       version = '3.0',
       description = 'Internet Filter Kernel For Contextual Media Group',
       ext_modules = [ifcmgkernel])

