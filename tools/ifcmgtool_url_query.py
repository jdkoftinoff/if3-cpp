#!/usr/bin/env python

import os
import sys
import if3kernel
import urlparse


def main():
  cmg_home = '/opt/cmg'
  if os.environ.has_key('CMG_HOME'):
    cmg_home = os.environ['CMG_HOME']
  precompiled_path = os.path.join( cmg_home, 'share/if3db-pre' )
  if3kernel.startup( 
    os.path.join(precompiled_path,'hostnames.pre'),
    os.path.join(precompiled_path,'urls.pre'),
    os.path.join(precompiled_path,'phrases.pre')
    )
  
  url = urlparse.urlsplit( sys.argv[1] )
  
  print if3kernel.scan_url( 1, url.netloc, url.geturl() )

if __name__ == "__main__":
    main()
    if3kernel.shutdown()
    

