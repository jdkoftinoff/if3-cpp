#!/usr/bin/env python

import os
import sys
import ifcmgkernel

def main():
  CMG_PREFIX = '/usr'
  if os.environ.has_key('CMG_PREFIX'):
    CMG_PREFIX = os.environ['CMG_PREFIX']
  precompiled_path = CMG_PREFIX + '/share/ifcmgdb-pre'

  compiled_hostname_filename = os.path.join( precompiled_path, 'hostnames.pre' )
  compiled_url_filename = os.path.join( precompiled_path, 'urls.pre' )
  compiled_alphanumeric_filename = os.path.join( precompiled_path, 'alphanumeric.pre' )
  ifcmgkernel.startup( compiled_hostname_filename, compiled_url_filename, compiled_alphanumeric_filename )
  
  print ifcmgkernel.scan_url( sys.argv[1] )

if __name__ == "__main__":
    main()

