.. _func-values:

values
------

::

    array values(object $obj)

Returns the values of the provided object.
Note that because JSON hashes are inheritently unordered, the
values associated with the provided object ``obj`` are inheritently
unordered.  Implementations are not required to return values in
any specific order.  For example, given the input::

    {"a": "first", "b": "second", "c": "third"}

The expression ``values(@)`` could have any of these return values:

* ``["first", "second", "third"]``
* ``["first", "third", "second"]``
* ``["second", "first", "third"]``
* ``["second", "third", "first"]``
* ``["third", "first", "second"]``
* ``["third", "second", "first"]``

If you would like a specific order, consider using the
``sort`` or ``sort_by`` functions.

.. cssclass:: table

.. list-table:: Examples
  :header-rows: 1

  * - Given
    - Expression
    - Result
  * - ``{"foo": "baz", "bar": "bam"}``
    - ``values(@)``
    - ``["baz", "bam"]``
  * - ``["a", "b"]``
    - ``values(@)``
    - ``<error: invalid-type>``
  * - ``false``
    - ``values(@)``
    - ``<error: invalid-type>``

