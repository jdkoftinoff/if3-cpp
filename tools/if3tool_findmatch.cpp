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
#include "if3_tree.hpp"
#include "if3_multiscanner.hpp"
#include "if3_dynbuf.hpp"

using namespace if3;

static void usage()
{
  fprintf( 
    stderr, 
    "usage:\n"
    "\tif3tool_findmatch [compiled db dir] [non compiled db dir] [input test data] [output censored data]\n" );
}

void dump( multiscanner::result_t &find_result, const string_t &title )
{
  unsigned int i;
  
  fprintf( stdout, "%s:\n", title.c_str() );
  
  fprintf( stdout, "good urls found :\t%u\n",
           find_result.get_total_good_url_match_count()
    );
  
  for( i=0; i<multiscanner::MAX_CATEGORIES; ++i )
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
  
  for( i=0; i<multiscanner::MAX_CATEGORIES; ++i )
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
  
  for( i=0; i<multiscanner::MAX_CATEGORIES; ++i )
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
  using namespace if3::multiscanner;
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
 
  multiscanner_t ms( compiled_db_dir, non_compiled_db_dir);

  result_t find_result;

  categories_enable_t good_url_enable_bits;
  categories_enable_t bad_url_enable_bits;
  categories_enable_t postbad_url_enable_bits;
  categories_enable_t bad_phrase_enable_bits;

  find_result = ms.find_in_data(
    input_test_data.get_data(),
    input_test_data.get_data_length(),
    good_url_enable_bits,
    bad_url_enable_bits, 
    postbad_url_enable_bits,
    bad_phrase_enable_bits
    );  
  
  dump( find_result, string_t("find results") );
  
  ms.censor_in_data(
    input_test_data.get_data(),
    input_test_data.get_data_length(),
    '*',
    bad_url_enable_bits,
    postbad_url_enable_bits,
    bad_phrase_enable_bits
   );
  
  dump( find_result, string_t("censor results") );
  
  input_test_data.extract_to_file( output_censored_data_filename );
  
  return 0;
}

