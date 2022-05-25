.. _func-length:

length
------

::

    number length(string|array|object $subject)

Returns the length of the given argument using the following types rules:

1. string: returns the number of code points in the string
2. array: returns the number of elements in the array
3. object: returns the number of key-value pairs in the object

.. cssclass:: table

.. list-table:: Examples
  :header-rows: 1

  * - Given
    - Expression
    - Result
  * - n/a
    - ``length(`abc`)``
    - ``3``
  * - "current"
    - ``length(@)``
    - ``7``
  * - "current"
    - ``length(not_there)``
    - ``<error: invalid-type>``
  * - ``["a", "b", "c"]``
    - ``length(@)``
    - ``3``
  * - ``[]``
    - ``length(@)``
    - ``0``
  * - ``{}``
    - ``length(@)``
    - ``0``
  * - ``{"foo": "bar", "baz": "bam"}``
    - ``length(@)``
    - ``2``

