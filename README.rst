JMESPath Site
=============

.. image:: https://badges.gitter.im/Join Chat.svg
   :target: https://gitter.im/jmespath/chat


This is the repo for the website: http://jmespath.org

Join us on our `Gitter channel <https://gitter.im/jmespath/chat>`__.


Overview
========

The http://jmespath.org/ site is a static website that uses
`Sphinx <http://sphinx-doc.org/>`__, a python documentation
system, under the hood.  If you are familiar with sphinx, then you know how to
build this static site.

Working on the JMESPath Site
============================

Working on the jmespath site is just like any other python project that uses
sphinx for documentation.  If you are not familiar with the process, the steps
below outline what's needed to work on the JMESPath site.

Initial Setup
-------------

First create a virtual environment::

  $ virtualenv venv
  $ . venv/bin/activate

If you do not have the "virtualenv" exectuable installed, you can use ``pip``
to install it::

  $ pip install virtualenv
  $ virtualenv venv
  $ . venv/bin/activate

Next, install the python package requirements::

  $ pip install -r requirements.txt

You're now all set to start working on the JMESPath site.  Note that if you
want to exist the virtualenv environment once you're done working on the
JMESPath site, you can just run "deactivate", which will deactivate your
virtual environment.

Making Changes to the Site
--------------------------

Once you have a virtual environment set up, you can start working on the
JMESPath site. All the content is under the ``docs/`` folder.  All of the
content is written using reStructuredText.  If you are not familiar with the
syntax, the sphinx site has an excellent
`reStructuredText primer <http://sphinx-doc.org/rest.html>`__.

Once you've made changes you can build the docs by running ``make html``.  Make
sure that you've activated your virtual environment (this was done in the
previous section when you ran ``. venv/bin/activate``.  Once the docs have
finished building the html files will be in ``docs/_build/html/``.  **Pro
Tip**: python has a built in web server you can use the easily view the
rendered html::

  $ make html
  $ cd docs/_build/html
  $ python -m http.server

You can then view the docs at http://localhost:8000/

License
=======

All source code in this repo is licensed under Apache 2 (see LICENSE.txt).

All documentation, including the JMESPath specification, is Creative
Commons licensed (CC BY 4.0). See LICENSE-docs.txt.
