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



Filters and Multiselect Lists
=============================

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


Filters and Multiselect Hashes
==============================

In the previous example we were taking an array of hashes, and only simplifying
down to an array of two element arrays containing a name and an age.  We're
also only including list elements where the ``age`` key is greater than ``20``.
If instead we want to create the same structure, but only include the ``age``
and ``name`` key, we can instead say:


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


Working with Nested Data
========================


.. jpexample:: reservations[].instances[].[tags[?Key==`Name`].Values[] | [0], type, state.name]
    :layout: 2cols-long

    {
      "reservations": [
        {
          "instances": [
            {"type": "small",
             "state": {"name": "running"},
             "tags": [{"Key": "Name",
                       "Values": ["Web"]},
                      {"Key": "version",
                       "Values": ["1"]}]},
            {"type": "large",
             "state": {"name": "stopped"},
             "tags": [{"Key": "Name",
                       "Values": ["Web"]},
                      {"Key": "version",
                       "Values": ["1"]}]}
          ]
        }, {
          "instances": [
            {"type": "medium",
             "state": {"name": "terminated"},
             "tags": [{"Key": "Name",
                       "Values": ["Web"]},
                      {"Key": "version",
                       "Values": ["1"]}]},
            {"type": "xlarge",
             "state": {"name": "running"},
             "tags": [{"Key": "Name",
                       "Values": ["DB"]},
                      {"Key": "version",
                       "Values": ["1"]}]}
          ]
        }
      ]
    }

The above example combines several JMESPath features including the flatten
operator, multiselect lists, filters, and pipes.

The input data contains a top level key, "reservations", which is a list.
Within each list, there is an "instances" key, which is also a list.

The first thing we're doing here is creating a single list from multiple lists
of instances.  By using the :ref:`flatten` we can take the
two instances from the first list and the two instances from the second list,
and combine them into a single list.  Try changing the above expression to just
``reservations[].instances[]`` to see what this flattened list looks like.
Everything to the right of the ``reservations[].instances[]`` is about taking
the flattened list and paring down to contain only the data that we want.  This
expression is taking each element in the original list and transforming it into
a three element sublist.  The three elements are:

* The first element in the ``Values`` list whose ``Key`` has a value of
  ``Name``.
* The ``type``
* The ``state.name`` of each instance.

The most interesting of those three expressions is the
``tags[?Key==`Name`].Values[] | [0]`` part.  Let's examine that further.

The first thing to notice is the we're filtering down the list associated
with the ``tags`` key.  The ``tags[?Key==`Name`]`` tells us to only include
list elements that contain a ``Key`` whose value is ``Name``.  From those
filtered list elements we're going to take the ``Values`` key and flatten
the list.  Finally, the ``| [0]`` will take the entire list and extract the
first element in the list.
