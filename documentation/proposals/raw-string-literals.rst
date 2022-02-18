===================
Raw String Literals
===================

:JEP: 12
:Author: Michael Dowling
:Status: accepted
:Created: 09-Apr-2015

Abstract
========

This JEP proposes the following modifications to JMESPath in order to improve
the usability of the language and ease the implementation of parsers:

- Addition of a **raw string literal** to JMESPath that will allow expressions
  to contain raw strings that are not mutated by JSON escape sequences (e.g.,
  "\\n", "\\r", "\\u005C").
- Deprecation of  the current literal parsing behavior that allows for unquoted
  JSON strings to be parsed as JSON strings, removing an ambiguity in the
  JMESPath grammar and helping to ensure consistency among implementations.

This proposal seeks to add the following syntax to JMESPath::

    'foobar'
    'foo\'bar'
    `bar` -> Parse error/warning (implementation specific)


Motivation
==========

Raw string literals are provided in `various programming languages
<https://en.wikipedia.org/wiki/String_literal#Raw_strings>`_ in order to prevent
language specific interpretation (i.e., JSON parsing) and remove the need for
escaping, avoiding a common problem called `leaning toothpick syndrome (LTS)
<https://en.wikipedia.org/wiki/Leaning_toothpick_syndrome>`_. Leaning toothpick
syndrome is an issue in which strings become unreadable due to excessive use of
escape characters in order to avoid delimiter collision (e.g., ``\\\\\\``).

When evaluating a JMESPath expression, it is often necessary to utilize string
literals that are not extracted from the data being evaluated, but rather
statically part of the compiled JMESPath expression. String literals are useful
in many areas, but most notably when invoking functions or building up
multi-select lists and hashes.

The following expression returns the number of characters found in the string
``foo``. When parsing this expression, ```"foo"``` is parsed as a JSON value
which produces the string literal value of ``foo``::

    `"foo"`

The following expression is functionally equivalent. Notice that the quotes are
elided from the JSON literal::

    `foo`

These string literals are parsed using a JSON parser according to
`RFC 4627 <https://www.ietf.org/rfc/rfc4627.txt>`_, which will expand unicode
escape sequences, newline characters, and several other escape sequences
documented in RFC 4627 section 2.5.

For example, the use of an escaped unicode value ``\u002B`` is expanded into
``+`` in the following JMESPath expression::

    `"foo\u002B"` -> "foo+"

You can escape escape sequences in JSON literals to prevent an escape sequence
from being expanded::

    `"foo\\u002B"` -> "foo\u002B"
    `foo\\u002B` -> "foo\u002B"

While this allows you to provide literal strings, it presents the following
problems:

1. Incurs an additional JSON parsing penalty.
2. Requires the cognitive overhead of escaping escape characters if you
   actually want the data to be represented as it was literally provided
   (which can lead to LTS). If the data being escaped was meant to be used
   along with another language that uses ``\`` as an escape character, then the
   number of backslash characters doubles.
3. Introduces an ambiguous rule to the JMESPath grammar that requires a prose
   based specification to resolve the ambiguity in parser implementations.

   The relevant literal grammar rules are currently defined as follows::

      literal = "`" json-value "`"
      literal =/ "`" 1*(unescaped-literal / escaped-literal) "`"
      unescaped-literal = %x20-21 /       ; space !
                              %x23-5B /   ; # - [
                              %x5D-5F /   ; ] ^ _
                              %x61-7A     ; a-z
                              %x7C-10FFFF ; |}~ ...
      escaped-literal   = escaped-char / (escape %x60)
      json-value = false / null / true / json-object / json-array /
                   json-number / json-quoted-string
      false = %x66.61.6c.73.65   ; false
      null  = %x6e.75.6c.6c      ; null
      true  = %x74.72.75.65      ; true
      json-quoted-string = %x22 1*(unescaped-literal / escaped-literal) %x22
      begin-array     = ws %x5B ws  ; [ left square bracket
      begin-object    = ws %x7B ws  ; { left curly bracket
      end-array       = ws %x5D ws  ; ] right square bracket
      end-object      = ws %x7D ws  ; } right curly bracket
      name-separator  = ws %x3A ws  ; : colon
      value-separator = ws %x2C ws  ; , comma
      ws              = *(%x20 /              ; Space
                          %x09 /              ; Horizontal tab
                          %x0A /              ; Line feed or New line
                          %x0D                ; Carriage return
                         )
      json-object = begin-object [ member *( value-separator member ) ] end-object
      member = quoted-string name-separator json-value
      json-array = begin-array [ json-value *( value-separator json-value ) ] end-array
      json-number = [ minus ] int [ frac ] [ exp ]
      decimal-point = %x2E       ; .
      digit1-9 = %x31-39         ; 1-9
      e = %x65 / %x45            ; e E
      exp = e [ minus / plus ] 1*DIGIT
      frac = decimal-point 1*DIGIT
      int = zero / ( digit1-9 *DIGIT )
      minus = %x2D               ; -
      plus = %x2B                ; +
      zero = %x30                ; 0

   The ``literal`` rule is ambiguous because ``unescaped-literal`` includes
   all of the same characters that ``json-value`` match, allowing any value
   that is valid JSON to be matched on either ``unescaped-literal`` or
   ``json-value``.


Rationale
---------

When implementing parsers for JMESPath, one must provide special case parsing
when parsing JSON literals due to the allowance of elided quotes around JSON
string literals (e.g., ```foo```). This specific aspect of JMESPath cannot be
described unambiguously in a context free grammar and could become a common
cause of errors when implementing JMESPath parsers.

Parsing JSON literals has other complications as well. Here are the steps
needed to currently parse a JSON literal value in JMESPath:

1. When a ````` token is encountered, begin parsing a JSON literal.
2. Collect each character between the opening ````` and closing `````
   token, including any escaped ````` characters (i.e., ``\```) and store the
   characters in a variable (let's call it ``$lexeme``).
3. Copy the contents of ``$lexeme`` to a temporary value in which all leading
   and trailing whitespace is removed. Let's call this ``$temp`` (this is
   currently not documented but required in the
   `JMESPath compliance tests <https://github.com/jmespath/jmespath.test/blob/c532a20e3bca635fb6ca248e5e955e1bd146a965/tests/syntax.json#L592-L606>`_).
4. If ``$temp`` can be parsed as valid JSON, then use the parsed result as the
   value for the literal token.
5. If ``$temp`` cannot be parsed as valid JSON, then wrap the contents of
   ``$lexeme`` in double quotes and parse the wrapped value as a JSON string,
   making the following expressions equivalent: ```foo``` == ```"foo"```, and
   ```[1, ]``` == ```"[1, ]"```.

It is reasonable to assume that the most common use case for a JSON literal in
a JMESPath expression is to provide a string value to a function argument or
to provide a literal string value to a value in a multi-select list or
multi-select hash. In order to make providing string values easier, it was
decided that JMESPath should allow the quotes around the string to be elided.

This proposal posits that allowing quotes to be elided when parsing JSON
literals should be deprecated in favor of adding a proper string literal rule
to JMESPath.


Specification
=============

A raw string literal is value that begins and ends with a single quote, does
not interpret escape characters, and may contain escaped single quotes to
avoid delimiter collision.


Examples
--------

Here are several examples of valid raw string literals and how they are
parsed:

- A basic raw string literal, parsed as ``foo bar``::

      'foo bar'

- An escaped single quote, parsed as ``foo'bar``::

      'foo\'bar'

- A raw string literal that contains new lines::

      'foo
      bar
      baz!'

  The above expression would be parsed as a string that contains new lines::

      foo
      baz
      bar!

- A raw string literal that contains escape characters,
  parsed as ``foo\nbar``::

       foo\nbar


ABNF
----

The following ABNF grammar rules will be added, and is allowed anywhere an
expression is allowed::

    raw-string        = "'" *raw-string-char "'"
    ; The first grouping matches any character other than "\"
    raw-string-char   = (%x20-26 / %x28-5B / %x5D-10FFFF) / raw-string-escape
    raw-string-escape = escape ["'"]

This rule allows any character inside of a raw string, including an escaped
single quote.

In addition to adding a ``raw-string`` rule, the ``literal`` rule in the ABNF
will be updated to become::

    literal = "`" json-value "`"


Impact
======

The impact to existing users of JMESPath is that the use of a JSON literal
in which the quotes are elided SHOULD be converted to use the string-literal
rule of the grammar. Whether or not this conversion is absolutely necessary
will depend on the specific JMESPath implementation.

Implementations MAY choose to support the old syntax of allowing elided quotes
in JSON literal expressions. If an implementation chooses this approach, the
implementation SHOULD raise some kind of warning to the user to let them know
of the deprecation and possible incompatibility with other JMESPath
implementations.

In order to support this type of variance in JMESPath implementations, all of
the JSON literal compliance test cases that involve elided quotes MUST be
removed, and test cases regarding failing on invalid unquoted JSON values MUST
not be allowed in the compliance test unless placed in a JEP 12 specific
test suite, allowing implementations that support elided quotes in JSON
literals to filter out the JEP 12 specific test cases.


Alternative approaches
======================

There are several alternative approaches that could be taken.


Leave as-is
-----------

This is a valid and reasonable suggestion. Leaving JMESPath as-is would avoid
a breaking change to the grammar and users could continue to use multiple
escape characters to avoid delimiter collision.

The goal of this proposal is not to add functionality to JMESPath, but rather
to make the language easier to use, easier to reason about, and easier to
implement. As it currently stands, the behavior of JSON parsing is ambiguous
and requires special casing when implementing a JMESPath parser. It also allows
for minor differences in implementations due to this ambiguity.

Take the following example::

    `[1`

One implementation may interpret this expression as a JSON string with the
string value of ``"[1"``, while other implementations may raise a parse error
because the first character of the expression appears to be valid JSON.

By updating the grammar to require valid JSON in the JSON literal token, we can
remove this ambiguity completely, removing a potential source of inconsistency
from the various JMESPath implementations.


Disallow single quotes in a raw string
--------------------------------------

This proposal states that single quotes in a raw string literal must be escaped
with a backslash. An alternative approach could be to not allow single quotes
in a raw string literal. While this would simplify the ``raw-string`` grammar
rule, it would severely limit the usability of the ``raw-string`` rule, forcing
users to use the ``literal`` rule.


Use a customizable delimiter
----------------------------

Several languages allow for a custom delimiter to be placed around a raw
string. For example, Lua allows for a `long bracket <https://www.lua.org/manual/5.2/manual.html#3.1>`_ notation in which raw
strings are surrounded by ``[[]]`` with any number of balanced `=` characters
between the brackets::

    [==[foo=bar]==] -- parsed as "foo=bar"

This approach is very flexible and removes the need to escape any characters;
however, this can not be expressed in a regular grammar. A parser would need to
keep track of the number of opened delimiters and ensure that it is closed with
the appropriate number of matching characters.

The addition of a string literal as described in this JEP does not preclude a
later addition of a heredoc or delimited style string literal as provided by
languages like Lua, `D <https://dlang.org/lex.html#DelimitedString>`_,
`C++ <https://en.wikipedia.org/wiki/C%2B%2B11#New_string_literals>`_, etc...
