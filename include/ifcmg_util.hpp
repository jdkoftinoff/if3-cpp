/*
  
The IF Contextual Media Group Kernel Version 3
Source Code

Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
Copyright (c) 1995-2005
By Contextual Media Group, Inc.
http://www.contextualmediagroup.com/

ALL RIGHTS RESERVED.

*/

#ifndef IFCMG_UTIL_HPP
#define IFCMG_UTIL_HPP

#include "ifcmg_world.hpp"
#include "ifcmg_string.hpp"


namespace ifcmg
{  
  namespace util
  {
    
    bool formpath( 
      string_t &dest, 
      string_t const &path, 
      string_t const &file, 
      string_t const &extension, 
      int maxlen 
      );
    
        
    inline void sleep( int seconds )
    {
#if defined(IFCMG_CONFIG_WIN32)
      Sleep( seconds*1000 );
#else
      sleep( seconds );
#endif
    }
    
    inline void sleepms( int milliseconds )
    {
#if defined(IFCMG_CONFIG_WIN32)
      Sleep( milliseconds );
#else
      usleep( milliseconds*1000 );
#endif
      
    }
    
    inline const char *find_extension( const char *path )
    {
      int pos=std::strlen(path);
      while( --pos>=0 )
      {
        if( path[pos]=='.' )
          return &path[pos];
        if( path[pos]=='/' || path[pos]=='\\' )
          return 0;
      }
      return 0;
    }
    
#if defined(IFCMG_CONFIG_POSIX)
#define IFCMG_PATHSEP '/'
#else
#define IFCMG_PATHSEP '\\'
#endif
    
    inline filename_t &fix_directory_name(
      filename_t &d 
      )
    {
      // if path does not end in '/' or '\\' then append appropriate path sep char      
      int len = d.length();
      if( len >0 && d[len-1]!='\\' && d[len-1]!='/'  )
      {
        d += IFCMG_PATHSEP;
      }      
      return d;
    }
    
    
    inline char * strip_crlf( char *buf )
    {
      char *p=buf;
      
      while( *p )
      {
        if( *p=='\r' || *p=='\n' )
        {
          *p='\0';
          break;
        }
        ++p;
      }
      
      return buf;
    }
    
    inline bool read_line(
                          string_t &result,
                          FILE *f
                          )
    {
      bool r=false;      
      char tmpbuf[4096+1];
      tmpbuf[4096]='\0';
      
      if( fgets( tmpbuf, 4096, f ) )
      {
        strip_crlf(tmpbuf);
        result = tmpbuf;
        r=true;
      }
      else
      {
        result = "";
      }
      return r;
    }
    
    inline filename_t append_path( 
      const filename_t &base_path, 
      const filename_t &file,
      const char path_sep = IFCMG_PATHSEP
      )
    {
      filename_t result( base_path );
      fix_directory_name(
        result
        );
      result += file;
      
      return result;
    }
    
    inline bool file_exists(
      const filename_t &fname 
      )
    {
      FILE *f =fopen( fname.c_str(), "rb" );
      if( f )
      {
        fclose(f);
      }
      return f!=0;
    }
  }
}

#endif
