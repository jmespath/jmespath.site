.. _func-to-number:

to_number
---------

::

    number to_number(any $arg)

* string - Returns the parsed number.  Any string that conforms to the
  ``json-number`` production is supported.  Note that the floating number
  support will be implementation specific, but implementations should support
  at least IEEE 754-2008 binary64 (double precision) numbers, as this is
  generally available and widely used.
* number - Returns the passed in value.
* array - null
* object - null
* boolean - null
* null - null


