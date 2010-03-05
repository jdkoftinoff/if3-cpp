#include "if3_world_precompiled.hpp"
#include "if3_http.hpp"

namespace if3
{
  namespace http
  {
    bool flatten_item(buf_t &outbuf, key_t const &key, value_t const &val)
    {
      if (!outbuf.append_from_string(key))
        return false;
      if (!outbuf.append_from_string(": "))
        return false;
      if (!outbuf.append_from_string(val))
        return false;
      if (!outbuf.append_from_string("\r\n"))
        return false;
      return true;
    }

    bool flatten_items(buf_t &outbuf, items_t const &items)
    {
      for (items_t::const_iterator i = items.begin(); i != items.end(); i++)
      {
        if (!flatten_item(outbuf, i->first, i->second))
          return false;
      }
      if (!outbuf.append_from_string("\r\n\r\n"))
        return false;
      return true;
    }

    bool request_t::parse(const buf_t &inbuf, int start_pos, int *end_pos)
    {
      int cur_pos = start_pos;

      if (!parse_request(inbuf, cur_pos, &cur_pos))
        return false;

      if (!parse_items(inbuf, cur_pos, &cur_pos))
        return false;

      if (end_pos)
        *end_pos = cur_pos;
      return true;
    }

    bool request_t::parse_request(const buf_t &inbuf, int start_pos, int *end_pos)
    {
      int cur_pos = start_pos;
      string_t line;
      string_t tmp_line;
      const string_t delimiters = "\t ";

      clear();

      cur_pos = inbuf.extract_to_string(line, cur_pos, 8192);
      if (cur_pos < 0)
        return false;

      //std::cout << "got line: '" << line << "'\n";

      string_t::size_type line_pos1 = line.find_first_of(delimiters, 0);
      if (line_pos1 == string_t::npos)
        return false;

      m_type = line.substr(0, line_pos1);
      //std::cout << "request_type: '" << m_request_type << "'\n";

      line_pos1 = line.find_first_not_of(delimiters, line_pos1 + 1);
      if (line_pos1 == string_t::npos)
        return false;

      string_t::size_type line_pos2 = line.find_first_of(delimiters, line_pos1);
      if (line_pos2 == string_t::npos)
        return false;

      m_url = line.substr(line_pos1, line_pos2 - (line_pos1));

      //std::cout << "request_url: '" << m_request_url << "'\n";

      line_pos2 = line.find_first_not_of(delimiters, line_pos2 + 1);
      if (line_pos2 == string_t::npos)
        return false;

      string_t::size_type line_pos3 = line.find_last_not_of(delimiters);
      if (line_pos3 == string_t::npos)
        return false;

      m_http_version = line.substr(line_pos2, line_pos3 - line_pos2 + 1);
      //std::cout << "version: '" << m_version << "'\n";

      if (end_pos)
        *end_pos = cur_pos;
      return true;
    }

    bool request_t::parse_item(const request_t::multiline_item_t &item)
    {
      multiline_item_t::const_iterator i = item.begin();
      key_t key;
      value_t value;

      // first line is special, it always has the key

      // make sure we have a line to parse
      if (i == item.end())
        return false;

      {
        // key is all text up to first ':'
        string_t::size_type first_colon = i->find_first_of(':');
        if (first_colon == string_t::npos)
          return false;

        // extract key
        key = key_tolower(i->substr(0, first_colon));
        if (key.empty())
          return false;

        // extract value
        string_t::size_type next_non_space = i->find_first_not_of("\t ", first_colon + 1);
        if (next_non_space == string_t::npos)
          return false;
        value = i->substr(next_non_space);

        // do we know about this item already?
        items_t::iterator existing_key = m_items.find(key);
        if (existing_key != m_items.end())
        {
          // yes!
          // prepend existing value to new value and remove original partial entry
          value = existing_key->second + ", " + value;
          m_items.erase(existing_key);
        }
      }

      // now also append any additional multiline values
      for (++i; i != item.end(); ++i)
      {
        if (i->empty())
          return false;

        string_t::size_type next_non_space = i->find_first_not_of("\t ");
        if (next_non_space == string_t::npos)
          return false;
        value = value + ", " + i->substr(next_non_space);
      }

      m_items.insert(std::make_pair(key, value));

      // success
      return true;
    }

    bool request_t::parse_items(const buf_t &inbuf, int start_pos, int *end_pos)
    {
      int cur_pos = start_pos;
      bool r = false;
      const string_t lws = "\t ";

      string_t line;
      bool done = false;

      multiline_item_t multiline;
      do
      {
        cur_pos = inbuf.extract_to_string(line, cur_pos, 8192);
        if (cur_pos < 0)
        {
          // end of buffer before completion, error parsing.
          break;
        }

        // blank line means end of header
        if (line.empty())
        {
          // were we parsing a multiline request?
          if (!multiline.empty())
          {
            // yes, handle it now
            r = parse_item(multiline);
            multiline.clear();
          }
          else
          {
            // we are done with success
            r = true;
          }
        }
        else
        {
          // does the line have blank at beginning?

          if (line[0] == ' ' || line[0] == '\t')
          {
            // yes, are we in multiline mode?
            if (!multiline.empty())
            {
              // yes, so append this line to our multiline
              multiline.push_back(line);
            }
            else
            {
              // this is error, we got indented line without key
              r = false;
              break;
            }
          }
          else
          {
            // the line starts with text, this finishes any multiline and starts a new one
            if (!multiline.empty())
            {
              // yes, handle it now
              r = parse_item(multiline);
              multiline.clear();
              // error parsing multiline?
              if (!r)
                break;
            }

            // new multiline started it must have a ':' in it after a field
            if (line.find_first_of(':') > 0)
            {
              // clear multiline
              multiline.clear();

              multiline.push_back(line);
            }
          }
        }
      } while (!done);

      if (end_pos)
      {
        *end_pos = cur_pos;
      }
      return r;
    }

    bool request_t::flatten(buf_t &outbuf)
    {
      if (!outbuf.append_from_string(m_type))
        return false;
      if (!outbuf.append_from_string(" "))
        return false;
      if (!outbuf.append_from_string(m_url))
        return false;
      if (!outbuf.append_from_string(" "))
        return false;
      if (!outbuf.append_from_string(m_http_version))
        return false;
      if (!outbuf.append_from_string("\r\n"))
        return false;

      if (!flatten_items(outbuf, m_items))
        return false;
      return true;
    }

  }

}
