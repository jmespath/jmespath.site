===============
Lexical Scoping
===============

:JEP: 11
:Author: James Saryerwinnie
:Status: draft
:Created: 24-Feb-2015

Abstract
========

This JEP proposes a new function ``let()`` (originally proposed by Michael
Dowling) that allows for evaluating an expression with an explicitly defined
lexical scope.  This will require some changes to the lookup semantics in
JMESPath to introduce scoping, but provides useful functionality such as being
able to refer to elements defined outside of the current scope used to evaluate
an expression.


Motivation
==========

As a JMESPath expression is being evaluated, the current element, which can be
explicitly referred to via the ``@`` token, changes as expressions are
evaluated.  Given a simple sub expression such as ``foo.bar``, first the
``foo`` expression is evaluted with the starting input JSON document, and the
result of that expression is then used as the current element when the ``bar``
element is evaluted.  Conceptually we're taking some object, and narrowing down
its current element as the expression is evaluted.

Once we've drilled down to a specific current element, there is no way, in the
context of the currently evaluated expression, to refer to any elements outside
of that element.  One scenario where this is problematic is being able to refer
to a parent element.

For example, suppose we had this data::

  {"first_choice": "WA",
   "states": [
     {"name": "WA", "cities": ["Seattle", "Bellevue", "Olympia"]},
     {"name": "CA", "cities": ["Los Angeles", "San Francisco"]},
     {"name": "NY", "cities": ["New York City", "Albany"]},
   ]
  }

Let's say we wanted to get the list of cities of the state corresponding to our
``first_choice`` key.  We'll make the assumption that the state names are
unique in the ``states`` list.  This is currently not possible with JMESPath.
In this example we can hard code the state ``WA``::

  states[?name==`WA`].cities

but it is not possible to base this on a value of ``first_choice``, which
comes from the parent element.  This JEP proposes a solution that makes
this possible in JMESPath.


Specification
=============

There are two components to this JEP, a new function, ``let()``, and a change
to the way that identifiers are resolved.


The let() Function
------------------

The ``let()`` function is heavily inspired from the ``let`` function commonly
seen in the Lisp family of languages:

* https://clojuredocs.org/clojure.core/let
* http://docs.racket-lang.org/guide/let.html

The let function is defined as follows::

  any let(object scope, expression->any expr)

``let`` is a function that takes two arguments.  The first argument is a JSON
object.  This hash defines the names and their corresponding values that will
be accessible to the expression specified in the second argument.  The second
argument is an expression reference that will be evaluated.

Resolving Identifiers
---------------------

Prior to this JEP, identifiers are resolved by consulting the current context
in which the expression is evaluted.  For example, using the same
``search`` function as defined in the JMESPath specification, the
evaluation of::

    search(foo, {"foo": "a", "bar": "b"}) -> "a"

will result in the ``foo`` identifier being resolved in the context of
the input object ``{"foo": "a", "bar": "b"}``.  The context object defines
``foo`` as ``a``, which results in the identifier ``foo`` being resolved as
``a``.

In the case of a sub expression, where the current evaluation context
changes once the left hand side of the sub expression is evaluted::


    search(a.b, {"a": {"b": "y"}) -> "y"

The identifier ``b`` is resolved with a current context of
``{"b": "y"}``, which results in a value of ``y``.

This JEP adds an additional step to resolving identifiers.  In addition
to the implicit evaluation context that changes based on the result
of continually evaluating expressions, the ``let()`` command allows
for additional contexts to be specified, which we refer to by the common
name scope.  The steps for resolving an identifier are:

* Attempt to lookup the identifier in the current evaluation context.
* If this identifier is not resolved, look up the value in the current
  scope provided by the user.
* If the idenfitier is not resolved and there is a parent scope, attempt
  to resolve the identifier in the parent scope.  Continue doing this until
  there is no parent scope, in which case, if the identifier has not been
  resolved, the identifier is resolved as ``null``.

Parent scopes are created by nested ``let()`` calls.

Below are a few examples to make this more clear.  First, let's
examine the case where the identifier can be resolved from the
current evaluation context::

    search(let({a: `x`}, &b), {"b": "y"}) -> "y"

In this scenario, we are evaluating the expression ``b``, with the
context object of ``{"b": "y"}``.  Here ``b`` has a value of ``y``,
so the result of this function is ``y``.

Now let's look at an example where an identifier is resolved from
a scope object provided via ``let()``::


    search(let({a: `x`}, &a, {"b": "y"})) -> "x"

Here, we're trying to resolve the ``a`` identifier.  The current
evaluation context, ``{"b": "y"}``, does not define ``a``.  Normally,
this would result in the identifier being resolved as ``null``::

    search(a, {"b": "y"}) -> null

However, we now fall back to looking in the provided scope object ``{"a":
"x"}``, which was provided as the first argument to ``let``.  Note here that
the value of ``a`` has a value of ``"x"``, so the identifier is resolved as
``"x"``, and the return value of the ``let()`` function is ``"x"``.

Finally, let's look at an example of parent scopes.  Consider the
following expression::


    search(let({a: `x`}, &let({b: `y`}, &{a: a, b: b, c: c})),
           {"c": "z"}) -> {"a": "x", "b": "y", "c": "z"}

Here we have nested let calls, and the expression we are trying to
evaluate is the multiselect hash ``{a: a, b: b, c: c}``.  The
``c`` identifier comes from the evaluation context ``{"c": "z"}``.
The ``b`` identifier comes from the scope object in the second ``let``
call: ``{b: `y`}``.  And finally, here's the lookup process for the
``a`` identifier:

* Is ``a`` defined in the current evaluation context?  No.
* Is ``a`` defined in the scope provided by the user?  No.
* Is there a parent scope?  Yes
* Does the parent scope, ``{a: `x`}``, define ``a``?  Yes, ``a`` has
  the value of ``"x"``, so ``a`` is resolved as the string ``"x"``.


Current Node Evaluation
-----------------------

While the JMESPath specification defines how the current node is determined,
it is worth explicitly calling out how this works with the ``let()`` function
and expression references.  Consider the following expression::

    a.let({x: `x`}, &b.let({y: `y`}, &c))

Given the input data::

    {"a": {"b": {"c": "foo"}}}

When the expression ``c`` is evaluated, the current evaluation context is
``{"c": "foo"}``.  This is because this expression isn't evaluated until
the second ``let()`` call evaluates the expression, which does not
occur until the first ``let()`` function evaluates the expression.

Motivating Example
------------------

With these changes defined, the expression in the "Motivation" section can be
be written as::

  let({first_choice: first_choice}, &states[?name==first_choice].cities)

Which evalutes to ``["Seattle", "Bellevue", "Olympia"]``.


Rationale
=========

If we just consider the feature of being able to refer to a parent element,
this approach is not the only way to accomplish this.  We could also allow
for explicit references using a specific token, say ``$``.
The original example in the "Motivation" section would be::

  states[?name==$.first_choice].cities

While this could work, this has a number of downsides, the biggest one being
that you'll need to always keep track of the parent element.  You don't know
ahead of time if you're going to need the parent element, so you'll always need
to track this value.  It also doesn't handle nested lexical scopes.  What if
you wanted to access a value in the grand parent element?  Requiring an
explicit binding approach via ``let()`` handles both these cases, and doesn't
require having to track parent elements.  You only need to track additional
scope when ``let()`` is used.
