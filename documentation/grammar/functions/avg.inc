.. _func-avg:

avg
---

::

    number avg(array[number] $elements)

Returns the average of the elements in the provided array.

An empty array will produce a return value of null.

.. cssclass:: table

.. list-table:: Examples
  :header-rows: 1

  * - Given
    - Expression
    - Result
  * - ``[10, 15, 20]``
    - ``avg(@)``
    - ``15``
  * - ``[10, false, 20]``
    - ``avg(@)``
    - ``<error: invalid-type>``
  * - ``[false]``
    - ``avg(@)``
    - ``<error: invalid-type>``
  * - ``false``
    - ``avg(@)``
    - ``<error: invalid-type>``


