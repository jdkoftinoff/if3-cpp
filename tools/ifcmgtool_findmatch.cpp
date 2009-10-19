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
#include "ifcmg_tree.hpp"
#include "ifcmg_multiscanner.hpp"
#include "ifcmg_dynbuf.hpp"

using namespace ifcmg;

static void usage()
{
  fprintf( 
    stderr, 
    "usage:\n"
    "\tifcmgtool_findmatch [compiled db dir] [non compiled db dir] [input test data] [output censored data]\n" );
}

void dump( multiscanner_result_t &find_result, const string_t &title )
{
  unsigned int i;
  
  fprintf( stdout, "%s:\n", title.c_str() );
  
  fprintf( stdout, "good urls found :\t%u\n",
           find_result.get_total_good_url_match_count()
    );
  
  for( i=0; i<multiscanner_result_t::MAX_CATEGORIES; ++i )
  {
    unsigned int count=find_result.get_good_url_match_count( i );
    if( count )
    {
      fprintf( stdout, "\tcategory %u:\t%u\n", i+1, count );
    }
  }
  
  fprintf( stdout, "bad urls found :\t%u\n",
           find_result.get_total_bad_url_match_count()
    );
  
  for( i=0; i<multiscanner_result_t::MAX_CATEGORIES; ++i )
  {
    unsigned int count=find_result.get_bad_url_match_count( i );
    if( count )
    {
      fprintf( stdout, "\tcategory %u:\t%u\n", i+1, count );
    }
  }
  
  fprintf( stdout, "bad phrases found :\t%u\n",
           find_result.get_total_bad_phrase_match_count()
    );
  
  for( i=0; i<multiscanner_result_t::MAX_CATEGORIES; ++i )
  {
    unsigned int count=find_result.get_bad_phrase_match_count( i );
    if( count )
    {
      fprintf( stdout, "\tcategory %u:\t%u\n", i+1, count );
    }
  }
  fprintf( stdout, "\n" );
  
}


int main( int argc, char **argv )
{
  if( argc<4 )
  {
    usage();
    return 1;
  }
  
  const filename_t compiled_db_dir(argv[1]);
  const filename_t non_compiled_db_dir(argv[2]);
  
  const filename_t input_test_data_filename(argv[3]);
  const filename_t output_censored_data_filename(argv[4]);
  
  dynbuf_t input_test_data( input_test_data_filename );
  
  multiscanner_t ms( compiled_db_dir, non_compiled_db_dir, 0xff, 0xff, 0xff, 0xff );
  
  multiscanner_result_t find_result;
  
  find_result = ms.find_in_data(
    input_test_data.get_data(),
    input_test_data.get_data_length(),
    0xff, 
    0xff, 
    0xff,
    0xff
    );  
  
  dump( find_result, string_t("find results") );
  
  ms.censor_in_data(
    input_test_data.get_data(),
    input_test_data.get_data_length(),
    '*',
    0xff, 
    0xff,
    0xff
    );
  
  dump( find_result, string_t("censor results") );
  
  input_test_data.extract_to_file( output_censored_data_filename );
  
  return 0;
}

