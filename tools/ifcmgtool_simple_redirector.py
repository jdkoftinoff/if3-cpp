#!/usr/bin/env python

import tornado.httpserver
import tornado.ioloop
import tornado.options
import tornado.web

from tornado.options import define, options

define("port", default=80, help="run port", type=int)

class HealthCheck(tornado.web.RequestHandler):
  def get(self):
    self.write("Good")

class SimpleRedirector(tornado.web.RequestHandler):
  def get(self,params):
    self.redirect("http://ad2.adsovo.com" + self.request.uri)

def main():
  tornado.options.parse_command_line()
  application = tornado.web.Application([
    (r"/health", HealthCheck ),
	  (r"/st?(.*)", SimpleRedirector )
  ])
  http_server = tornado.httpserver.HTTPServer(application)
  http_server.listen(options.port)
  tornado.ioloop.IOLoop.instance().start()

if __name__ == "__main__":
    main()
