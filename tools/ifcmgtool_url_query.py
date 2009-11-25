#!/usr/bin/env python

import os
import sys
import ifcmgkernel

def main():
  CMG_PREFIX = '/usr'
  if os.environ.has_key('CMG_PREFIX'):
    CMG_PREFIX = os.environ['CMG_PREFIX']
  precompiled_path = CMG_PREFIX + '/share/ifcmgdb-pre'
  noncompiled_path = CMG_PREFIX + '/share/ifcmgdb-non'
  ifcmgkernel.startup( precompiled_path, noncompiled_path)
  print ifcmgkernel.scan_url( sys.argv[1] )

if __name__ == "__main__":
    main()
