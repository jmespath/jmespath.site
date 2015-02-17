=================
JMESPath Examples
=================


This page contains numerous examples of JMESPath examples
in action.  If you're new to JMESPath, you can start with the
:doc:`tutorial`, which goes over the basics of JMESPath.

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


Using Functions
===============

:ref:`JMESPath functions <functions>` give you a lot of power and flexibility
when working with JMESPath expressions.  Below are some common expressions and
functions used in JMESPath.

sort_by
-------

.. jpexample:: sort_by(Contents, &Date)[*].{Key: Key, Size: Size}
    :layout: 2cols-long

    {
      "Contents": [
        {
          "Date": "2014-12-21T05:18:08.000Z",
          "Key": "logs/bb",
          "Size": 303
        },
        {
          "Date": "2014-12-20T05:19:10.000Z",
          "Key": "logs/aa",
          "Size": 308
        },
        {
          "Date": "2014-12-20T05:19:12.000Z",
          "Key": "logs/qux",
          "Size": 297
        },
        {
          "Date": "2014-11-20T05:22:23.000Z",
          "Key": "logs/baz",
          "Size": 329
        },
        {
          "Date": "2014-12-20T05:25:24.000Z",
          "Key": "logs/bar",
          "Size": 604
        },
        {
          "Date": "2014-12-20T05:27:12.000Z",
          "Key": "logs/foo",
          "Size": 647
        }
      ]
    }

The first interesting thing here if the use of the function ``sort_by``.  In
this example we are sorting the ``Contents`` array by the value of each
``LastModified`` key in each element in the ``Contents`` array.  The
``sort_by`` function takes two arguments.  The first argument is an array, and
the second argument describes the key that should be used to sort the array.

The second interesting thing in this expression is that the second argument
starts ``&``, which creates an expression type.  Think of this conceptually as
a reference to an expression that can be evaluated later.  If you are familiar
with lambda and anonymous functions, expression types are similiar.  The reason
we use ``&LastModified`` instead of ``LastModified`` is because if the
expression is ``LastModified``, it would be evaluated before calling the
function, and given there's no ``LastModified`` key in the outer hash, the
second second would evaluate to ``null``.  Check out :ref:`function-evaluation`
in the specification for more information on how functions are evaluated in
JMESPath.

And finally, the last interesting thing in this expression is the ``[*]``
immediately after the ``sort_by`` function call.  The reason for this is that
we want to apply the multiselect hash, the second half of the expression, to
each element in the sorted array.  In order to do this we need a projection.
The ``[*]`` does exactly that, it takes the input array and creates a
projection such that the multiselect hash ``{Key: Key, Size: Size}`` will be
applied to each element in the list.

There are other functions that take expression types that are similar to
``sort_by`` including :ref:`func-min-by` and :ref:`func-max-by`.

Pipes
=====

Pipe expression are useful for stopping projections.  They can also be used to
group expressions.

Main Page
---------

Let's look at a modified version of the expression on the `JMESPath front page
<http://jmespath.org>`__.


.. jpexample:: locations[?state == `WA`].name | sort(@)[-2:] | {WashingtonCities: join(`, `, @)}
    :layout: 2cols-long

    {
      "locations": [
        {"name": "Seattle", "state": "WA"},
        {"name": "New York", "state": "NY"},
        {"name": "Bellevue", "state": "WA"},
        {"name": "Olympia", "state": "WA"}
      ]
    }

We can think of this JMESPath expression as having three components, each
separated by the pipe character ``|``.  The first expression is familiar to us,
it's similar to the first example on this page.  The second part of the
expression, ``sort(@)``, is similar to the ``sort_by`` function we saw in the
previous section.  The ``@`` token is used to refer to the current element.
The :ref:`func-sort` function takes a single parameter which is an array.  If
the input JSON document was a hash, and we wanted to sort the ``foo`` key,
which was an array, we could just use ``sort(foo)``.  In this scenario, the
input JSON document is the array we want to sort.  To refer to this value, we
use the current element, ``@``, to indicate this.  We're also only taking a
subset of the sorted array.  We're using a slice (``[-2:]``) to indicate that
we only want the last two elements in the sorted array to be passed through to
the final third of this expression.

And finally, the third part of the expression,
``{WashingtonCities: join(`, `, @)}``, creates a multiselect hash.  It takes as
input, the list of sorted city names, and produces a hash witih a single key,
``WashingtonCities``, whose values are the input list (denoted by ``@``) as a
string separated by a comma.
