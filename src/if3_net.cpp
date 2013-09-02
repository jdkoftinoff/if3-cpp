#include "if3_world_precompiled.hpp"
#include "if3_net.hpp"


#ifdef _WIN32

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif


int gettimeofday(struct timeval *tv, struct timezone *tz)
{
  FILETIME ft;
  unsigned __int64 tmpres = 0;
  static int tzflag;

  if (NULL != tv)
  {
    GetSystemTimeAsFileTime(&ft);

    tmpres |= ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;

    /*converting file time to unix epoch*/
    tmpres /= 10;  /*convert into microseconds*/
    tmpres -= US_DELTA_EPOCH_IN_MICROSECS;
    tv->tv_sec = (long)(tmpres / 1000000UL);
    tv->tv_usec = (long)(tmpres % 1000000UL);
  }
  return 0;
}

#endif


namespace if3
{
  namespace net
  {
#ifdef _WIN32
    void net_init()
    {
      WSADATA wsaData;
      WORD version;
      int error;
      version = MAKEWORD( 2, 2 );
      error = WSAStartup( version, &wsaData );
      /* check for error */
      if ( error != 0 )
      {
        /* error occured */
        throw("Can't initialize Winsock");
      }
      /* check for correct version */
      if ( version != wsaData.wVersion )
      {
        /* incorrect WinSock version */
        if3_throw(std::runtime_error,"Incorrect Winsock version");
      }
      /* WinSock has been initialized */
    }
#else
    void net_init()
    {
      signal(SIGPIPE, SIG_IGN);
    }
#endif

#if defined(IF3_CONFIG_POSIX)
    void tcp_fork_server_t::run()
    {
      for (net_address_t::const_iterator i = m_bind_address.begin(); i != m_bind_address.end(); ++i)
      {
        logger->log_info("about to fork for binding to %s", to_string(*i).c_str());

        daemon::run_in_fork(std::bind(&tcp_fork_server_t::run_on_address, this, i));
      }
    }

    void tcp_fork_server_t::run_on_address(net_address_t::const_iterator &a)
    {
      logger->log_info("Starting run on %s", to_string(*a).c_str());

      socket_handle_t sock = socket(a->ai_family, a->ai_socktype, a->ai_protocol);
      if (sock == -1)
      {
        throw std::runtime_error("unable to create socket");
      }

      const char opt = 1;
      setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

      if (bind(sock, a->ai_addr, a->ai_addrlen) != 0)
      {
        if (errno != EADDRINUSE)
        {
          throw std::runtime_error(string_t("bind: ") + string_t(strerror(errno)));
        }
        close(sock);
      }

      if (listen(sock, SOMAXCONN) != 0)
      {
        throw std::runtime_error(string_t("listen: ") + string_t(strerror(errno)));
      }

      daemon::run_in_fork(std::bind(&tcp_fork_server_t::run_accept, this, sock), m_num_forks_per_addr);

      daemon::wait_for_children_to_finish("parent of acceptors", m_observer);
    }

    void tcp_fork_server_t::run_accept(socket_handle_t accept_socket)
    {
      IF3_LOG_DEBUG("%s: Entering run_accept", m_title.c_str());

      m_observer.set_on_sigterm( std::bind( &tcp_fork_server_t::on_sigterm, this ) );
      m_observer.set_on_sigchld( std::bind( &tcp_fork_server_t::on_sigchld, this ) );
      m_observer.set_on_sigint( std::bind( &tcp_fork_server_t::on_sigint, this ) );
      m_observer.set_on_sighup( std::bind( &tcp_fork_server_t::on_sighup, this ) );
      m_observer.set_on_sigquit( std::bind( &tcp_fork_server_t::on_sigquit, this ) );
      m_observer.set_on_sigalrm( std::bind( &tcp_fork_server_t::on_sigalrm, this ) );


      while (!m_done)
      {
        struct sockaddr_storage rem;
        socklen_t remlen = sizeof(rem);
        socket_handle_t fd;

        IF3_LOG_DEBUG("%s about to accept", m_title.c_str());
        do
        {
          if (m_observer.poll())
          {
            IF3_LOG_DEBUG("tcp_fork_server_t::run_accept saw signal");
          }

          IF3_LOG_DEBUG("%s calling accept", m_title.c_str());
          fd = accept(accept_socket, (struct sockaddr *) &rem, (socklen_t *) &remlen);

          IF3_LOG_DEBUG("%s accept returned %d", m_title.c_str(), fd);
        } while ((fd < 0 && errno == EINTR) && !m_done );

        if (fd < 0)
        {
          if( m_done )
          {
            break;
          }
          IF3_LOG_DEBUG("accept() failed: %s", strerror(errno));
        }
        else
        {
          IF3_LOG_DEBUG("accepted connection on socket %d", fd);

          try
          {
            m_session_handler.run(fd, (struct sockaddr *) &rem, remlen);
          }
          catch ( std::exception &e )
          {
            logger->log_error("session handler caught exception: %s", e.what());
          }
          close(fd);
        }
      }

      IF3_LOG_DEBUG("%s: leaving run_accept", m_title.c_str());

    }
#endif

    bool read_until_pattern(
        buf_t &dest_buf,
        buf_t &overflow_buf,
        socket_handle_t sock,
        string_t end_pattern,
        int max_len,
        int max_total_time_in_ms)
    {
      bool r = false;
      bool done = false;
      int e;
      int next_scan_start_pos = 0;
      socket_handle_t max_sock = sock + 1;
      timeval current_time;
      int64_t deadline_time;

      dest_buf.clear();
      overflow_buf.clear();
      if (gettimeofday(&current_time, NULL) < 0)
      {
        logger->log_error("gettimeofday error: %s", strerror(errno));
        return false;
      }
      deadline_time = timeval_to_usec(&current_time) + max_total_time_in_ms * 1000;

      fd_set readable;
      FD_ZERO( &readable );

      do
      {
        if (gettimeofday(&current_time, NULL) < 0)
        {
          logger->log_error("gettimeofday error: %s", strerror(errno));
          break;
        }
        int64_t usecs_to_deadline = deadline_time - timeval_to_usec(&current_time);
        if (usecs_to_deadline < 0)
        {
          IF3_LOG_DEBUG("deadline hit");
          break;
        }

        timeval timeout_time;
        usec_to_timeval(&timeout_time, usecs_to_deadline);

        FD_SET( sock, &readable );
        do
        {
          e = select(max_sock, &readable, 0, 0, &timeout_time);
        } while (e == -1 && (errno == EAGAIN || errno == EINTR));

        // error?
        if (e < 0)
        {
          logger->log_error("error on select: %s", strerror(errno));
          break;
        }
        // timeout?
        if (e == 0)
        {
          IF3_LOG_DEBUG("timeout");
          break;
        }
        if (e == 1 && FD_ISSET(sock, &readable))
        {
          uint8_t read_buf[2048];

          do
          {
            e = recv(sock, (char *)&read_buf[0], sizeof(read_buf), 0);
          } while (e == -1 && (errno = EINTR));

          // error?
          if (e < 0)
          {
            logger->log_error("recv error: %s", strerror(errno));
            break;
          }
          // socket closed?
          if (e == 0)
          {
            IF3_LOG_DEBUG("socket closed");
            break;
          }
          // data?
          if (e > 0)
          {
            if ((dest_buf.get_data_length() + e) > max_len)
            {
              IF3_LOG_DEBUG("receive data larger than %d, stopping.", max_len);
              break;
            }
            dest_buf.append_from_data(read_buf, e);
            int end_pattern_length = end_pattern.length();
            int dest_buf_length = dest_buf.get_data_length();
            int end_scan_pos = dest_buf_length;
            unsigned char *d = dest_buf.get_data();
            IF3_LOG_DEBUG("dest_buf length: %d", dest_buf_length);
            for (int i = next_scan_start_pos; i < end_scan_pos; ++i)
            {
              if (d[i] == (unsigned char) end_pattern.at(0))
              {
                IF3_LOG_DEBUG("found start of potential pattern at %d", i);

                bool completed = true;
                int last_match_pos = 0;
                for (int j = 0; j < end_pattern_length; ++j)
                {
                  if (d[i + j] == (unsigned char) end_pattern.at(j))
                  {
                    IF3_LOG_DEBUG("char %d (0x%02X) matches pattern char %d", i + j, (int) d[i + j], j);
                  }
                  else
                  {
                    IF3_LOG_DEBUG("char %d (0x%02X) does not match pattern char %d (%0x%02X)", i + j, (int) d[i + j], j,
                        end_pattern.at(j));
                    completed = false;
                    break;
                  }
                }
                if (completed)
                {
                  IF3_LOG_DEBUG("found entire pattern ending so length is %d", i + end_pattern_length);

                  done = true;
                  last_match_pos = i + end_pattern_length;
                  overflow_buf.append_from_data(d + last_match_pos, dest_buf_length - last_match_pos);
                  dest_buf.set_data_length(last_match_pos);
                  break;
                }
              }
            }

            if (!done)
            {
              next_scan_start_pos = dest_buf_length - end_pattern_length;
              if (next_scan_start_pos < 0)
                next_scan_start_pos = 0;
            }
          }
        }

      } while (!done);

      return r;
    }

    bool prepare_read_block(buf_t &dest_buf, int max_read_chunk)
    {
      bool r = true;
      if (dest_buf.get_buf_length() - dest_buf.get_data_length() < max_read_chunk)
      {
        if (!dest_buf.expand(dest_buf.get_buf_length() + max_read_chunk))
        {
          r = false;
        }
      }
      return r;
    }

    int read_block(buf_t &dest_buf, socket_handle_t sock, int max_read_chunk)
    {
      unsigned char * dest_pos = dest_buf.get_data() + dest_buf.get_data_length();
      ssize_t dest_len = ::read(sock, dest_pos, max_read_chunk);
      if (dest_len > 0)
      {
        dest_buf.set_data_length(dest_buf.get_data_length() + dest_len);
      }
      return dest_len;
    }

    int write_block(const buf_t &src_buf, socket_handle_t sock, int max_write_chunk)
    {
      const unsigned char *src_pos = src_buf.get_data();
      int todo = src_buf.get_data_length();
      while (todo > 0)
      {
        ssize_t r;
        int chunk_todo = todo;
        if (chunk_todo > max_write_chunk)
          chunk_todo = max_write_chunk;

        do
        {
          r = ::write(sock, src_pos, chunk_todo);
        } while (r == -1 && errno == EINTR);

        if (r == -1)
        {
          break;
        }
      }
      return src_buf.get_data_length() - todo;
    }
  }
}
