.. _func-min-by:

min_by
------

::

    min_by(array elements, expression->number|expression->string expr)

Return the minimum element in an array using the expression ``expr`` as the
comparison key.  The entire maximum element is returned.
Below are several examples using the ``people`` array (defined above) as the
given input.


.. cssclass:: table

.. list-table:: Examples
  :header-rows: 1

  * - Expression
    - Result
  * - ``min_by(people, &age)``
    - ``{"age": 10, "age_str": "10", "bool": true, "name": 3}``
  * - ``min_by(people, &age).age``
    - ``10``
  * - ``min_by(people, &to_number(age_str))``
    - ``{"age": 10, "age_str": "10", "bool": true, "name": 3}``
  * - ``min_by(people, &age_str)``
    - ``<error: invalid-type>``
  * - ``min_by(people, age)``
    - ``<error: invalid-type>``


.. _not_null:

not_null
--------

::

    any not_null([any $argument [, any $...]])

Returns the first argument that does not resolve to ``null``.  This function
accepts one or more arguments, and will evaluate them in order until a
non null argument is encounted.  If all arguments values resolve to ``null``,
then a value of ``null`` is returned.

.. cssclass:: table

.. list-table:: Examples
  :header-rows: 1

  * - Given
    - Expression
    - Result
  * - ``{"a": null, "b": null, "c": [], "d": "foo"}``
    - ``not_null(no_exist, a, b, c, d)``
    - ``[]``
  * - ``{"a": null, "b": null, "c": [], "d": "foo"}``
    - ``not_null(a, b, `null`, d, c)``
    - ``"foo"``
  * - ``{"a": null, "b": null, "c": [], "d": "foo"}``
    - ``not_null(a, b)``
    - ``null``


