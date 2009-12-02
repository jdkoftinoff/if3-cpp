/*

The IF Contextual Media Group Kernel Version 3
Source Code

Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
Copyright (c) 1995-2005
By Contextual Media Group, Inc.
http://www.contextualmediagroup.com/

ALL RIGHTS RESERVED.

*/

#include "ifcmg_world_precompiled.hpp"
#include "ifcmg_string.hpp"
#include "ifcmg_util.hpp"
#include "ifcmg_compile.hpp"


using namespace ifcmg;

static void usage()
{
  fprintf(
    stderr,
    "usage:\nifcmgtool_compile_all [source dir] [dest dir]\n"
    "source dir should contain the files:\n"
    "\t#-hostnames.txt   (bad url lists)\n"
    "\t#-urls.txt  (good url lists)\n"
    "\t#-phrases.txt   (bad phrase lists)\n"
    "\n"
    "When # is 001-064\n"
    );
}


int main( int argc, char **argv )
{
  if( argc<3 )
  {
    usage();
    return 1;
  }
  else
  {
    filename_t src_dir( argv[1] );
    filename_t dest_dir( argv[2] );    
    util::fix_directory_name( dest_dir );
    mkdir( dest_dir.c_str(), 0750 );
    int count=0;

    filename_t output_filename;

    form(output_filename,"%s%s.pre", dest_dir.c_str(), "hostnames");
    count+=compile_url_files_into_one( src_dir, output_filename, "hostnames", 0, 64 );

    form(output_filename,"%s%s.pre", dest_dir.c_str(), "urls");
    count+=compile_url_files_into_one( src_dir, output_filename, "urls", 0, 64 );

    form(output_filename,"%s%s.pre", dest_dir.c_str(), "phrases");
    count+=compile_url_files_into_one( src_dir, output_filename, "phrases", 0, 64 );

    fprintf( stdout, "%d files precompiled\n", count );
  }

  return 0;
}

