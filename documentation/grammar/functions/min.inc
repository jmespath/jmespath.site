.. _func-min:

min
---

::

    number min(array[number]|array[string] $collection)

Returns the lowest found number in the provided ``$collection`` argument.


.. cssclass:: table

.. list-table:: Examples
  :header-rows: 1

  * - Given
    - Expression
    - Result
  * - ``[10, 15]``
    - ``min(@)``
    - ``10``
  * - ``["a", "b"]``
    - ``min(@)``
    - ``"a"``
  * - ``["a", 2, "b"]``
    - ``min(@)``
    - ``<error: invalid-type>``
  * - ``[10, false, 20]``
    - ``min(@)``
    - ``<error: invalid-type>``


