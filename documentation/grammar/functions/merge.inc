.. _func-merge:

merge
-----

::

    object merge([object *argument, [, object $...]])

Accepts 0 or more objects as arguments, and returns a single object
with subsequent objects merged.  Each subsequent object's key/value
pairs are added to the preceding object.  This function is used
to combine multiple objects into one.  You can think of this as
the first object being the base object, and each subsequent argument
being overrides that are applied to the base object.


.. cssclass:: table

.. list-table:: Examples
  :header-rows: 1

  * - Expression
    - Result
  * - ``merge(`{"a": "b"}`, `{"c": "d"}`)``
    - ``{"a": "b", "c": "d"}``
  * - ``merge(`{"a": "b"}`, `{"a": "override"}`)``
    - ``{"a": "override"}``
  * - ``merge(`{"a": "x", "b": "y"}`, `{"b": "override", "c": "z"}`)``
    - ``{"a": "x", "b": "override", "c": "z"}``


