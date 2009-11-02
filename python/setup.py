from distutils.core import setup, Extension
import os

pwd = os.getcwd()

ifcmgkernel = Extension(
    'ifcmgkernel',
    include_dirs = [ pwd + '/../include' ],
    library_dirs = [ pwd + '/../tmp-target/build/lib' ],
    libraries = [ 'ifcmgkernel' ],
    sources = ['ifcmgkernel.cpp'],
    )

setup (name = 'ifcmgkernel',
       version = '3.0',
       description = 'Internet Filter Kernel For Contextual Media Group',
       ext_modules = [ifcmgkernel])

