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
here are filters and multiselects.  In this example below, we're taking the
array of people and, for any element with an age key whose value is greater
than 20, we're creating a sub list of the name and age values.


.. jpexample:: people[?age > `20`].[name, age]
    :layout: 2cols-expand

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

In the previous example we were taking an array of hashes, and simplifying down
to an array of two element arrays containing a name and an age.  We're also
only including list elements where the ``age`` key is greater than ``20``.  If
instead we want to create the same hash structure but only include the ``age``
and ``name`` key, we can instead say:


.. jpexample:: people[?age > `20`].{name: name, age: age}
    :layout: 2cols-expand

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
    :layout: 2cols-expand

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


Notice in the above example instead of applying a filter expression
(``[? <expr> ]``), we're selecting all array elements via ``[*]``.


Working with Nested Data
========================


.. jpexample:: reservations[].instances[].[tags[?Key=='Name'].Values[] | [0], type, state.name]
    :layout: 2cols-expand

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
of instances.  By using the :ref:`flatten` we can take the two instances from
the first list and the two instances from the second list, and combine them
into a single list.  Try changing the above expression to just
``reservations[].instances[]`` to see what this flattened list looks like.
Everything to the right of the ``reservations[].instances[]`` is about taking
the flattened list and paring it down to contain only the data that we want.
This expression is taking each element in the original list and transforming it
into a three element sublist.  The three elements are:

* In the ``tags`` list, select the first element in the flattened ``Values``
  list whose ``Key`` has a value of ``Name``.
* The ``type``
* The ``state.name`` of each instance.

The most interesting of those three expressions is the
``tags[?Key=='Name'].Values[] | [0]`` part.  Let's examine that further.

The first thing to notice is the we're filtering down the list associated
with the ``tags`` key.  The ``tags[?Key==`Name`]`` tells us to only include
list elements that contain a ``Key`` whose value is ``Name``.  From those
filtered list elements we're going to take the ``Values`` key and flatten
the list.  Finally, the ``| [0]`` will take the entire list and extract the
0th element.


Filtering and Selecting Nested Data
----------------------------------------

In this example, we're going to look at how you can filter nested hashes.


.. jpexample:: people[?general.id==`100`].general | [0]
    :layout: 2cols-expand

    {
      "people": [
        {
          "general": {
            "id": 100,
            "age": 20,
            "other": "foo",
            "name": "Bob"
          },
          "history": {
            "first_login": "2014-01-01",
            "last_login": "2014-01-02"
          }
        },
        {
          "general": {
            "id": 101,
            "age": 30,
            "other": "bar",
            "name": "Bill"
          },
          "history": {
            "first_login": "2014-05-01",
            "last_login": "2014-05-02"
          }
        }
      ]
    }

In this example we're searching through the ``people`` array.  Each element in
this array contains a hash of two elements, and each value in the hash is
itself a hash.  We're trying to retrieve the value of the ``general`` key
that contains an ``id`` key with a value of ``100``.

If we just had the expression ``people[?general.id==`100`]``, we'd have a
result of::

    [{
      "general": {
        "id": 100,
        "age": 20,
        "other": "foo",
        "name": "Bob"
      },
      "history": {
        "first_login": "2014-01-01",
        "last_login": "2014-01-02"
      }
    }]

Let's walk through how we arrived at this result.  In words, the
``people[?general.id==`100`]`` expression is saying "for each element in the
people array, select the elements where the ``general.id`` equals ``100``".
If we trace the execution of this filtering process we have::

    # First element:
        {
          "general": {
            "id": 100,
            "age": 20,
            "other": "foo",
            "name": "Bob"
          },
          "history": {
            "first_login": "2014-01-01",
            "last_login": "2014-01-02"
          }
        },
    # Applying the expression ``general.id`` to this hash::
        100
    # Does 100==100?
        true
    # Add this first element (in its entirety) to the result list.

    # Second element:
        {
          "general": {
            "id": 101,
            "age": 30,
            "other": "bar",
            "name": "Bill"
          },
          "history": {
            "first_login": "2014-05-01",
            "last_login": "2014-05-02"
          }
        }

    # Applying the expression ``general.id`` to this element::
        101
    # Does 101==100?
        false
    # Do not add this element to the results list.
    # Result of this expression is a list containing the first element.


However, this still isn't the final value we want which is::

      {
        "id": 100,
        "age": 20,
        "other": "foo",
        "name": "Bob"
      }

In order to get to this value from our filtered results we need to first
select the ``general`` key.  This gives us a list of just the values of the
``general`` hash::

      [{
        "id": 100,
        "age": 20,
        "other": "foo",
        "name": "Bob"
      }]

From there, we then uses a pipe (``|``) to stop projections so that we can
finally select the first element (``[0]``).  Note that we are making the
assumption that there's only one hash that contains an ``id`` of ``100``.
Given the way the data is structured, it's entirely possible to have data such
as::

    {
      "people": [
        {
          "general": {
            "id": 100,
            "age": 20
          },
          "history": {
          }
        },
        {
          "general": {
            "id": 101,
            "age": 30
          },
          "history": {
          }
        },
        {
          "general": {
            "id": 100,
            "age": 30
          },
          "history": {
          }
        }
      ]
    }

Note here that the first and last elements in the ``people`` array both have an
``id`` of ``100``.  Our expression would then select the first element that
matched.

Finally, it's worth mentioning there is more than one way to write this
expression.  In this example we've decided that after we filter the list we're
going to select the value of the ``general`` key and then select the first
element in that list.  We could also reverse the order of those operations, we
could have taken the filtered list, selected the first element, and then
extracted the value associated with the ``general`` key.  That expression
would be::

    people[?general.id==`100`] | [0].general

Both versions are equally valid.


Using Functions
===============

:ref:`JMESPath functions <functions>` give you a lot of power and flexibility
when working with JMESPath expressions.  Below are some common expressions and
functions used in JMESPath.

sort_by
-------

.. jpexample:: sort_by(Contents, &Date)[*].{Key: Key, Size: Size}
    :layout: 2cols-expand

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
starts with ``&``, which creates an expression type.  Think of this
conceptually as a reference to an expression that can be evaluated later.  If
you are familiar with lambda and anonymous functions, expression types are
similiar.  The reason we use ``&LastModified`` instead of ``LastModified`` is
because if the expression is ``LastModified``, it would be evaluated before
calling the function, and given there's no ``LastModified`` key in the outer
hash, the second second would evaluate to ``null``.  Check out
:ref:`function-evaluation` in the specification for more information on how
functions are evaluated in JMESPath.  Also, note that we're taking advantage of
the fact that the dates are in ISO 8601 format, which can be sorted
lexicographically.

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
<https://jmespath.org>`__.


.. jpexample:: locations[?state == 'WA'].name | sort(@)[-2:] | {WashingtonCities: join(', ', @)}
    :layout: 2cols-expand

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
``{WashingtonCities: join(', ', @)}``, creates a multiselect hash.  It takes as
input, the list of sorted city names, and produces a hash witih a single key,
``WashingtonCities``, whose values are the input list (denoted by ``@``) as a
string separated by a comma.
