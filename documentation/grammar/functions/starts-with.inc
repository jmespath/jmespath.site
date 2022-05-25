.. _func-starts-with:

starts_with
-----------

::

    boolean starts_with(string $subject, string $prefix)

Returns ``true`` if the ``$subject`` starts with the ``$prefix``, otherwise
this function returns ``false``.

.. list-table:: Examples
  :header-rows: 1

  * - Given
    - Expression
    - Result
  * - ``foobarbaz``
    - ``starts_with(@, `foo`)``
    - ``true``
  * - ``foobarbaz``
    - ``starts_with(@, `baz`)``
    - ``false``
  * - ``foobarbaz``
    - ``starts_with(@, `f`)``
    - ``true``


