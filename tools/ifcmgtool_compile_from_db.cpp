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


struct db_needle_t
{
  db_needle_t() :
    m_id(0),
    m_needle(""),
    m_section(-1),
    m_autogen(0),
    m_score(1)
  {
    for( int i=0; i<4; ++i )
    {
      m_category[i] = -1;
    }
  }

  friend
  std::ostream & operator << ( std::ostream &o, const db_needle_t &v )
  {
    o << v.m_id << "," << "\"" << v.m_needle << "\"," << v.m_section << ","
      << v.m_category[0] << "," << v.m_category[1] << ","
      << v.m_category[2] << "," << v.m_category[3] << ","
      << (v.m_autogen ? "true" : "false") << "," << v.m_score;
      
    return o;
  }
  
  int64_t m_id;
  std::string m_needle;
  int64_t m_section;
  int64_t m_category[4];
  int32_t m_autogen;
  int32_t m_score;
};

namespace soci
{
  template<> struct type_conversion<db_needle_t>
  {
    typedef values base_type;
    static void from_base(values const & v, indicator /* ind */, db_needle_t & n)
    {
      n.m_id = v.get<int>("ID");
      n.m_needle = v.get<std::string>("needle");
      n.m_section = v.get<int64_t>("section");
      n.m_category[0] = v.get<int64_t>("category1");
      n.m_category[1] = v.get<int64_t>("category2",int64_t(-1) );
      n.m_category[2] = v.get<int64_t>("category3",int64_t(-1) );
      n.m_category[3] = v.get<int64_t>("category4",int64_t(-1) );
      n.m_autogen = v.get<int32_t>("autogen",0);
      n.m_score = v.get<int32_t>("score",1);

    }
    static void to_base(const db_needle_t & n, values & v, indicator & ind)
    {
      v.set("id", n.m_id);
      v.set("section", n.m_section);
      v.set("category1", n.m_category[0] );
      v.set("category2", n.m_category[1] );
      v.set("category3", n.m_category[2] );
      v.set("category4", n.m_category[3] );
      v.set("autogen", n.m_autogen );
      v.set("score", n.m_score );
      ind = i_ok;
    }
  };
}

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

    db_needle_t n;
    sql << "select (id,needle,section,category1,category2,category3,category4,autogen,score) from needles where id = 12253831", into (n);

    cout << n << "\n";
  }
  catch( std::exception const &e  )
  {
    cerr << "Exception caught: " << e.what() << endl;
  }  
}


