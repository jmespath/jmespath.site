"""Generate a sitemap.xml.

This extension will generate a sitemap.xml file by recording links while the
site is buliding.  This code is based off the version from the guzzle sphinx
theme with the following license:

Copyright (c) 2013 Michael Dowling <mtdowling@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
"""
import os
import urllib.parse
import xml.etree.ElementTree as ET


def setup(app):
    """Setup conntects events to the sitemap builder."""
    app.connect('html-page-context', add_html_link)
    app.connect('build-finished', create_sitemap)
    app.sitemap_links = []


def add_html_link(app, pagename, templatename, context, doctree):
    """As each page is built, collect page names for the sitemap"""
    base_url = app.config['html_theme_options'].get('base_url', '')
    if base_url:
        full_url = urllib.parse.urljoin(base_url, pagename + '.html')
        app.sitemap_links.append(full_url)


def create_sitemap(app, exception):
    """Generates the sitemap.xml from the collected HTML page links"""
    if exception is not None or not app.sitemap_links:
        return
    sitemap_filename = os.path.join(app.outdir, 'sitemap.xml')
    print("Generating sitemap.xml in %s" % sitemap_filename)
    root = ET.Element("urlset")
    root.set("xmlns", "https://www.sitemaps.org/schemas/sitemap/0.9")
    for link in app.sitemap_links:
        url = ET.SubElement(root, "url")
        ET.SubElement(url, "loc").text = link
    ET.ElementTree(root).write(sitemap_filename)
