.. _func-contains:

contains
--------

::

    boolean contains(array|string $subject, any $search)

Returns ``true`` if the given ``$subject`` contains the provided ``$search``
string.

If ``$subject`` is an array, this function returns true if one of the elements
in the array is equal to the provided ``$search`` value.

If the provided ``$subject`` is a string, this function returns true if
the string contains the provided ``$search`` argument.

.. cssclass:: table

.. list-table:: Examples
  :header-rows: 1

  * - Given
    - Expression
    - Result
  * - n/a
    - ``contains(`foobar`, `foo`)``
    - ``true``
  * - n/a
    - ``contains(`foobar`, `not`)``
    - ``false``
  * - n/a
    - ``contains(`foobar`, `bar`)``
    - ``true``
  * - n/a
    - ``contains(`false`, `bar`)``
    - ``<error: invalid-type>``
  * - n/a
    - ``contains(`foobar`, 123)``
    - ``false``
  * - ``["a", "b"]``
    - ``contains(@, `a`)``
    - ``true``
  * - ``["a"]``
    - ``contains(@, `a`)``
    - ``true``
  * - ``["a"]``
    - ``contains(@, `b`)``
    - ``false``
  * - ``["foo", "bar"]``
    - ``contains(@, `foo`)``
    - ``true``
  * - ``["foo", "bar"]``
    - ``contains(@, `b`)``
    - ``false``


