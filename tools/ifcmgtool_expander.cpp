/*
  
The IF Contextual Media Group Kernel Version 3
Source Code

Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
Copyright (c) 1995-2005
By Contextual Media Group, Inc.
http://www.contextualmediagroup.com/

ALL RIGHTS RESERVED.

*/
#include "ifcmg_world.hpp"
#include "ifcmg_pattern_expander.hpp"
#include "ifcmg_util.hpp"

class dumping_target : public ifcmg::pattern_target_t
{
public:
  void add( const ifcmg::string_t &s, short flags )
  {
    fprintf( stdout, "+%s\n", s.c_str() );
  }
  
  void remove( const ifcmg::string_t &s )
  {
    fprintf( stdout, "-%s\n", s.c_str() );
  }
};

static void usage()
{
  fprintf( stderr, "usage:\nifcmgtool_compile [prefix]\nreads stdin, expands patterns, writes to stdout.\n" );
}


int main( int argc, char **argv )
{
  if( argc<2 )
  {
    usage();
    return 1;
  }
  
  ifcmg::string_t prefix( argv[1] );
  
  dumping_target target;
  ifcmg::pattern_expander_standard_t expander;
  
  ifcmg::string_t line;
  while( ifcmg::util::read_line<4096>( line, stdin ) )
  {
    expander.expand( line, target, 0, false, prefix );
  }
  return 0;
}

