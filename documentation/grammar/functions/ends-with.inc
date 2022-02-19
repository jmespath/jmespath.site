.. _func-ends-with:

ends_with
---------

::

    boolean ends_with(string $subject, string $prefix)

Returns ``true`` if the ``$subject`` ends with the ``$prefix``, otherwise this
function returns ``false``.


.. list-table:: Examples
  :header-rows: 1

  * - Given
    - Expression
    - Result
  * - ``foobarbaz``
    - ``ends_with(@, `baz`)``
    - ``true``
  * - ``foobarbaz``
    - ``ends_with(@, `foo`)``
    - ``false``
  * - ``foobarbaz``
    - ``ends_with(@, `z`)``
    - ``true``


