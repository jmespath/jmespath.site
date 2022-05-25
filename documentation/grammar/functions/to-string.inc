.. _func-to-string:


to_string
---------

::

    string to_string(any $arg)

* string - Returns the passed in value.
* number/array/object/boolean - The JSON encoded value of the object.  The
  JSON encoder should emit the encoded JSON value without adding any additional
  new lines.


.. cssclass:: table

.. list-table:: Examples
  :header-rows: 1

  * - Expression
    - Result
  * - ``to_string(`2`)``
    - ``"2"``


