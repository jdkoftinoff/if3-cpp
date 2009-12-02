#!/usr/bin/env python

import os
import sys
import ifcmgkernel
import urlparse


def main():
  cmg_home = '/opt/cmg'
  if os.environ.has_key('CMG_HOME'):
    cmg_home = os.environ['CMG_HOME']
  precompiled_path = os.path.join( cmg_home, 'share/ifcmgdb-pre' )
  ifcmgkernel.startup( 
    os.path.join(precompiled_path,'hostnames.pre'),
    os.path.join(precompiled_path,'urls.pre'),
    os.path.join(precompiled_path,'phrases.pre')
    )
  
  url = urlparse.urlsplit( sys.argv[1] )
  
  print ifcmgkernel.scan_url( 1, url.netloc, url.geturl() )

if __name__ == "__main__":
    main()
    ifcmgkernel.shutdown()
    

