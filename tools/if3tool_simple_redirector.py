#!/usr/bin/env python

import os
import tornado.httpserver
import tornado.ioloop
import tornado.options
import tornado.web
from tornado.options import define, options
import urllib
import if3kernel
import urlparse

define("port", default=8080, help="run port", type=int)


class FavIcon(tornado.web.RequestHandler):
  def get(self):
    self.set_header("Content-Type","image/vnd.microsoft.icon") 

class HealthCheck(tornado.web.RequestHandler):
  def get(self):
    self.set_header("Content-Type","text/plain")
    self.write("Good")

class AutoRedirector(tornado.web.RequestHandler):
  def get(self):
      self.redirect("http://ad2.adsovo.com" + self.request.uri)
  def post(self):
      self.redirect("http://ad2.adsovo.com" + self.request.uri)      
      
#  self.set_header("Content-Type","text/plain")
#  self.write("uri = %s\n" % (self.request.uri) )
    
    
class SimpleRedirector(tornado.web.RequestHandler):
  def get(self):
    ad_type = self.get_argument("ad_type")
    ad_size = self.get_argument("ad_size")
    section = self.get_argument("section")
    trace = int(self.get_argument("t","0"))
    referer= self.get_argument("q","null")
    if self.request.headers.has_key("Referer"):
      referer = self.request.headers["Referer"]
    referer_url = urlparse.urlsplit(referer)
    
    r=if3kernel.scan_url(trace,referer_url.netloc, referer_url.geturl())

    result = r[0]-1
    proof = r[1]
    
    fil = "bw"
    gotit = False
    logline = referer + "\t" + str(result) + "\n"
    if result == 0:
      fil = "gw"
      self.log_known_good.write(logline)
      self.log_known_good.flush()
      gotit = True
    if result == 1:
      fil = "bw"
      self.log_known_bad.write(logline)
      self.log_known_bad.flush()
      gotit = True
    if result == 36:
      fil = "unknown"
      self.log_unknown.write(logline)
      self.log_unknown.flush()
      gotit = True

    if gotit != True:
      self.log_unknown.write(logline)

    params = {
      'ad_type' : ad_type,
      'ad_size' : ad_size,
      'section' : section,
      'fil' : fil,
    }
     
    redirect_to ="http://ad2.adsovo.com/st?" + urllib.urlencode(params)
    if trace == 1: 
      self.set_header("Content-Type","text/plain")
      self.write("uri = %s\n" % (self.request.uri) )
      self.write("ad_type = " + ad_type + "\n" )
      self.write("ad_size = " + ad_size + "\n" )
      self.write("section = " + section + "\n" )
      self.write("referer was: " + referer + "\n")
      self.write("redirection to : " + redirect_to + "\n")
      self.write("result is : " + str(result) + "\n" )
      self.write("proof is : \n" + proof + "\n")
    else:
      self.redirect( redirect_to )

def main():  
  IF3_home = '/opt/IF3'
  if os.environ.has_key('IF3_HOME'):
    IF3_home = os.environ['IF3_HOME']
  precompiled_path = os.path.join( IF3_home, 'share/if3db-pre' )
  if3kernel.startup( 
    os.path.join(precompiled_path,'hostnames.pre'),
    os.path.join(precompiled_path,'urls.pre'),
    os.path.join(precompiled_path,'phrases.pre')
    )
  log_known_good = open("known_good.log","a")
  log_unknown = open("unknown.log","a")
  log_known_bad = open("known_bad.log","a")
  
  SimpleRedirector.log_known_good = log_known_good
  SimpleRedirector.log_unknown = log_unknown
  SimpleRedirector.log_known_bad = log_known_bad
  try:
    tornado.options.parse_command_line()
    application = tornado.web.Application([
        (r"/health", HealthCheck ),
        (r"/st", SimpleRedirector ),
        (r"/favicon.ico", FavIcon ),
        (r"/.*", AutoRedirector ),
        ])
    http_server = tornado.httpserver.HTTPServer(application)
    http_server.listen(options.port)
    tornado.ioloop.IOLoop.instance().start()
  finally:
    if3kernel.shutdown()
    log_known_bad.close()
    log_known_good.close()
    log_unknown.close()
      
if __name__ == "__main__":
  main()

