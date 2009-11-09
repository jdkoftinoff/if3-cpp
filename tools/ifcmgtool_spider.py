#! /usr/bin/env python

from HTMLParser import HTMLParser
import httplib
import re
import sys
import urlparse
import time
import urllib
import urllib2
import threading
import datetime
import ifcmgkernel

def ifcmgkernel_run_scan( link, text, links, content_links ):
    try:
        return ifcmgkernel.run_scan(link,text,links,content_links)
    except:
        return [0,0,0]

class MyParser(HTMLParser):
    def __init__(self, spider):
        HTMLParser.__init__(self)
        self.spider = spider
        self.text = ''
        self.content_links = ''
        self.links = ''

    def reset(self):
        HTMLParser.reset(self)
        self.text = ''
        self.content_links = ''
        self.links = ''

    def handle_starttag(self, tag, attrs):
        if tag == 'a':
            self.handle_a(tag,attrs)
        elif tag == 'img':
            self.handle_img(tag,attrs)
        elif tag == 'iframe':
            self.handle_iframe(tag,attrs)
        elif tag == 'script':
            self.handle_script(tag,attrs)
            
    def handle_a(self,tag,attrs):
        for i in attrs:
            if i[0] == 'href':
                self.handle_link(i[1])
                return None

    def handle_img(self,tag,attrs):
        for i in attrs:
            if i[0] == 'src':
                self.handle_content_link( i[1] )
            if i[0] == 'alt':
                self.handle_data( i[1] )

    def handle_script(self,tag,attrs):
        for i in attrs:
            if i[0] == 'src':
                self.handle_content_link( i[1] )
                return None

    def handle_iframe(self,tag,attrs):
        for i in attrs:
            if i[0] == 'src':
                self.handle_content_link( i[1] )
                return None

    def handle_link(self,link):
        self.spider.validate_add_todo(link)
        self.links += link + '\n'

    def handle_content_link(self,link):
        self.content_links += link + '\n'


    def handle_data(self,data):
        t = data.strip()
        if len(t)>0:
            self.text += data + '\n'

    def handle_endtag(self, tag):
        pass

class ScanResults:
    def __init__(self,link,tts, bytes, category, ad_tags_found, accessed, date ):
        self.link = link
        self.tts = tts
        self.bytes = bytes
        self.category = category
        self.ad_tags_found = ad_tags_found
        self.accessed = accessed
        self.date = date
        
    def __str__(self):
        r = '%s\t%d\t%d\t%d\t%d\t%d\t%s' \
            % ( self.link, self.accessed, self.category, self.ad_tags_found, self.bytes, self.tts, self.date )
        return r
        
class Spider:
    viewed_queue = []
    todo_queue = []
  
    def __init__(self, site, pagelimit):
        self.site = site
        self.parser = MyParser(self)
        self.todo_queue.append('http://' + site + '/')
        self.pagelimit = pagelimit
        self.done = False
        self.bytes = 0
        self.current_link = 'http://' + site + '/'

    def close(self):
        self.parser.reset()
        self.parser.close()
        
    def run(self):
        if self.pagelimit <= 0:
            self.done = True
            return None
        if self.todo_queue == []:
            self.done = True
            return None
        else:
            starttime = time.clock()
            link = self.todo_queue.pop(0)
            html = self.get_html(link)
            if html == None:
                return ScanResults(
                    link,
                    int((time.clock()-starttime)*1000),
                    0,
                    0,
                    0,
                    0,
                    datetime.datetime.now()
                    )
            else:
                if self.parse(link,html):
                    self.pagelimit -= 1
                    return self.scan( link, starttime )
                else:
                    self.pagelimit -= 1
                    return self.scan_after_failed_parse( link, starttime, html )

    def scan_after_failed_parse(self,link,starttime, html):
        r = ifcmgkernel_run_scan(
                link,
                html,
                html,
                html
                )

        accessed = r[0]
        category = r[1]
        ad_tags_found = r[2]

        return ScanResults(
            link,
            int((time.clock()-starttime)*1000),
            self.bytes,
            category,
            ad_tags_found,
            accessed,
            datetime.datetime.now()
            )

    def scan(self,link,starttime):
        r = ifcmgkernel_run_scan(
                link,
                self.parser.text,
                self.parser.links,
                self.parser.content_links
                )

        accessed = r[0]
        category = r[1]
        ad_tags_found = r[2]

        return ScanResults(
            link,
            int((time.clock()-starttime)*1000),
            self.bytes,
            category,
            ad_tags_found,
            accessed,
            datetime.datetime.now()
            )

    def parse(self,link,html):
        try:
            self.current_link = self.fixup(link)
            self.parser.reset()
            self.bytes = len(html)
            self.parser.feed(html)
            return True
        except:
            return False

    def fixup(self,link):
        pos = link.find('#')
        if pos>=0:
            r=link[:pos]
        else:
            r = link
        return r

    def validate_add_todo(self,link):
        plain_url = urlparse.urljoin(self.current_link, self.fixup(link), False )
        link = urlparse.urlsplit(plain_url,'http',False)
        if link.netloc != self.site:
                return None
        self.add_todo(plain_url)

    def add_todo(self, link):
        if (link in self.viewed_queue) == False:
            self.todo_queue.append(link)
            self.viewed_queue.append(link)

    def get_html(self, link):
        try:
            url = link
            user_agent = 'Mozilla/4.0 (compatible; MSIE 5.5; Windows NT)'
            headers = { 'User-Agent' : user_agent }
            req = urllib2.Request(url, None, headers)

            response = urllib2.urlopen(req)

            content_type = response.info()['Content-Type']
            if content_type[:4] == 'text':
                the_page = response.read()
                #print the_page
                return the_page
        except:
            pass
        return None


def main():
    ifcmgkernel.startup('/opt/cmg/ifcmgdb/share/ifcmgdb-pre','/opt/cmg/ifcmgdb/share/ifcmgdb-non')
    s = Spider(sys.argv[1],10)

    while s.done == False:
        r=s.run()
        if r is not None:
            print r

    s.close()

if __name__ == "__main__":
    main()
