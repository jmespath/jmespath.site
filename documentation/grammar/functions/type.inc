.. _func-type:

type
----

::

    string type(array|object|string|number|boolean|null $subject)

Returns the JavaScript type of the given ``$subject`` argument as a string
value.

The return value MUST be one of the following:

* number
* string
* boolean
* array
* object
* null


.. cssclass:: table

.. list-table:: Examples
  :header-rows: 1

  * - Given
    - Expression
    - Result
  * - ``"foo"``
    - ``type(@)``
    - ``"string"``
  * - ``true``
    - ``type(@)``
    - ``"boolean"``
  * - ``false``
    - ``type(@)``
    - ``"boolean"``
  * - ``null``
    - ``type(@)``
    - ``"null"``
  * - ``123``
    - ``type(@)``
    - ``number``
  * - ``123.05``
    - ``type(@)``
    - ``number``
  * - ``["abc"]``
    - ``type(@)``
    - ``"array"``
  * - ``{"abc": "123"}``
    - ``type(@)``
    - ``"object"``


