=================
JMESPath Tutorial
=================

This is a guide through the JMESPath language.  JMESPath is a query language
for JSON.  You can extract and transform elements from a JSON document.
The examples below are interactive.  You can change the JMESPath expressions
and see the results update automatically.

Basic Expressions
=================

The simplest JMESPath expression is an :ref:`identifier <identifiers>`, which
selects a key in an JSON object:

.. jpexample:: a
    :layout: 2cols
    :cols: 40
    :rows: 5

    {"a": "foo", "b": "bar", "c": "baz"}

Try changing the expression above to ``b``, and ``c`` and note the updated
result.  Also note that if you refer to a key that does not exist, a value of
``null`` (or the language equivalent of ``null``) is returned.

You can use a :ref:`subexpression <subexpressions>` to return to nested values
in a JSON object:


.. jpexample:: a.b.c.d
    :layout: 2cols
    :cols: 40
    :rows: 5

    {"a": {"b": {"c": {"d": "value"}}}}

If you refer to a key that does not exist, a value of ``null`` is returned.
Attempting to subsequently access identifiers will continue to return a value
of ``null``.  Try changing the expression to ``b.c.d.e`` above.

:ref:`Index Expressions <indexexpressions>` allow you to select a specific
element in a list.  It should look similar to array access in common
programming languages.  Indexing is 0 based.

.. jpexample:: [1]
    :layout: 2cols
    :cols: 40
    :rows: 5

    ["a", "b", "c", "d", "e", "f"]

If you specify an index that's larger than the list, a value of
``null`` is returned.  You can also use negative indexing to index
from the end of the list.  ``[-1]`` refers to the last element
in the list, ``[-2]`` refers to the penultimate element.  Try it out
in the example above.

You can combine identifiers, sub expressions, and index expressions to
access JSON elements.

.. jpexample:: a.b.c[0].d[1][0]
    :layout: 2cols
    :cols: 40
    :rows: 10

    {"a": {
      "b": {
        "c": [
          {"d": [0, [1, 2]]},
          {"d": [3, 4]}
        ]
      }
    }}

Projections
===========

Projections are one of the key features of JMESPath.  It allows you
to apply an expression to a collection of elements.  There are four kinds of
projections:

* List Projections
* Object Projections
* Flatten Projections
* Filter Projections

List Projections
----------------

A :ref:`wildcard expression <wildcards>`  creates a list projection, which is a
projection over a JSON array.  This is best illustrated with an example.
Let's say we have a JSON document describing a people, and  each array element
is a JSON object that has a ``first``, ``last``, and ``age`` key.  Suppose
we wanted a list of all the first names in our list.


.. jpexample:: people[*].first
    :layout: 2cols
    :cols: 50
    :rows: 10

    {
      "people": [
        {"first": "James", "last": "d"},
        {"first": "Jacob", "last": "e"},
        {"first": "Jayden", "last": "f"},
        {"missing": "different"}
      ],
      "foo": {"bar": "baz"}
    }

In the example above, the ``first`` expression, which is just an identifier, is
applied to each element in the ``people`` array.  The results are collected
into a JSON array and returned as the result of the expression.  The expression
can be more complex than a basic ``identifier``.  For example, the expression
``foo[*].bar.baz[0]`` would project the ``bar.baz[0]`` expression to each
element in the ``foo`` array.

There's a few things to keep in mind when working with projections.  These are
discussed in more detail in the :ref:`wildcard expressions <wildcards>` section
of the spec, but the main points are:

* Projections are evaluated as two steps.  The left hand side (LHS) creates a
  JSON array of initial values.  The right hand side (RHS) of a projection is
  the expression to project for each element in the JSON array created by the
  left hand side.  Each projection type has slightly different semantics when
  evaluating either the left hand side and/or the right hand side.
* If the result of the expression projected onto an individual array element is
  ``null``, then that value is omitted from the collected set of results.
* You can stop a projection with a Pipe Expression (discussed later).
* A list projection is only valid for a JSON array.  If the value is not a
  list, then the result of the expression is ``null``.

You can try this out in the demo above.  Notice how  ``people[*].first`` only
included three elements, even though the people array has four elements.
This is because the last element, ``{"missing": "different"}`` evalues to
``null`` when the expression ``first`` is applied, and ``null`` values are not
added to the collected result array.  If you try the expression ``foo[*].bar``
you'll see a result of ``null``, because the value associated with the ``foo``
key is a JSON object, not an array, and a list projection is only defined for
JSON arrays.

Object Projections
------------------

Whereas a list projection is defined for a JSON array, an object projection is
defined for a JSON object.  You can create an object projection using the ``*``
syntax.  This will create a list of the values of the JSON object, and project
the right hand side of the projection onto the list of values.

.. jpexample:: ops.*.numArgs
    :layout: 2cols
    :cols: 50
    :rows: 10

    {
      "ops": {
        "functionA": {"numArgs": 2},
        "functionB": {"numArgs": 3},
        "functionC": {"variadic": true}
      }
    }

In the example above the ``*`` creates a JSON array of the values associated
with the ``ops`` JSON object.  The RHS of the projection, ``numArgs``, is then
applied to the JSON array, resulting in the final array of ``[2, 3]``.  Below
is a sample walkthrough of how an implementation could *potentially* implement
evaluating an object projection.  First, the object projection can be broken
down into its two components, the left hand side (LHS) and its right hand side
(RHS):

* **LHS**: ``ops``
* **RHS**: ``numArgs``

First, the LHS is evaluated to create the initial array to be projected::

    evaluate(ops, inputData) -> [{"numArgs": 2}, {"numArgs": 3},
                                 {"variadic": True}]

Then the RHS is applied to each element in the array::

    evaluate(numArgs, {numArgs: 2}) -> 2
    evaluate(numArgs, {numArgs: 3}) -> 3
    evaluate(numArgs, {variadic: true}) -> null

Any ``null`` values are not included in the final result, so the result of the
entire expression is therefore ``[2, 3]``.


Flatten Projections
-------------------

More than one projection can be used in a JMESPath expression.  In the case of
a List/Object projection, the structure of the original document is preserved
when creating projection within a projection.  For example, let's take
the expression ``reservations[*].instances[*].state``.  This expression
is saying that the top level key ``reservations`` has an array as a value.  For
each of those array elements, project the ``instances[*].state`` expression.
Within each list element, there's an ``instances`` key which itself is a value,
and we create a sub projection for each each list element in the list.
Here's an example of that:

.. jpexample:: reservations[*].instances[*].state
    :layout: 2cols
    :cols: 50
    :rows: 20

    {
      "reservations": [
        {
          "instances": [
            {"state": "running"},
            {"state": "stopped"}
          ]
        },
        {
          "instances": [
            {"state": "terminated"},
            {"state": "runnning"}
          ]
        }
      ]
    }

The result of this expression is ``[["running", "stopped"], ["terminated",
"running"]]``, which is a list of lists.  The outer list is from the
projection of ``reservations[*]``, and the inner list is
a projection of ``state`` created from ``instances[*]``::

    1st       r0                         r1
    2nd i0          i1             i0            i1
    [["running", "stopped"], ["terminated", "running"]]

What if we just want a list of all the states of our instances?  We'd ideally
like a result ``["running", "stopped", "terminated", "running"]``.  In this
situation, we don't care which reservation the instance belonged to, we just
want a list of states.

This is the problem that a :ref:`Flatten Projection <flatten>` solves. To get
the desired result, you can use ``[]`` instead of ``[*]`` to flatten a list:
``reservations[].instances[].state``.  Try changing ``[*]`` to ``[]`` in the
expression above and see how the result changes.

While the :ref:`spec <flatten>` goes into more detail, a simple rule of thumb to use
for the flatten operator, ``[]``, is that:

* It flattens sublists into the parent list (not recursively, just one level).
* It creates a projection, so anything on the RHS of the flatten projection is
  projected onto the newly created flattened list.

You can also just use ``[]`` on its own to flatten a list:

.. jpexample:: []
    :layout: 2cols
    :cols: 50

    [
      [0, 1],
      2,
      [3],
      4,
      [5, [6, 7]]
    ]

If you flattened the result of the expression again, ``[][]``, you'd then get a
result of ``[0, 1, 2, 3, 4, 5, 6, 7]``.  Try it out in the example above.


Filter Projections
------------------

Up to this point we've looked at:

* List projections
* Object projections
* Flatten projections

Evaluating the RHS of a projection is a basic type of filter.  If the result of
the expression evaluated against an individual element results in ``null``, then
the element is excluded from the final result.

A filter projection allows you to filter the LHS of the projection *before*
evaluating the RHS of a projection.

For example, let's say we have a list of machines, each has a ``name`` and a
``state``.  We'd like the name of all machines that are running.
In pseudocode, this would be::

    result = []
    foreach machine in inputData['machines']
      if machine['state'] == 'running'
        result.insert_at_end(machine['name'])
    return result

A filter projection can be used to accomplish this:

.. jpexample:: machines[?state==`running`].name
    :layout: 2cols
    :cols: 50

    {
      "machines": [
        {"name": "a", "state": "running"},
        {"name": "b", "state": "stopped"},
        {"name": "b", "state": "running"}
      ]
    }

Try changing ``running`` to ``stopped`` in the example above.  You can also
remove the ``.name`` at the end of the expression if you just want the entire
JSON object of each machine that has the specified state.

A filter expression is defined for an array and has the general form
``LHS [? <expression> <comparator> <expression>] RHS``.  The
:ref:`filter expression <filterexpressions>` spec details exactly what
comparators are available and how they work, but the standard comparators are
supported, i.e ``==, !=, <, <=, >, >=``.


Pipe Expressions
================

Projections are an important concept in JMESPath.  However, there are times
when projection semantics are *not* what you want.  A common scenario is when
you want to operate of the *result* of a projection rather than projecting an
expression onto each element in the array.  For example, the expression
``people[*].first`` will give you an array containing the first names of
everyone in the people array.  What if you wanted the first element in that
list?  If you tried ``people[*].first[0]`` that you just evaluate ``first[0]``
for each element in the people array, and because indexing is not defined for
strings, the final result would be an empty array, ``[]``.  To accomplish the
desired result, you can use a pipe expression, ``<expression> | <expression>``,
to indicate that a projection must stop.  This is shown in the example below:


.. jpexample:: people[*].first | [0]
    :layout: 2cols
    :cols: 50
    :rows: 10

    {
      "people": [
        {"first": "James", "last": "d"},
        {"first": "Jacob", "last": "e"},
        {"first": "Jayden", "last": "f"},
        {"missing": "different"}
      ],
      "foo": {"bar": "baz"}
    }

In the example above, the RHS of the list projection is ``first``.  When a pipe
is encountered, the result up to that point is passed to the RHS of the pipe
expression.  The pipe expression is evaluated as::

    evaluate(people[*].first, inputData) -> ["James", "Jacob", "Jayden"]
    evaluate([0], ["James", "Jacob", "Jayden"]) -> "James"


MultiSelect
===========

Up to this point, we've looked at JMESPath expressions that help to pare down a
JSON document into just the elements you're interested in.  This next concept,
:ref:`multiselect lists <multiselectlist>` and
:ref:`multiselect hashes <multiselecthash>` allow you to create JSON elements.
This allows you to create elements that don't exist in a JSON document.  A
multiselect list creates a list and a multiselect hash creates a JSON object.

This is an example of a multiselect list:

.. jpexample:: people[].[name, state.name]
    :layout: 2cols
    :cols: 50
    :rows: 20

    {
      "people": [
        {
          "name": "a",
          "state": {"name": "up"}
        },
        {
          "name": "b",
          "state": {"name": "down"}
        },
        {
          "name": "c",
          "state": {"name": "up"}
        }
      ]
    }

In the expression above, the ``[name, state.name]`` portion is a multiselect
list.  It says to create a list of two element, the first element is the result
of evaluating the ``name`` expression against the list element, and the second
element is the result of evaluating ``state.name``.  Each list element will
therefore create a two element list, and the final result of the entire
expression is a list of two element lists.

Unlike a projection, the result of the expression in always included, even if
the result is a null.  If you change the above expression to ``people[].[foo,
bar]`` each two element list will be ``[null, null]``.

A multiselect has the same basic idea of a multiselect list, except it instead
creates a hash instead of an array.  Using the same example above, if we
instead wanted to create a two element hash that had two keys, ``Name`` and
``State``, we could use this:

.. jpexample:: people[].{Name: name, State: state.name}
    :layout: 2cols
    :cols: 50
    :rows: 20

    {
      "people": [
        {
          "name": "a",
          "state": {"name": "up"}
        },
        {
          "name": "b",
          "state": {"name": "down"}
        },
        {
          "name": "c",
          "state": {"name": "up"}
        }
      ]
    }

Next Steps
==========

We've now seen an overview of the JMESPath language.  You can also check out:

* The :ref:`JMESPath Spec <spec>`, which has the official ABNF grammar and full
  details of the semantics of the language.
