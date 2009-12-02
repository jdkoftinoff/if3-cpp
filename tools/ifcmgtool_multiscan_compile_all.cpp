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
    "usage:\nifcmgtool_multiscan_compile_all [source dir] [dest dir]\n"
    "source dir should contain the files:\n"
    "\t#-badurl.txt   (bad url lists)\n"
    "\t#-postbadurl.txt   (post bad url lists)\n"
    "\t#-goodurl.txt  (good url lists)\n"
    "\t#-badphr.txt   (bad phrase lists)\n"
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
    int count=0;

    count+=compile_files<tree_traits_url_t,pattern_expander_standard_t>(
      src_dir,
      dest_dir,
      "goodurl"
      );

    count+=compile_files<tree_traits_url_t,pattern_expander_standard_t>(
      src_dir,
      dest_dir,
      "badurl"
      );

    count+=compile_files<tree_traits_url_t,pattern_expander_standard_t>(
      src_dir,
      dest_dir,
      "postbadurl"
      );

    count+=compile_files<tree_traits_alphanumeric_t,pattern_expander_standard_t>(
      src_dir,
      dest_dir,
      "badphr"
      );

    fprintf( stdout, "%d files precompiled\n", count );
  }

  return 0;
}

