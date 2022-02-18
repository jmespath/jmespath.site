==================
JMESPath Libraries
==================

The JMESPath specification is implemented in various languages.  Each list
below shows JMESPath libraries as well as the compliance level.  The compliance
level is based on which compliance tests the library can pass.


.. cssclass:: table

.. list-table::
  :header-rows: 1

  * - Language
    - Name
    - Compliance Level
  * - Python
    - `jmespath.py <https://github.com/jmespath/jmespath.py>`__
    - Fully compliant
  * - PHP
    - `jmespath.php <https://github.com/jmespath/jmespath.php>`__
    - Fully compliant
  * - Javascript
    - `jmespath.js <https://github.com/jmespath/jmespath.js>`__
    - Fully compliant
  * - Ruby
    - `jmespath.rb <https://github.com/trevorrowe/jmespath.rb>`__
    - Fully compliant
  * - Lua
    - `jmespath.lua <https://github.com/jmespath/jmespath.lua>`__
    - Fully compliant
  * - Go
    - `go-jmespath <https://github.com/jmespath/go-jmespath>`__
    - Fully compliant
  * - Java
    - `jmespath-java <https://github.com/burtcorp/jmespath-java>`__
    - Fully compliant
  * - Rust
    - `jmespath.rs <https://github.com/mtdowling/jmespath.rs>`__
    - Fully compliant
  * - DotNet
    - `jmespath.net <https://github.com/jdevillard/JmesPath.Net>`__
    - Fully compliant

In addition to the JMESPath libraries above, there are a number of
miscellaneous JMESPath tools.

.. cssclass:: table

.. list-table::
  :header-rows: 1

  * - Tool
    - Description
  * - `jmespath.terminal <https://github.com/jmespath/jmespath.terminal>`__
    - Provides a JMESPath interactive terminal that you can use to evaluate
      JMESpath expressions as you type.  The README in the
      `github repo <https://github.com/jmespath/jmespath.terminal>`__ shows
      GIFs of ``jpterm`` in action.
  * - `jp <https://github.com/jmespath/jp>`__
    - Provides a JMESPath command line interface called ``jp``.
      This cross platform tool accepts JSON data through stdin or input files,
      and prints the result of evaluating the JMESPath expression to stdout.
      This is useful if you're writing shell scripts that need to manipulate
      JSON data.
