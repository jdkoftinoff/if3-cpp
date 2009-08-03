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
#include "if2k_tree.h"

static void usage()
{
  fprintf( stderr, "usage:\nif2ktool_compile [url|alpha] [inputfile] [outputfile] [codeoffset] [prefix]\n" );
}

int main( int argc, char **argv )
{
  if( argc<5 )
  {
    usage();
    return 1;
  }
  
  const char *input_filename = argv[2];
  const char *output_filename = argv[3];
  int code_offset = strtol( argv[4],0,10 );
  std::string prefix;
  
  if( argc==6 )
  {
    prefix += argv[5];
  }
  
  if( strcmp(argv[1],"url")==0 )
  {
    bool r = if2k::tree_compile<if2k::tree_traits_url_t,if2k::pattern_expander_standard_t>(
      input_filename,
      output_filename,
      code_offset,
      prefix
      );
    return int(!r);
  }
  else if( strcmp(argv[1],"alpha")==0 )
  {
    bool r = if2k::tree_compile<if2k::tree_traits_alphanumeric_t,if2k::pattern_expander_standard_t>(
      input_filename,
      output_filename,
      code_offset,
      prefix
      );
    return int(!r);
  }
  else
  {
    usage();
    return 1;
  }
}

