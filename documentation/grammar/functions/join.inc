.. _func-join:

join
----

::

    string join(string $glue, array[string] $stringsarray)

Returns all of the elements from the provided ``$stringsarray`` array joined
together using the ``$glue`` argument as a separator between each.


.. cssclass:: table

.. list-table:: Examples
  :header-rows: 1

  * - Given
    - Expression
    - Result
  * - ``["a", "b"]``
    - ``join(`, `, @)``
    - ``"a, b"``
  * - ``["a", "b"]``
    - :literal:`join(\`\`, @)`
    - ``"ab"``
  * - ``["a", false, "b"]``
    - ``join(`, `, @)``
    - ``<error: invalid-type>``
  * - ``[false]``
    - ``join(`, `, @)``
    - ``<error: invalid-type>``

