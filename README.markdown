Introduction
------------

This project is a simple compiler built during the languages and compilers course in Poli-USP.


How to run
----------

If you are a cool guy and have ruby and rake in your machine, you can skip to
the next topic and build the project yourself, and after that, you can run it.

But, if you still are one of these old fashioned guys (=p), and still don't have
ruby, don't worry, we are giving you a already built binary, just check it out the compiler file.

We know that you are lazy and will not write a program in our language, so we
already wrote a program for you. The source is the final_program.fh file.

So, in order to run the compiler and actually get your hands dirty with MVN 
assembly, just run `./compiler final_program.fh`. The output file will be the
out.asm one.


How to build
------------

We are building the project with [Rake](http://rake.rubyforge.org/). All you
will need to do is `rake generate_binary`


Tests
-----

We are very proud to say that we have a very good test coverage in this project.
And hey, we think that is not so common for C projects to have such a good test
suite. =p

To run the test suite, you will need Rake too. And one more time, all you have
to do is `rake test:all`.

If you would like to run the tests separately, just do `rake -T`, and check
all the unit tests that we have under the namespace test:.



Authors
-------

  * Hugo Baraúna (hugo.barauna@gmail.com)
  * Fabio Yamate (fabioyamate@gmail.com)
