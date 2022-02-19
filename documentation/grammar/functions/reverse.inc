.. _func-reverse:

reverse
-------

::

    array reverse(string|array $argument)

Reverses the order of the ``$argument``.


.. list-table:: Examples
  :header-rows: 1

  * - Given
    - Expression
    - Result
  * - ``[0, 1, 2, 3, 4]``
    - ``reverse(@)``
    - ``[4, 3, 2, 1, 0]``
  * - ``[]``
    - ``reverse(@)``
    - ``[]``
  * - ``["a", "b", "c", 1, 2, 3]``
    - ``reverse(@)``
    - ``[3, 2, 1, "c", "b", "a"]``
  * - ``"abcd``
    - ``reverse(@)``
    - ``dcba``


