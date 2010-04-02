#ifndef IF3_NET_HPP
#define IF3_NET_HPP

#include "if3_world.hpp"
#include "if3_string.hpp"
#include "if3_util.hpp"
#include "if3_daemon.hpp"
#include "if3_buf.hpp"

namespace if3
{
  namespace net
  {
#ifdef _WIN32
    typedef SOCKET socket_handle_t;
#else
    typedef int socket_handle_t;
#endif

    void net_init();

    inline string_t to_string(const ::sockaddr *a, size_t a_len)
    {
#if 0
      char buf1[1024] = "";
      char buf1_serv[128] = "";
      ::getnameinfo ( a, a_len, buf1, sizeof(buf1)-1, buf1_serv, sizeof(buf1_serv)-1, 0 );

      buf1[1023] = '\0';
      buf1_serv[127] = '\0';

      char buf2[128] = "";
      char buf2_serv[128] = "";
      ::getnameinfo( a, a_len, buf2, sizeof(buf2)-1, buf2_serv, sizeof(buf2_serv)-1, NI_NUMERICHOST|NI_NUMERICSERV);

      buf2[127] = '\0';
      buf2_serv[127] = '\0';

      string_t result =
      string_t("[") + string_t(buf1) + "]:" + buf1_serv
      + " ( [" + string_t(buf2) + "]:" + buf2_serv + " )";
#else
      char buf2[128] = "";
      char buf2_serv[128] = "";
      ::getnameinfo(a, a_len, buf2, sizeof(buf2) - 1, buf2_serv, sizeof(buf2_serv) - 1, NI_NUMERICHOST | NI_NUMERICSERV);

      buf2[127] = '\0';
      buf2_serv[127] = '\0';

      string_t result = string_t("[") + string_t(buf2) + "]:" + buf2_serv;

#endif
      return result;
    }

    inline string_t to_string(const ::sockaddr_storage *a, size_t a_len)
    {
      return to_string((const ::sockaddr *) a, a_len);
    }

    inline string_t to_string(const ::addrinfo &a)
    {
      return to_string(a.ai_addr, a.ai_addrlen);
    }

    inline std::ostream & operator <<(std::ostream& o, const ::addrinfo &v)
    {
      o << to_string(v);
      return o;
    }

    class net_address_t
    {
    public:
      net_address_t(const char *hostname, const char *port)
      {
        ::addrinfo hints;
        ::memset(&hints, '\0', sizeof(hints));
        hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG; // | AI_NUMERICSERV;
        hints.ai_socktype = SOCK_STREAM;
        int e = ::getaddrinfo(hostname, port, &hints, &m_ai);
        if (e != 0)
        {
          throw std::runtime_error("getaddrinfo: " + string_t(strerror(e)));
        }
      }

      class const_iterator
      {
      public:
        const_iterator(const const_iterator &o) :
          m_v(o.m_v)
        {
        }

        const_iterator(::addrinfo *v) :
          m_v(v)
        {
        }

        const ::addrinfo & operator *() const
        {
          return *m_v;
        }

        const ::addrinfo * operator ->() const
        {
          return m_v;
        }

        const_iterator & operator ++()
        {
          if (m_v != 0)
            m_v = m_v->ai_next;
          return *this;
        }

        const_iterator operator ++(int)
        {
          const_iterator prev(*this);
          if (m_v != 0)
            m_v = m_v->ai_next;
          return prev;
        }

        bool operator ==(const const_iterator &o) const
        {
          return m_v != o.m_v;
        }

        bool operator !=(const const_iterator &o) const
        {
          return m_v != o.m_v;
        }

      private:
        ::addrinfo *m_v;
      };

      const_iterator begin() const
      {
        return const_iterator(m_ai);
      }
      const_iterator end() const
      {
        return const_iterator(0);
      }

      ~net_address_t()
      {
        ::freeaddrinfo(m_ai);
      }

      inline friend std::ostream & operator <<(std::ostream& o, const net_address_t &v)
      {
        o << "{\n";
        for (net_address_t::const_iterator i = v.begin(); i != v.end(); ++i)
        {
          o << *i << "\n";
        }
        o << "}\n";
        return o;
      }

    private:
      ::addrinfo *m_ai;
    };

    class tcp_server_context_t
    {
    public:
      tcp_server_context_t()
      {
      }
      virtual ~tcp_server_context_t()
      {
      }
    };

    class tcp_server_session_base_t
    {
    public:
      tcp_server_session_base_t()
      {
      }

      virtual ~tcp_server_session_base_t()
      {
      }

      virtual void run(socket_handle_t s, struct sockaddr *addr, socklen_t addrlen) = 0;

    };

    inline void set_socket_nonblocking(socket_handle_t s)
    {
      fcntl(s, F_SETFL, fcntl(s, F_GETFL, 0) | O_NONBLOCK);
    }

    inline void set_socket_blocking(socket_handle_t s)
    {
      fcntl(s, F_SETFL, fcntl(s, F_GETFL, 0) & (~O_NONBLOCK));
    }

#if defined(IF3_CONFIG_POSIX)
    class tcp_fork_server_t
    {
    public:
      tcp_fork_server_t(
          const char *bind_hostname,
          const char *bind_port,
          tcp_server_session_base_t &session_handler,
          int num_forks_per_addr,
          string_t title) :
        m_bind_address(bind_hostname, bind_port), m_session_handler(session_handler), m_num_forks_per_addr(num_forks_per_addr),
            m_title(title), m_observer(title), m_done(false)
      {
      }

      virtual ~tcp_fork_server_t()
      {
      }

      virtual void run();

      virtual void run_on_address(net_address_t::const_iterator &a);

      virtual void run_accept(socket_handle_t accept_socket);

      virtual void on_sigalrm()
      {
        daemon::default_on_sigalrm();
      }

      virtual void on_sigterm()
      {
        daemon::default_on_sigterm();
        m_done=true;
      }

      virtual void on_sigchld()
      {
        daemon::process_sigchlds();
      }

      virtual void on_sighup()
      {
        daemon::default_on_sighup();
      }

      virtual void on_sigint()
      {
        daemon::default_on_sigint();
        m_done=true;
      }

      virtual void on_sigquit()
      {
        daemon::default_on_sigquit();
        m_done=true;
      }

    private:
      net_address_t m_bind_address;
      tcp_server_session_base_t &m_session_handler;
      int m_num_forks_per_addr;
      string_t m_title;
      daemon::signals_observer_t m_observer;
      bool m_done;
    };

#endif

    inline int64_t timeval_to_usec(const struct timeval* tv)
    {
      return ((int64_t) tv->tv_sec * 1000000 + tv->tv_usec);
    }

    inline timeval* usec_to_timeval(timeval* tv, int64_t usec)
    {
      tv->tv_sec = usec / 1000000;
      tv->tv_usec = usec % 1000000;
      return tv;
    }

    inline
    void timeval_add(struct timeval *r, const struct timeval *a, const struct timeval *b)
    {
      usec_to_timeval(r, timeval_to_usec(a) + timeval_to_usec(b));
    }

    inline
    void timeval_add(struct timeval *r, const struct timeval *a, int64_t b_usec)
    {
      usec_to_timeval(r, timeval_to_usec(a) + b_usec);
    }

    inline
    void timeval_add(struct timeval *r, int64_t a_usec, const struct timeval *b)
    {
      usec_to_timeval(r, timeval_to_usec(b) + a_usec);
    }

    inline
    void timeval_sub(struct timeval *r, const struct timeval *a, const struct timeval *b)
    {
      usec_to_timeval(r, timeval_to_usec(a) - timeval_to_usec(b));
    }

    inline
    void timeval_sub(struct timeval *r, const struct timeval *a, int64_t b_usec)
    {
      usec_to_timeval(r, timeval_to_usec(a) - b_usec);
    }

    inline
    void timeval_sub(struct timeval *r, int64_t a_usec, const struct timeval *b)
    {
      usec_to_timeval(r, a_usec - timeval_to_usec(b));
    }

    inline int64_t timeval_compare(const struct timeval *a, const struct timeval *b)
    {
      return timeval_to_usec(a) - timeval_to_usec(b);
    }

    bool read_until_pattern(buf_t &dest_buf, buf_t &overflow_buf, socket_handle_t sock, string_t end_pattern, int max_len = 1024
        * 10, int max_total_time_in_ms = 500);

    bool prepare_read_block(buf_t &dest_buf, int max_read_chunk = 8192);

    int read_block(buf_t &dest_buf, socket_handle_t sock, int max_read_chunk = 8192);

    int write_block(const buf_t &src_buf, socket_handle_t sock, int max_write_chunk = 4096);
  }
}

#endif
