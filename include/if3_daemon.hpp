#ifndef IF3_DAEMON_HPP
#define IF3_DAEMON_HPP

#include "if3_world.hpp"
#include "if3_string.hpp"
#include "if3_util.hpp"
#include "if3_syslog.hpp"

namespace if3
{
  namespace daemon
  {
#if defined(IF3_CONFIG_POSIX)
    typedef std::deque< pid_t > children_list_t;
    extern children_list_t running_children;

    extern volatile int32_t num_sigterm;
    extern volatile int32_t num_sighup;
    extern volatile int32_t num_sigint;
    extern volatile int32_t num_sigchld;
    extern volatile int32_t num_sigquit;
    extern volatile int32_t num_sigalrm;

    void drop_root(
                   string_t uid_name
                   );

    void daemonize(
                   bool real_daemon,
                   string_t identity,
                   filename_t home_dir,
                   filename_t pid_file,
                   string_t new_uid,
                   filename_t log_file = ""
                   );

    void end();

    void handle_sigterm(int);
    void handle_sigint(int);
    void handle_sighup(int);
    void handle_sigchld(int);
    void handle_sigquit(int);
    void handle_sigalrm(int);

    void setup_sigterm();
    void setup_sigchld();
    void setup_sigint();
    void setup_sighup();
    void setup_sigquit();
    void setup_sigalrm();

    void register_pid(pid_t pid);
    void unregister_pid(pid_t pid);

    void process_sigchlds();

    void send_signal_to_children(int sig);

    void prepare_child_start();

    void default_on_sigterm();
    void default_on_sigchld();
    void default_on_sigint();
    void default_on_sighup();
    void default_on_sigquit();
    void default_on_sigalrm();


    pid_t do_fork();

    inline
    void
    run_in_fork(
                function< void() > thing_to_run_in_fork,
                int fork_count = 1
                )
    {
      for (int i = 0; i < fork_count; ++i)
      {
        pid_t pid = do_fork();
        if (pid == 0)
        {
          // in fork child
          try
          {
            thing_to_run_in_fork();
          }
          catch (std::exception &e)
          {
            logger->log_error("running in fork failed: %s", e.what());
            _exit(1);
          }
          _exit(0);
        }
        else if (pid < 0)
        {
          // error forking
          throw std::runtime_error("Error Forking");
        }
      }
    }

    struct change_observer_t
    {
      change_observer_t( volatile int32_t &observed )
        : m_observed(observed),
          m_snapshot(observed),
          m_newvalue(observed)
      {
      }

      bool did_change()
      {
        m_newvalue = m_observed;
        return m_newvalue != m_snapshot;
      }

      int32_t accept_change()
      {
        m_snapshot = m_newvalue;
        return m_newvalue;
      }

    private:
      volatile int32_t &m_observed;
      int32_t m_snapshot;
      int32_t m_newvalue;
    };


    class signals_observer_t
    {
    public:
      signals_observer_t(
          string_t title,
          function<void()> on_sigterm = default_on_sigterm,
          function<void()> on_sigchld = default_on_sigchld,
          function<void()> on_sighup = default_on_sighup,
          function<void()> on_sigint = default_on_sigint,
          function<void()> on_sigquit = default_on_sigquit,
          function<void()> on_sigalrm = default_on_sigalrm) :
        m_title(title),
        m_on_sigterm(on_sigterm),
        m_on_sigchld(on_sigchld),
        m_on_sighup(on_sighup),
        m_on_sigint(on_sigint),
        m_on_sigquit(on_sigquit),
        m_on_sigalrm(on_sigalrm),
        m_sigterm_observer(num_sigterm),
        m_sigchld_observer(num_sigchld),
        m_sighup_observer(num_sighup),
        m_sigint_observer(num_sigint),
        m_sigquit_observer(num_sigquit),
        m_sigalrm_observer(num_sigalrm)
      {
        setup_sigterm();
        setup_sighup();
        setup_sigint();
        setup_sigquit();
        setup_sigalrm();
      }

      void set_on_sigterm( function< void () > f )
      {
        m_on_sigterm = f;
      }

      void set_on_sigchld( function< void () > f )
      {
        m_on_sigchld = f;
      }

      void set_on_sighup( function< void () > f )
      {
        m_on_sighup = f;
      }

      void set_on_sigint( function< void () > f )
      {
        m_on_sigint = f;
      }

      void set_on_sigquit( function< void () > f )
      {
        m_on_sigquit = f;
      }

      void set_on_sigalrm( function< void () > f )
      {
        m_on_sigalrm = f;
      }

      bool poll()
      {
        bool r = false;

        if (m_sigterm_observer.did_change())
        {
          logger->log_info("%s: SIGTERM", m_title.c_str());
          m_sigterm_observer.accept_change();
          m_on_sigterm();
          r = true;
        }

        if (m_sigchld_observer.did_change())
        {
          m_sigchld_observer.accept_change();
          process_sigchlds();
          r = true;
        }

        if (m_sighup_observer.did_change())
        {
          logger->log_info("%s: SIGHUP", m_title.c_str());
          m_sighup_observer.accept_change();
          m_on_sighup();
          r = true;
        }

        if (m_sigint_observer.did_change())
        {
          logger->log_info("%s: SIGINT", m_title.c_str());
          m_sigint_observer.accept_change();
          m_on_sigint();
          r = true;
        }

        if (m_sigquit_observer.did_change())
        {
          logger->log_info("%s: SIGQUIT", m_title.c_str());
          m_sigquit_observer.accept_change();
          m_on_sigquit();
          r = true;
        }

        if (m_sigalrm_observer.did_change())
        {
          logger->log_info("%s: SIGALRM", m_title.c_str());
          m_sigalrm_observer.accept_change();
          m_on_sigalrm();
          r = true;
        }
        return r;
      }

    private:
      string_t m_title;
      function<void()> m_on_sigterm;
      function<void()> m_on_sigchld;
      function<void()> m_on_sighup;
      function<void()> m_on_sigint;
      function<void()> m_on_sigquit;
      function<void()> m_on_sigalrm;

      change_observer_t m_sigterm_observer;
      change_observer_t m_sigchld_observer;
      change_observer_t m_sighup_observer;
      change_observer_t m_sigint_observer;
      change_observer_t m_sigquit_observer;
      change_observer_t m_sigalrm_observer;
    };

    void wait_for_children_to_finish(
                                     const char *title,
                                     signals_observer_t &signals_observer
                                     );

    inline void wait_for_children_to_finish(
                                     const char *title
                                     )
    {
      signals_observer_t o(title);
      wait_for_children_to_finish( title, o );
    }

#endif
  }
}

#endif
