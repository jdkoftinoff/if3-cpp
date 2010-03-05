#include "if3_world_precompiled.hpp"
#include "if3_tree.hpp"

static void usage()
{
  fprintf( stderr, "usage:\nif3tool_compile [url|alpha] [inputfile] [outputfile] [codeoffset] [prefix]\n" );
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
    bool r = if3::tree_compile<if3::tree_traits_url_t,if3::pattern_expander_standard_t>(
      input_filename,
      output_filename,
      code_offset,
      prefix
      );
    return int(!r);
  }
  else if( strcmp(argv[1],"alpha")==0 )
  {
    bool r = if3::tree_compile<if3::tree_traits_alphanumeric_t,if3::pattern_expander_standard_t>(
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

