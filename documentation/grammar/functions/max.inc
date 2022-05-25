.. _func-max:

max
---

::

    number max(array[number]|array[string] $collection)

Returns the highest found number in the provided array argument.

An empty array will produce a return value of null.

.. cssclass:: table

.. list-table:: Examples
  :header-rows: 1

  * - Given
    - Expression
    - Result
  * - ``[10, 15]``
    - ``max(@)``
    - ``15``
  * - ``["a", "b"]``
    - ``max(@)``
    - ``"b"``
  * - ``["a", 2, "b"]``
    - ``max(@)``
    - ``<error: invalid-type>``
  * - ``[10, false, 20]``
    - ``max(@)``
    - ``<error: invalid-type>``


