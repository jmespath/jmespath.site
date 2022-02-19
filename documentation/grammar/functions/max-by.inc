.. _func-max-by:

max_by
------

::

    max_by(array elements, expression->number|expression->string expr)

Return the maximum element in an array using the expression ``expr`` as the
comparison key.  The entire maximum element is returned.
Below are several examples using the ``people`` array (defined above) as the
given input.


.. cssclass:: table

.. list-table:: Examples
  :header-rows: 1

  * - Expression
    - Result
  * - ``max_by(people, &age)``
    - ``{"age": 50, "age_str": "50", "bool": false, "name": "d"}``
  * - ``max_by(people, &age).age``
    - ``50``
  * - ``max_by(people, &to_number(age_str))``
    - ``{"age": 50, "age_str": "50", "bool": false, "name": "d"}``
  * - ``max_by(people, &age_str)``
    - ``<error: invalid-type>``
  * - ``max_by(people, age)``
    - ``<error: invalid-type>``


