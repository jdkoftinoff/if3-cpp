/*

The IF Contextual Media Group Kernel Version 3
Source Code

Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
Copyright (c) 1995-2005
By Contextual Media Group, Inc.
http://www.contextualmediagroup.com/

ALL RIGHTS RESERVED.

 */
#ifndef if3_KERNEL_HPP
#define if3_KERNEL_HPP

#include "if3_world.hpp"
#include "if3_string.hpp"
#include "if3_buf.hpp"
#include "if3_util.hpp"
#include "if3_scanner.hpp"
#include "if3_compile.hpp"

namespace if3
{
  namespace kernel
  {

    enum
    {
      MAX_CATEGORIES = 64,
      MAX_MATCH_LEN = 2048
    };

    typedef int category_t;

    class categories_enable_t
    {
    public:

      categories_enable_t()
      :
      m_enabled(MAX_CATEGORIES)
      {
        for (category_t i = 0; i < MAX_CATEGORIES; ++i)
        {
          m_enabled[i] = true;
        }
      }

      categories_enable_t(string_t cats)
      :
      m_enabled(MAX_CATEGORIES)
      {
        for (category_t i = 0; i < MAX_CATEGORIES; ++i)
        {
          m_enabled[i] = false;
        }
        set_from_string(cats);
      }

      categories_enable_t(const categories_enable_t &other)
      : m_enabled(other.m_enabled)
      {
      }

      const categories_enable_t & operator =(const categories_enable_t &other)
      {
        m_enabled = other.m_enabled;
        return *this;
      }

      void
      clear()
      {
        for (size_t i = 0; i < m_enabled.size(); ++i)
        {
          m_enabled[i] = false;
        }
      }

      void
      set_from_string(string_t cats)
      {
        std::vector<string_t> items = util::split(cats, ',');
        for (std::vector<string_t>::iterator i = items.begin(); i != items.end(); i++)
        {
          int value = strtol(i->c_str(), 0, 10);
          if (value > 0 && value <= MAX_CATEGORIES)
          {
            m_enabled[value - 1] = true;
          }
        }
      }

      bool
      is_enabled(category_t c) const
      {
        return m_enabled[c];
      }

      friend inline
      std::ostream & operator <<(std::ostream &o, const categories_enable_t &v)
      {
        bool first = true;
        for (size_t i = 0; i < v.m_enabled.size(); ++i)
        {
          if (first)
            first = false;
          else
            o << ',';

          if (v.m_enabled[i])
          {
            o << i;
          }
        }

        return o;
      }


    private:
      std::vector<bool> m_enabled;
    };

    class match_and_category_t
    {
    public:

      match_and_category_t() : m_match(), m_category(0)
      {
      }

      match_and_category_t(string_t match, category_t category)
      :
      m_match(match),
      m_category(category)
      {
      }

      const match_and_category_t & operator =(const match_and_category_t &other)
      {
        m_match = other.m_match;
        m_category = other.m_category;
        return *this;
      }

      const string_t &
      match() const
      {
        return m_match;
      }

      category_t
      category() const
      {
        return m_category;
      }

      friend inline std::ostream & operator <<(std::ostream &o, const match_and_category_t &v)
      {
        o << "\t" << v.category() << "\t" << v.match();
        return o;
      }

    private:
      string_t m_match;
      category_t m_category;
    };

    typedef std::list< match_and_category_t > match_container_t;

    inline std::ostream &
    operator <<(
                std::ostream &o,
                const match_container_t &v
                )
    {
      for (match_container_t::const_iterator i = v.begin(); i != v.end(); ++i)
      {
        o << "\t" << *i << "\n";
      }
      return o;
    }

    class match_histogram_t
    {
    public:

      typedef std::valarray< float > container_type;

      match_histogram_t() : m_counts(MAX_CATEGORIES)
      {
        for( int i=0; i<MAX_CATEGORIES; ++i )
        {
          m_counts[i] = 0.0f;
        }
      }

      match_histogram_t(const match_histogram_t &other) : m_counts( other.m_counts )
      {
      }

      float
      count_for_category(category_t cat) const
      {
        if (cat >= 0 && cat < MAX_CATEGORIES)
          return m_counts[cat];
        else
          throw std::range_error("invalid category for histogram");
      }

      void
      add_category(category_t cat)
      {
        if (cat >= 0 && cat < MAX_CATEGORIES)
          m_counts[cat] = m_counts[cat] + 1.0f;
        else
          throw std::range_error("invalid category for histogram");
      }

      const match_histogram_t & operator =(const match_histogram_t &other)
      {
        m_counts = other.m_counts;
        return *this;
      }

      const container_type &
      counts() const
      {
        return m_counts;
      }

      void
      counts(const container_type &v)
      {
        m_counts = v;
      }

      friend inline
      std::ostream & operator <<(std::ostream &o, const match_histogram_t &v)
      {
        for (category_t i = 0; i < MAX_CATEGORIES; ++i)
        {
          float c = v.count_for_category(i);
          o << "\t" << i + 1 << "\t" << c << "\n";
        }
        return o;
      }
    private:
      container_type m_counts;
    };

    class match_results_t
    {
    public:

      match_results_t()
      :
      m_total_match_count(0),
      m_histogram(),
      m_matches(),
      m_largest_category(0)
      {
      }

      match_results_t(const match_results_t &o)
      :
      m_total_match_count(o.m_total_match_count),
      m_histogram(o.m_histogram),
      m_matches(o.m_matches),
      m_largest_category(o.m_largest_category)
      {
      }

      const match_results_t & operator =(const match_results_t &o)
      {
        m_total_match_count = o.m_total_match_count;
        m_histogram = o.m_histogram;
        m_matches = o.m_matches;
        m_largest_category = o.m_largest_category;
        return *this;
      }

      unsigned int
      total_match_count() const
      {
        return m_total_match_count;
      }

      const match_histogram_t &
      histogram() const
      {
        return m_histogram;
      }

      const match_container_t &
      matches() const
      {
        return m_matches;
      }

      category_t
      largest_category() const
      {
        return m_largest_category;
      }

      float
      largest_category_score() const
      {
        return m_histogram.count_for_category(m_largest_category);
      }

      void
      add_match(category_t category, const char *match_text)
      {
        m_total_match_count++;
        m_histogram.add_category(category);
        if (m_histogram.count_for_category(category) >
            m_histogram.count_for_category(m_largest_category))
        {
          m_largest_category = category;
        }

        if (match_text)
        {
          m_matches.push_back(match_and_category_t(string_t(match_text), category));
        }

      }

      friend inline std::ostream & operator <<(std::ostream &o, const match_results_t &v)
      {
        o << "total match count: " << v.total_match_count() << "\n";
        o << "histogram: \n" << v.histogram() << "\n";
        o << "matches: \n" << v.matches() << "\n";
        o << "largest category: " << v.largest_category() << "\n";
        o << "largest category score: " << v.largest_category_score() << "\n";
        return o;
      }

    private:
      unsigned int m_total_match_count;
      match_histogram_t m_histogram;
      match_container_t m_matches;
      category_t m_largest_category;
    };

    template <class T>
    class match_event_t : public tree_event_t<T>
    {
    public:
      typedef T tree_traits_type;
      typedef treebase_t<tree_traits_type> tree_type;
      typedef typename tree_traits_type::index_t match_item_type;
      typedef typename tree_traits_type::flags_t flags_type;

      match_event_t(
                    match_results_t &result,
                    categories_enable_t &enabled_categories,
                    bool enable_proof
                    )
      :
      m_result(result),
      m_enabled_categories(enabled_categories),
      m_enable_proof(enable_proof)
      {
      }

      match_event_t(const match_event_t &o)
      :
      m_result(o.m_result),
      m_enabled_categories(o.m_enabled_categories),
      m_enable_proof(o.m_enable_proof)
      {
      }

      virtual void operator () (
                                const tree_type &tree,
                                match_item_type match_item
                                )
      {
        flags_type flags = tree.get(match_item).flags;

        if (flags >= 0)
        {
          category_t current_category = category_t(flags);

          if (m_enabled_categories.is_enabled(current_category))
          {
            if (m_enable_proof)
            {
              char match_text[MAX_MATCH_LEN] = "";
              tree.extract(match_text, MAX_MATCH_LEN, match_item);
              match_text[MAX_MATCH_LEN - 1] = '\0';

              m_result.add_match(current_category, match_text);
            }
            else
            {
              m_result.add_match(current_category, 0);
            }
          }
        }
      }

    private:
      match_results_t &m_result;
      categories_enable_t &m_enabled_categories;
      bool m_enable_proof;
    };

    class full_scan_results_t
    {
    public:

      full_scan_results_t()
      : m_hostname(),
      m_link(),
      m_text(),
      m_links(),
      m_content_links(),
      m_final_category(-1),
      m_final_category_score(0.0f)
      {
      }

      full_scan_results_t(const full_scan_results_t &other)
      :
      m_hostname(other.m_hostname),
      m_link(other.m_link),
      m_text(other.m_text),
      m_links(other.m_links),
      m_content_links(other.m_content_links),
      m_final_category(other.m_final_category),
      m_final_category_score(other.m_final_category_score)
      {
      }

      const full_scan_results_t & operator =(const full_scan_results_t &other)
      {
        m_hostname = other.m_hostname;
        m_link = other.m_link;
        m_text = other.m_text;
        m_links = other.m_links;
        m_content_links = other.m_content_links;
        m_final_category = other.m_final_category;
        m_final_category_score = other.m_final_category_score;

        return *this;
      }

      match_results_t &
      hostname()
      {
        return m_hostname;
      }

      const match_results_t &
      hostname() const
      {
        return m_hostname;
      }

      match_results_t &
      link()
      {
        return m_link;
      }

      const match_results_t &
      link() const
      {
        return m_link;
      }

      match_results_t &
      text()
      {
        return m_text;
      }

      const match_results_t &
      text() const
      {
        return m_text;
      }

      match_results_t &
      links()
      {
        return m_links;
      }

      const match_results_t &
      links() const
      {
        return m_links;
      }

      match_results_t &
      content_links()
      {
        return m_content_links;
      }

      const match_results_t &
      content_links() const
      {
        return m_content_links;
      }

      category_t
      final_category() const
      {
        return m_final_category;
      }

      float
      final_category_score() const
      {
        return m_final_category_score;
      }

      category_t
      calculate_final_category()
      {
        m_final_category = m_hostname.largest_category();
        m_final_category_score = m_hostname.largest_category_score();

        if (m_link.largest_category_score() > m_final_category_score)
        {
          m_final_category = m_link.largest_category();
          m_final_category_score = m_link.largest_category_score();
        }

        if (m_text.largest_category_score() > m_final_category_score)
        {
          m_final_category = m_text.largest_category();
          m_final_category_score = m_text.largest_category_score();
        }

        if (m_links.largest_category_score() > m_final_category_score)
        {
          m_final_category = m_links.largest_category();
          m_final_category_score = m_links.largest_category_score();
        }

        if (m_content_links.largest_category_score() > m_final_category_score)
        {
          m_final_category = m_content_links.largest_category();
          m_final_category_score = m_content_links.largest_category_score();
        }
        return m_final_category;
      }

      friend inline
      std::ostream & operator <<(std::ostream &o, const full_scan_results_t &v)
      {
        o << "hostname:\n" << v.hostname() << "\n";
        o << "link:\n" << v.link() << "\n";
        o << "text:\n" << v.text() << "\n";
        o << "links:\n" << v.links() << "\n";
        o << "content_links:\n" << v.content_links() << "\n";
        o << "final category:\n" << v.final_category() << "\n";
        o << "final category score:\n" << v.final_category_score() << "\n";

        return o;
      }

    private:
      match_results_t m_hostname;
      match_results_t m_link;
      match_results_t m_text;
      match_results_t m_links;
      match_results_t m_content_links;
      category_t m_final_category;
      float m_final_category_score;
    };

    class full_scan_request_t
    {
    public:

      full_scan_request_t()
      :
      m_hostname(0),
      m_hostname_len(0),
      m_link(0),
      m_link_len(0),
      m_text(0),
      m_text_len(0),
      m_links(0),
      m_links_len(0),
      m_content_links(0),
      m_content_links_len(0)
      {
      }

      full_scan_request_t(const full_scan_request_t &other)
      :
      m_hostname(other.m_hostname),
      m_hostname_len(other.m_hostname_len),
      m_link(other.m_link),
      m_link_len(other.m_link_len),
      m_text(other.m_text),
      m_text_len(other.m_text_len),
      m_links(other.m_links),
      m_links_len(other.m_links_len),
      m_content_links(other.m_content_links),
      m_content_links_len(other.m_content_links_len)
      {
      }

      const full_scan_request_t & operator =(const full_scan_request_t &other)
      {
        m_hostname = other.m_hostname;
        m_hostname_len = other.m_hostname_len;
        m_link = other.m_link;
        m_link_len = other.m_link_len;
        m_text = other.m_text;
        m_text_len = other.m_text_len;
        m_links = other.m_links;
        m_links_len = other.m_links_len;
        m_content_links = other.m_content_links;
        m_content_links_len = other.m_content_links_len;
        return *this;
      }

      void
      hostname(const char *v, size_t len)
      {
        m_hostname = v;
        m_hostname_len = len;
      }

      const char *
      hostname() const
      {
        return m_hostname;
      }

      size_t
      hostname_len() const
      {
        return m_hostname_len;
      }

      void
      link(const char *v, size_t len)
      {
        m_link = v;
        m_link_len = len;
      }

      const char *
      link() const
      {
        return m_link;
      }

      size_t
      link_len() const
      {
        return m_link_len;
      }

      void
      text(const char *v, size_t len)
      {
        m_text = v;
        m_text_len = len;
      }

      const char *
      text() const
      {
        return m_text;
      }

      size_t
      text_len() const
      {
        return m_text_len;
      }

      void
      links(const char *v, size_t len)
      {
        m_links = v;
        m_links_len = len;
      }

      const char *
      links() const
      {
        return m_links;
      }

      size_t
      links_len() const
      {
        return m_links_len;
      }

      void
      content_links(const char *v, size_t len)
      {
        m_content_links = v;
        m_content_links_len = len;
      }

      const char *
      content_links() const
      {
        return m_content_links;
      }

      size_t
      content_links_len() const
      {
        return m_content_links_len;
      }

      friend inline
      std::ostream & operator <<(std::ostream &o, const full_scan_request_t &v)
      {
        o << "hostname: " << string_t(v.hostname(), v.hostname_len()) << "\n";
        o << "link: " << string_t(v.link(), v.link_len()) << "\n";
        o << "text:\n" << string_t(v.text(), v.text_len()) << "\n";
        o << "links:\n" << string_t(v.links(), v.links_len()) << "\n";
        o << "content_links:\n" << string_t(v.content_links(), v.content_links_len()) << "\n";
        return o;
      }

    private:
      const char *m_hostname;
      size_t m_hostname_len;

      const char *m_link;
      size_t m_link_len;

      const char *m_text;
      size_t m_text_len;

      const char *m_links;
      size_t m_links_len;

      const char *m_content_links;
      size_t m_content_links_len;
    };

    class kernel_t
    {
    public:

      kernel_t(
               filename_t compiled_hostname_filename,
               filename_t compiled_url_filename,
               filename_t compiled_alphanumeric_filename
               )
      :
      m_hostname_scanner(compiled_hostname_filename),
      m_url_scanner(compiled_url_filename),
      m_alphanumeric_scanner(compiled_alphanumeric_filename)
      {
      }

      ~kernel_t()
      {
      }

      int
      perform_scan(
                   full_scan_results_t &results,
                   full_scan_request_t &request,
                   categories_enable_t &enabled_categories,
                   bool enable_proof
                   )
      {
        {
          match_event_t<tree_traits_url_mmap_t> url_event(results.hostname(), enabled_categories, enable_proof);
          m_hostname_scanner.find(request.hostname(), request.hostname_len(), url_event);
        }

        {
          match_event_t<tree_traits_url_mmap_t> url_event(results.link(), enabled_categories, enable_proof);
          m_url_scanner.find(request.link(), request.link_len(), url_event);

          match_event_t<tree_traits_alphanumeric_mmap_t> alphanumeric_event(results.link(), enabled_categories, enable_proof);
          m_alphanumeric_scanner.find(request.link(), request.link_len(), alphanumeric_event);

        }

        {
          match_event_t<tree_traits_url_mmap_t> url_event(results.link(), enabled_categories, enable_proof);
          m_url_scanner.find(request.link(), request.link_len(), url_event);

          match_event_t<tree_traits_alphanumeric_mmap_t> alphanumeric_event(results.link(), enabled_categories, enable_proof);
          m_alphanumeric_scanner.find(request.link(), request.link_len(), alphanumeric_event);

        }

        {
          match_event_t<tree_traits_url_mmap_t> url_event(results.text(), enabled_categories, enable_proof);
          m_url_scanner.find(request.text(), request.text_len(), url_event);

          match_event_t<tree_traits_alphanumeric_mmap_t> alphanumeric_event(results.text(), enabled_categories, enable_proof);
          m_alphanumeric_scanner.find(request.text(), request.text_len(), alphanumeric_event);

        }

        {
          match_event_t<tree_traits_url_mmap_t> url_event(results.links(), enabled_categories, enable_proof);
          m_url_scanner.find(request.links(), request.links_len(), url_event);

          match_event_t<tree_traits_alphanumeric_mmap_t> alphanumeric_event(results.links(), enabled_categories, enable_proof);
          m_alphanumeric_scanner.find(request.links(), request.links_len(), alphanumeric_event);

        }

        {
          match_event_t<tree_traits_url_mmap_t> url_event(results.content_links(), enabled_categories, enable_proof);
          m_url_scanner.find(request.content_links(), request.content_links_len(), url_event);

          match_event_t<tree_traits_alphanumeric_mmap_t> alphanumeric_event(results.content_links(), enabled_categories, enable_proof);
          m_alphanumeric_scanner.find(request.content_links(), request.content_links_len(), alphanumeric_event);

        }

        return results.calculate_final_category();
      }

    private:
      url_scanner_precompiled_t m_hostname_scanner;
      url_scanner_precompiled_t m_url_scanner;
      alphanumeric_scanner_precompiled_t m_alphanumeric_scanner;

    };
  }
}
#endif
