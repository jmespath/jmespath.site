.. _func-sum:

sum
---

::

    number sum(array[number] $collection)

Returns the sum of the provided array argument.

An empty array will produce a return value of 0.

.. list-table:: Examples
  :header-rows: 1

  * - Given
    - Expression
    - Result
  * - ``[10, 15]``
    - ``sum(@)``
    - ``25``
  * - ``[10, false, 20]``
    - ``max(@)``
    - ``<error: invalid-type>``
  * - ``[10, false, 20]``
    - ``sum([].to_number(@))``
    - ``30``
  * - ``[]``
    - ``sum(@)``
    - ``0``


