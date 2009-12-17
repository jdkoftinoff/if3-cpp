/*
  
The Internet Filter Version 3 Kernel Version 3
Source Code

Written By Jeff Koftinoff <jeffk@internetfilter.com>
Copyright (c) 1995-2005
By Turner and Sons Productions, Inc.
http://www.internetfilter.com/

ALL RIGHTS RESERVED.

 */

#ifndef IF3_UTIL_HPP
#define IF3_UTIL_HPP

#include "if3_world.hpp"
#include "if3_string.hpp"


namespace if3
{
  namespace util
  {

    template <typename T1, typename T2>
    inline T1 lexical_cast( const T2 v )
    {
      std::stringstream ss;
      ss << v;
      T1 r;
      ss >> r;
      return r;
    }

    bool formpath(
                  string_t &dest,
                  string_t const &path,
                  string_t const &file,
                  string_t const &extension,
                  int maxlen
                  );

    inline void
    sleep(int seconds)
    {
#if defined(IF3_CONFIG_WIN32)
      Sleep(seconds * 1000);
#else
      sleep(seconds);
#endif
    }

    inline void
    sleepms(int milliseconds)
    {
#if defined(IF3_CONFIG_WIN32)
      Sleep(milliseconds);
#else
      usleep(milliseconds * 1000);
#endif

    }

    inline const char *
    find_extension(const char *path)
    {
      int pos = std::strlen(path);
      while (--pos >= 0)
      {
        if (path[pos] == '.')
          return &path[pos];
        if (path[pos] == '/' || path[pos] == '\\')
          return 0;
      }
      return 0;
    }

#if defined(IF3_CONFIG_POSIX)
#define IF3_PATHSEP '/'
#else
#define IF3_PATHSEP '\\'
#endif

    inline filename_t &
    fix_directory_name(
                       filename_t &d
                       )
    {
      // if path does not end in '/' or '\\' then append appropriate path sep char      
      int len = d.length();
      if (len > 0 && d[len - 1] != '\\' && d[len - 1] != '/')
      {
        d += IF3_PATHSEP;
      }
      return d;
    }

    inline char *
    strip_crlf(char *buf)
    {
      char *p = buf;

      while (*p)
      {
        if (*p == '\r' || *p == '\n')
        {
          *p = '\0';
          break;
        }
        ++p;
      }

      return buf;
    }

    inline bool
    read_line(
              string_t &result,
              FILE *f
              )
    {
      bool r = false;
      char tmpbuf[4096 + 1];
      tmpbuf[4096] = '\0';

      if (fgets(tmpbuf, 4096, f))
      {
        strip_crlf(tmpbuf);
        result = tmpbuf;
        r = true;
      }
      else
      {
        result = "";
      }
      return r;
    }

    inline filename_t
    append_path(
                const filename_t &base_path,
                const filename_t &file,
                const char path_sep = IF3_PATHSEP
                )
    {
      filename_t result(base_path);
      fix_directory_name(
                         result
                         );
      result += file;

      return result;
    }

    inline bool
    file_exists(
                const filename_t &fname
                )
    {
      FILE *f = fopen(fname.c_str(), "rb");
      if (f)
      {
        fclose(f);
      }
      return f != 0;
    }

    inline
    std::vector<string_t> &
    split(
          const string_t &s,
          char delim,
          std::vector<string_t> &elems)
    {
      std::stringstream ss(s);
      string_t item;
      while (std::getline(ss, item, delim))
      {
        elems.push_back(item);
      }
      return elems;
    }

    inline
    std::vector<string_t>
    split(const string_t &s, char delim)
    {
      std::vector<string_t> elems;
      return split(s, delim, elems);
    }


  }
}

#endif
