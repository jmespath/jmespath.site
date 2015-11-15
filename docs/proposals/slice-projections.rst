=================
Slice Projections
=================

:JEP: 10
:Author: James Saryerwinnie
:Status: accepted
:Created: 08-Feb-2015

Abstract
========

This document proposes modifying the semantics of slice expressions to
create projections, which brings consistency with the wildcard,
flattening, and filtering projections.


Motivation
==========

JEP 5 introduced slice expressions.  This added python slice semantics
to JSON.  Slicing does not produce a projection so expressions such as
the following will always return ``null``:  ``myarray[:10].foo.bar``.

Instead if you wanted to access ``foo.bar``  for each element in the
array slice you currently have to write ``myarray[:10][*].foo.bar``.

This JEP proposes that a slice expression will create a projection.


Rationale
---------

A reasonable objection to this JEP is that this is unnecessary because, as
shown in the example above, you can take any slice and create a projection via
``[*]``.  This is entirely true, unlike other JEPs, this JEP does not enable
any behavior that was previously not possible.

Instead, the main reason for this JEP is for consistency.  Right now there are
three types of array projections:

* List Projections (``foo[*].bar``)
* Filter Projections (``foo[?a==b].bar``)
* Flatten Projections (``foo[].bar``)

Note the general form, ``foo[<stuff here>].<child-expr>``.  Each of the
existing array projections have the same general semantics:

* Take the left hand side, which is a list, and produce another list as a
  result of evaluating the left hand side.  This newly produced list will
  contain elements of the original input (or elements of the elements of
  the original input in the case of the flatten projection).
* Evaluate the right hand side against each element in the list produced
  from evaluating the left hand side.

So in general, the left hand side is responsible for creating a new list
but not for manipulating individual elements of the list.  The right hand
side is for manipulating individual elements of the list.  In the case
of the list projection, every element from the original list is used.
In the case of a filter projection, only elements matching an expression
are passed to the right hand side.  In the case of a flatten projection,
sub arrays are merged before passing the expression onto the right hand
side.

It's a reasonable expectation that slices behave similar.  After all,
slices take an array and produce a sub array.  It many ways, it's very
similar to filter projections.  While filter projections only include
elements that match a particular expression, slice projections
only include elements from and to a specific index.  Given its semantics
are so close to the filter projections, slices should create projections
to be consistent.


Specification
=============

Whenver a slice is created, a projection will be created. This will be the
fourth type of array projection in JMESPath.  In addition to the existing array
projections:

* List Projections
* Flatten Projections
* Filter Projections

A new projection type, the slice projection will be added.  A slice projection
is evaluated similar to the other array projections.  Given a slice projection
which contains a left hand side containing the slice expression and a right
hand side, the slice expression is evaluated to create a new sub array, and
each expression on the right hand side is evaluted against each element from
the array slice to create the final result.

This JEP does not include any modifications to the JMESPath grammar.


Impact
======

The impact to existing users of slices is minimal.  Consider:

* Existing expressions such as ``foo[:10].bar`` are currently returning
  ``null``.  Now they will return non ``null`` values.
* The only impact to existing users is if someone had an expression such as
  ``foo[:10][0]``, which given the projection semantics will now create a list
  containing the 0th element from each sublist.  Before this JEP, that
  expression is equivalent to ``foo[0]`` so the slice is unnecessary.  And any
  users that actually had expressions like this can now just use ``foo[0]``
  instead.
