/*
  
The IF Contextual Media Group Kernel Version 3
Source Code

Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
Copyright (c) 1995-2005
By Contextual Media Group, Inc.
http://www.contextualmediagroup.com/

ALL RIGHTS RESERVED.

*/
#include "if3_world_precompiled.hpp"
#include "if3_pattern_expander.hpp"
#include "if3_util.hpp"

class dumping_target : public if3::pattern_target_t
{
public:
  void add( const if3::string_t &s, short flags )
  {
    fprintf( stdout, "+%s\n", s.c_str() );
  }
  
  void remove( const if3::string_t &s )
  {
    fprintf( stdout, "-%s\n", s.c_str() );
  }
};

static void usage()
{
  fprintf( stderr, "usage:\nif3tool_compile [prefix]\nreads stdin, expands patterns, writes to stdout.\n" );
}


int main( int argc, char **argv )
{
  if( argc<2 )
  {
    usage();
    return 1;
  }
  
  if3::string_t prefix( argv[1] );
  
  dumping_target target;
  if3::pattern_expander_standard_t expander;
  
  if3::string_t line;
  while( if3::util::read_line( line, stdin ) )
  {
    expander.expand( line, target, 0, false, prefix );
  }
  return 0;
}

