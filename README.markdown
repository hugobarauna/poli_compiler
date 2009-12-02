Introduction
------------

This project is a simple compiler built during the languages and compilers course in Poli-USP.


How to run
----------

First of all, we need to build our compiler, so skip to the next topic, and
after that, you can go back here, and we will teach you the art of how to run
our compiler. =p

We know that you are lazy and will not write a program in our language, so we
already wrote a program for you. The source is the samples/final_program.fh file.


Maybe you are a compiler geek, and would like to try another programs in our
bad ass mother fucker language. If that is true, just have a look in the other
programs inside the samples/ directory. 

So, in order to run the compiler and actually get your hands dirty with MVN 
assembly, just run `./bin/compiler samples/final_program.fh`. The output file will be the
out.asm one.


How to build
------------

We are building the project with [Rake](http://rake.rubyforge.org/). If you are 
a cool guy and have ruby and rake installed in your machine, all you have to do 
in order to build the project yourself is `rake generate_binary`. This will 
generate a bin file at bin/compiler. Be happy!

But, if you still are one of these old fashioned guys (=p), and still don't have
ruby, don't worry, we are giving you another way to build our project, with 
(argh!), a shell script. Just run `./build.sh`. This will generate a bin file at 
bin/compiler. Be happy!


Tests
-----

We are very proud to say that we have a very good test coverage in this project.
And hey, we think that this is not so common for C projects to have such a good test
suite. =p

To run the test suite, you will need Rake too. And one more time, all you have
to do is `rake test:all`.

If you would like to run the tests separately, just do `rake -T`, and check
all the unit tests that we have under test namespace.



Authors
-------

  * Hugo Baraúna (hugo.barauna@gmail.com)
  * Fabio Yamate (fabioyamate@gmail.com)
