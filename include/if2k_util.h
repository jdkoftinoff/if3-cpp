/*
  
The Internet Filter IF2K Version 2
Source Code

Written By Jeff Koftinoff <jeffk@jdkoftinoff.com>
Copyright (c) 1995-2005
By Turner and Sons Productions Ltd.
http://www.internetfilter.com/

ALL RIGHTS RESERVED.

*/

#ifndef _IF2K_UTIL_H
#define _IF2K_UTIL_H

#include "if2k_world.h"
#include "if2k_string.h"


namespace if2k
{  
  namespace util
  {
    inline void null_pointer( const char *reason ) 
    {
      if2k_throw( reason );
    }
    
    inline bool isalnum( char c )
    {
      return (c>='0'&& c<='9') || (c>='A' && c<='Z') || (c>='a' && c<='z');
    }
    
    
    inline bool isspace( char c )
    {
      return c==' ' || c=='\t' || c=='\r' || c=='\n';	
    }
    
    inline bool isprint( char c )
    {
      return c>=0x20 && c<0x7e;	
    }
    
    
    inline bool isalpha( char c )
    {
      return (c>='A' && c<='Z') || (c>='a' && c<='z');		
    }
    
    inline char toupper( char c )
    {
      if( c>='a' && c<='z' )
      {
        c-='a'-'A';
      }
      return c;
    }
    
    inline char tolower( char c )
    {
      if( c>='A' && c<='Z' )
      {	
        c+='a'-'A';
      }
      
      return c;
    }
    
    inline bool ispunct( char c )
    {
      return isprint(c) && !isspace(c) && !isalnum(c);
    }
    
    
    
    char * strupr( char *str );
    char * strlower( char *str );
    char * strip_crlf( char *buf );
    char * strip_endws( char *buf );
    char * strip_begws( char *buf );
    char * parsequoted( char *dest, const char *src, int maxlen );
    
    bool formpath( 
      char *dest, 
      const char *path, 
      const char *file, 
      const char *extension, 
      int maxlen 
      );
    
    
    inline bool strncat( char *dest, const char *src, int maxlen ) 
    {
      if( !dest || !src )
      {
        null_pointer("strncat");
        return false;	
      }	
      
      int len = strlen(dest);
      int srclen = strlen(src);
      
      if( len+srclen>=maxlen-1 )
      {
        return false;
      }
      
      strcat( dest, src );
      return true;
    } 
    
    
    inline bool strncat( char *dest, char c, int maxlen ) 
    {
      if( !dest )
      {
        null_pointer("strncat");
        return false;
      }
      
      int len = strlen( dest );
      if( len+1 >= maxlen-1 )
      {
        return false;
      }
      
      dest[len] = c;
      dest[len+1] = '\0';
      
      return true;	
    } 
    
    
    inline bool strncpy( char *dest, const char *src, int maxlen ) 
    {
      if( !dest || !src )
      {
        null_pointer("strncpy");
        return false;	
      }	
      
      int len=0;
      
      while( *src )
      {	
        if( len>=maxlen-1 )
        {
          *dest='\0';
          return false;
        }		
        
        *dest++ = *src++;
        len++;
      }
      *dest = '\0';
      
      return true;
    } 
    
    inline char * strupr( char *str ) 
    {
      if( !str )
      {
        null_pointer("strupr");
        return 0;	
      }	
      
      char *p = str;
      while( *p )
      {	
        *p = toupper( *p );
        ++p;
      }	
      return str;
    } 
    
    inline char * strlower( char *str ) 
    {
      if( !str )
      {
        null_pointer("strlower");
        return 0;
      }	
      
      char *p=str;
      
      while( *p )
      {
        *p = tolower( *p );
        ++p;
      }
      return str;
    } 
    
    inline int strcmp( const char *str1, const char *str2 ) 
    {
      if( !str1 || !str2 )
      {
        null_pointer("strcmp");
        return 0;	
      }	
      
      do
      {
        char c1=*str1;
        char c2=*str2;
        
        if( c1<c2 )
        {
          return -1;
        }
        if( c1>c2 )
        {
          return 1;
        }
        
        if( c1==0 || c2==0 )
        {
          break;
        }
        
        
        str1++;
        str2++;
      } while( 1 );
      
      return 0;
    } 
    
    inline int strncmp( const char *str1, const char *str2, int maxlen ) 
    {
      if( !str1 || !str2 )
      {
        null_pointer("strncmp");
        return 0;	
      }	
      
      int len=0;
      
      do
      {
        char c1=*str1;
        char c2=*str2;
        
        
        if( c1<c2 )
        {
          return -1;
        }
        if( c1>c2 )
        {
          return 1;
        }
        
        if( c1==0 || c2==0 )
        {
          break;
        }
        
        str1++;
        str2++;
        len++;
        
      } while( len<maxlen );
      
      return 0;
    } 
    
    inline int stricmp( const char *str1, const char *str2 ) 
    {
      if( !str1 || !str2 )
      {
        null_pointer("stricmp");
        return 0;
      }	
      
      do
      {
        char c1=*str1;
        char c2=*str2;
        
        if( c1>='A' && c1<='Z' )
        {
          c1 += ('a'-'A');
        }
        if( c2>='A' && c2<='Z' )
        {
          c2 += ('a'-'A');
        }
        
        if( c1<c2 )
        {
          return -1;
        }
        if( c1>c2 )
        {
          return 1;
        }
        
        if( c1==0 || c2==0 )
        {
          break;
        }
        
        
        str1++;
        str2++;
      } while( 1 );
      
      return 0;
    }
    
    inline int strnicmp( const char *str1, const char *str2, int maxlen ) 
    {
      if( !str1 || !str2 )
      {
        null_pointer("strnicmp");
        return 0;	
      }	
      
      int len=0;
      
      do
      {
        char c1=*str1;
        char c2=*str2;
        
        if( c1>='A' && c1<='Z' )
        {
          c1 += ('a'-'A');
        }
        if( c2>='A' && c2<='Z' )
        {
          c2 += ('a'-'A');
        }
        
        if( c1<c2 )
        {
          return -1;
        }
        if( c1>c2 )
        {
          return 1;
        }
        
        if( c1==0 || c2==0 )
        {
          break;
        }
        
        str1++;
        str2++;
        len++;
        
      } while( len<maxlen );
      
      return 0;
    }
    
    inline char * strip_crlf( char *buf ) 
    {
      if( !buf )
      {
        null_pointer("strip_crlf");
        return 0;	
      }
      
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
    
    inline char * strip_endws( char *buf ) 
    {
      if( !buf )
      {
        null_pointer("strip_endws");
        return 0;	
      }
      
      for( int i=strlen(buf)-1; i>=0; --i )	
      {
        char c = buf[i];
        
        if( c=='\r' || c=='\n' || c==' ' || c=='\t' )
        {
          buf[i]='\0';
        }		
        else
        {
          break;
        }
        
      }
      
      return buf;
    } 
    
    inline char * strip_begws( char *buf ) 
    {	
      if( !buf )
      {
        null_pointer("strip_begws");
        return 0;	
      }
      
      int skip = strspn(buf,"\r\n \t" );
      
      if( skip!=0 )
      {
        int len = strlen(buf) - skip;
        
        for( int i=0; i<=len; ++i )
        {
          buf[i] = buf[i+skip];
        }
      }
      return buf;
    } 
    
    inline char * parsequoted( char *dest, const char *src, int maxlen ) 
    {
      if( !dest || !src )
      {
        null_pointer("parsequoted");
        return 0;	
      }
      
      char *d = dest;
      const char *first_quote=0;
      int curlen=0;
      
      while( *src )
      {
        if( *src == '"' )
        {
          first_quote = src;
          break;
        }
        ++src;
      }
      
      if( first_quote )
      {
        // found first quote.
        // skip over it
        ++src;
        
        // now copy things over to dest
        
        while( *src )
        {
          if( *src=='"' )
          {
            *d++ = 0;
            
            // we completed the string!
            return d;
          }
          
          *d++ = *src++;
          
          curlen++;
          
          if( curlen >= maxlen )
          {
            // too long!
            break;
          }
          
        }
        
      }
      
      // either too long or not matching quotes or no quote.
      
      return 0;	
      
    }
    
    inline void sleep( int seconds )
    {
#if defined(IF2K_CONFIG_WIN32)
      Sleep( seconds*1000 );
#else
      sleep( seconds );
#endif
    }
    
    inline void sleepms( int milliseconds )
    {
#if defined(IF2K_CONFIG_WIN32)
      Sleep( milliseconds );
#else
      usleep( milliseconds*1000 );
#endif
      
    }
    
    inline const char *find_extension( const char *path )
    {
      int pos=strlen(path);
      while( --pos>=0 )
      {
        if( path[pos]=='.' )
          return &path[pos];
        if( path[pos]=='/' || path[pos]=='\\' )
          return 0;
      }
      return 0;
    }
    
    inline unsigned long get_time_seconds()
    {
      return (unsigned long)time(0);
    }
    
    template <int MAX_SIZE>
    inline bool read_line( string_t &result, FILE *f )
    {
      bool r=false;
      char tmpbuf[MAX_SIZE+1];
      tmpbuf[MAX_SIZE]='\0';
      
      if( fgets( tmpbuf, MAX_SIZE, f ) )
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
    
#if defined(IF2K_CONFIG_POSIX)
#define IF2K_PATHSEP '/'
#else
#define IF2K_PATHSEP '\\'
#endif
    
    inline filename_t &fix_directory_name(
      filename_t &d 
      )
    {
      // if path does not end in '/' or '\\' then append appropriate path sep char      
      int len = d.length();
      if( len >0 && d[len-1]!='\\' && d[len-1]!='/'  )
      {
        d += IF2K_PATHSEP;
      }      
      return d;
    }
    
    inline filename_t append_path( 
      const filename_t &base_path, 
      const filename_t &file,
      const char path_sep = IF2K_PATHSEP
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
