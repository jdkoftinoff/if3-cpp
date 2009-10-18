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
#include "jdks2k9/jdks2k9_fmt.hpp"
#include "soci.h"
#include "soci-postgresql.h"
#include "ifcmg_tree.hpp"

static void usage()
{
  std::cerr << "usage:\nifcmgtool_compile_from_db user password name host port\n" << std::flush;
}

int main( int argc, char **argv )
{
  using namespace std;
  
  if( argc<6 )
  {
    usage();
    return 1;
  }

  std::string user = argv[1];
  std::string pass = argv[2];
  std::string name = argv[3];
  std::string host = argv[4];
  int port = (int)strtol(argv[5],0,10);

  try
  {
    using namespace soci;
    using namespace jdks2k9;
    session sql( postgresql, fmt( "dbname=%s user=%s password=%s host=%s port=%d", name, user, pass, host, port ) );

    vector<string> category_names(100);
    sql << "select name from categories", into(category_names);

    for( vector<string>::const_iterator i = category_names.begin(); i!=category_names.end(); ++i )
    {
      cout << *i << "\n";
    }
  }
  catch( std::exception const &e  )
  {
    cerr << "Exception caught: " << e.what() << endl;
  }  
}


