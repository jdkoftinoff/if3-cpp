#!/usr/bin/env python

import os
import sys
import if3kernel
import urlparse


def main():
  IF3_home = '/opt/IF3'
  if os.environ.has_key('IF3_HOME'):
    IF3_home = os.environ['IF3_HOME']
  precompiled_path = os.path.join( IF3_home, 'share/if3db-pre' )
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
    

