.. _func-to-array:

to_array
---------

::

    array to_array(any $arg)

* array - Returns the passed in value.
* number/string/object/boolean - Returns a one element array containing
  the passed in argument.


.. cssclass:: table

.. list-table:: Examples
  :header-rows: 1

  * - Expression
    - Result
  * - ``to_array(`[1, 2]`)``
    - ``[1, 2]``
  * - ``to_array(`"string"`)``
    - ``["string"]``
  * - ``to_array(`0`)``
    - ``[0]``
  * - ``to_array(`true`)``
    - ``[true]``
  * - ``to_array(`{"foo": "bar"}`)``
    - ``[{"foo": "bar"}]``


