.. _func-abs:

abs
---

::

    number abs(number $value)

Returns the absolute value of the provided argument.  The signature indicates
that a number is returned, and that the input argument ``$value`` **must**
resolve to a number, otherwise a ``invalid-type`` error is triggered.

Below is a worked example.  Given::

    {"foo": -1, "bar": "2"}

Evaluating ``abs(foo)`` works as follows:

1. Evaluate the input argument against the current data::

     search(foo, {"foo": -1, "bar": "2"}) -> -1

2. Validate the type of the resolved argument.  In this case
   ``-1`` is of type ``number`` so it passes the type check.

3. Call the function with the resolved argument::

     abs(-1) -> 1

4. The value of ``1`` is the resolved value of the function expression
   ``abs(foo)``.


Below is the same steps for evaluating ``abs(bar)``:

1. Evaluate the input argument against the current data::

     search(bar, {"foo": -1, "bar": "2"}) -> "2"

2. Validate the type of the resolved argument.  In this case
   ``"2"`` is of type ``string`` so we immediately indicate that
   an ``invalid-type`` error occurred.


As a final example, here is the steps for evaluating ``abs(to_number(bar))``:

1. Evaluate the input argument against the current data::

    search(to_number(bar), {"foo": -1, "bar": "2"})

2. In order to evaluate the above expression, we need to evaluate
   ``to_number(bar)``::

    search(bar, {"foo": -1, "bar": "2"}) -> "2"
    # Validate "2" passes the type check for to_number, which it does.
    to_number("2") -> 2

   Note that `to_number`_ is defined below.

3. Now we can evaluate the original expression::

    search(to_number(bar), {"foo": -1, "bar": "2"}) -> 2

4. Call the function with the final resolved value::

    abs(2) -> 2

5. The value of ``2`` is the resolved value of the function expression
   ``abs(to_number(bar))``.


.. cssclass:: table

.. list-table:: Examples
  :header-rows: 1

  * - Expression
    - Result
  * - ``abs(1)``
    - ``1``
  * - ``abs(-1)``
    - ``1``
  * - ``abs(`abc`)``
    - ``<error: invalid-type>``


