.. _func-keys:

keys
----

::

    array keys(object $obj)

Returns an array containing the keys of the provided object.
Note that because JSON hashes are inheritently unordered, the
keys associated with the provided object ``obj`` are inheritently
unordered.  Implementations are not required to return keys in
any specific order.

.. cssclass:: table

.. list-table:: Examples
  :header-rows: 1

  * - Given
    - Expression
    - Result
  * - ``{"foo": "baz", "bar": "bam"}``
    - ``keys(@)``
    - ``["foo", "bar"]``
  * - ``{}``
    - ``keys(@)``
    - ``[]``
  * - ``false``
    - ``keys(@)``
    - ``<error: invalid-type>``
  * - ``[b, a, c]``
    - ``keys(@)``
    - ``<error: invalid-type>``


