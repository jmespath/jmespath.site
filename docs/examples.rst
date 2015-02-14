=================
JMESPath Examples
=================


This page contains numerous examples of JMESPath examples
in action.

.. note::

  Do you have any examples you'd like to add?  Send a
  `pull request <https://github.com/jmespath/jmespath.site>`__ on github.
  Are there examples you'd like to see that aren't here?  Let us know
  by opening an `issue on github <https://github.com/jmespath/jmespath.site/issues>`__.



Filters and Multiselects
========================

One of the most common usage scenarios for JMESPath is being able to take
a complex JSON document and simplify it down.  The main main features at work
here are filters and multiselects.  We'll take a look at some examples.


.. jpexample:: people[?age > `20`].[name, age]
    :layout: 2cols-long

    {
      "people": [
        {
          "age": 20,
          "other": "foo",
          "name": "Bob"
        },
        {
          "age": 25,
          "other": "bar",
          "name": "Fred"
        },
        {
          "age": 30,
          "other": "baz",
          "name": "George"
        }
      ]
    }

In this example we're taking an array of hashes, and only simplifying down an
array of two element arrays containing a name and an age.  We're also only
including list elements where the ``age`` key is greater than ``20``.  If
instead we want to create the same structure, but only include the ``age`` and
``name`` key, we can instead say:


.. jpexample:: people[?age > `20`].{name: name, age: age}
    :layout: 2cols-long

    {
      "people": [
        {
          "age": 20,
          "other": "foo",
          "name": "Bob"
        },
        {
          "age": 25,
          "other": "bar",
          "name": "Fred"
        },
        {
          "age": 30,
          "other": "baz",
          "name": "George"
        }
      ]
    }


The last half of the above expression contains key value pairs which have the
general form ``keyname: <expression>``.  In the above expression we're just
using a field as an expression, but they can be more advanced expressions.  For
example:

.. jpexample:: people[*].{name: name, tags: tags[0]}
    :layout: 2cols-long

    {
      "people": [
        {
          "age": 20,
          "tags": ["a", "b", "c"],
          "name": "Bob"
        },
        {
          "age": 25,
          "tags": ["d", "e", "f"],
          "name": "Fred"
        },
        {
          "age": 30,
          "tags": ["g", "h", "i"],
          "name": "George"
        }
      ]
    }
