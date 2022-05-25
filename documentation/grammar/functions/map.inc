.. _func-map:

map
---

::

    array[any] map(expression->any->any expr, array[any] elements)

Apply the ``expr`` to every element in the ``elements`` array
and return the array of results.  An ``elements`` of length
N will produce a return array of length N.

Unlike a projection, (``[*].bar``), ``map()`` will include
the result of applying the ``expr`` for every element in the
``elements`` array, even if the result if ``null``.

.. cssclass:: table

.. list-table:: Examples
  :header-rows: 1

  * - Given
    - Expression
    - Result
  * - ``{"array": [{"foo": "a"}, {"foo": "b"}, {}, [], {"foo": "f"}]}``
    - ``map(&foo, array)``
    - ``["a", "b", null, null, "f"]``
  * - ``[[1, 2, 3, [4]], [5, 6, 7, [8, 9]]]``
    - ``map(&[], @)``
    - ``[[1, 2, 3, 4], [5, 6, 7, 8, 9]]``


