.. _func-sort-by:

sort_by
-------

::

    sort_by(array elements, expression->number|expression->string expr)

Sort an array using an expression ``expr`` as the sort key.  For each element
in the array of ``elements``, the ``expr`` expression is applied and the
resulting value is used as the key used when sorting the ``elements``.

If the result of evaluating the ``expr`` against the current array element
results in type other than a ``number`` or a ``string``, a type error will
occur.

Below are several examples using the ``people`` array (defined above) as the
given input.  ``sort_by`` follows the same sorting logic as the ``sort``
function.


.. cssclass:: table

.. list-table:: Examples
  :header-rows: 1

  * - Expression
    - Result
  * - ``sort_by(people, &age)[].age``
    - ``[10, 20, 30, 40, 50]``
  * - ``sort_by(people, &age)[0]``
    - ``{"age": 10, "age_str": "10", "bool": true, "name": 3}``
  * - ``sort_by(people, &to_number(age_str))[0]``
    - ``{"age": 10, "age_str": "10", "bool": true, "name": 3}``


