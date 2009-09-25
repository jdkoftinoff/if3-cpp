/*
  
The Internet Filter IF2K Version 2
Source Code

Written By Jeff Koftinoff <jeffk@jdkoftinoff.com>
Copyright (c) 1995-2005
By Turner and Sons Productions Ltd.
http://www.internetfilter.com/

ALL RIGHTS RESERVED.

*/

#include "if2k_world.h"
#include "if2k_string.h"
#include "if2k_util.h"
#include "if2k_compile.h"


using namespace if2k;

static void usage()
{
  fprintf( 
    stderr, 
    "usage:\nif2ktool_compile_all [source dir] [dest dir]\n" 
    "source dir should contain the files:\n"
    "\t#badurl.txt   (bad url lists)\n"
    "\t#postbadurl.txt   (post bad url lists)\n"    
    "\t#goodurl.txt  (good url lists)\n"
    "\t#badphr.txt   (bad phrase lists)\n"
    "\n"
    "When # is 1,2,3,4,5,6,7,8\n"
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

