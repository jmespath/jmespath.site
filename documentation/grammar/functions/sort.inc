.. _func-sort:

sort
----

::

    array sort(array[number]|array[string] $list)

This function accepts an array ``$list`` argument and returns the sorted
elements of the ``$list`` as an array.

The array must be a list of strings or numbers.  Sorting strings is based on
code points.  Locale is not taken into account.



.. cssclass:: table

.. list-table:: Examples
  :header-rows: 1

  * - Given
    - Expression
    - Result
  * - ``[b, a, c]``
    - ``sort(@)``
    - ``[a, b, c]``
  * - ``[1, a, c]``
    - ``sort(@)``
    - ``[1, a, c]``
  * - ``[false, [], null]``
    - ``sort(@)``
    - ``[[], null, false]``
  * - ``[[], {}, false]``
    - ``sort(@)``
    - ``[{}, [], false]``
  * - ``{"a": 1, "b": 2}``
    - ``sort(@)``
    - ``null``
  * - ``false``
    - ``sort(@)``
    - ``null``


