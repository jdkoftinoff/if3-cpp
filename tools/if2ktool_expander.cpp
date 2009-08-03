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
#include "if2k_pattern_expander.h"
#include "if2k_util.h"

class dumping_target : public if2k::pattern_target_t
{
public:
  void add( const if2k::string_t &s, short flags )
  {
    fprintf( stdout, "+%s\n", s.c_str() );
  }
  
  void remove( const if2k::string_t &s )
  {
    fprintf( stdout, "-%s\n", s.c_str() );
  }
};

static void usage()
{
  fprintf( stderr, "usage:\nif2ktool_compile [prefix]\nreads stdin, expands patterns, writes to stdout.\n" );
}


int main( int argc, char **argv )
{
  if( argc<2 )
  {
    usage();
    return 1;
  }
  
  if2k::string_t prefix( argv[1] );
  
  dumping_target target;
  if2k::pattern_expander_standard_t expander;
  
  if2k::string_t line;
  while( if2k::util::read_line<4096>( line, stdin ) )
  {
    expander.expand( line, target, 0, false, prefix );
  }
  return 0;
}

