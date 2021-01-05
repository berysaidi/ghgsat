[![Build Status](https://travis-ci.com/berysaidi/ghgsat.svg?token=yNYn6mL8P46zqkxpb75A&branch=main)](https://travis-ci.com/berysaidi/ghgsat)

## Table of contents
* [General Info](#general-info)
* [Native Build Instructions](#nativeinstructions)
* [Cross Compilation Instructions](#crossinstructions)

## General Info

  This is an a quick s-expression parser in a single file which runs correctly for the example supplied in the test.
  It uses recursions to tokenize and look forward for delimiters.
  For every token, it prints its data type (Integer, Float, Date, Atomic) and corresponding value.
  This is by no means a perfect implementation, but considering the amount of time given, it should be acceptable.
  A perfect implementation would require a Full recursive decent parser and lexical analyser for LISP syntax
  If I had enough time, I would use Flex & Bison to perfectly cover all the corner cases.
  
## Native Build Instructions
We rely on a simple makefile to compile the binary for the s-expression.
for your hosted compiler use : ``` make build ```
## Cross Compilation Instructions
*  Getting an image with cross compilation tools. This will come with arm-linux-gnueabi
``` docker pull ev3dev/debian-stretch-cross ```
* Giving it a tagname
``` docker tag ev3dev/debian-stretch-cross cross_env ```
* Mounting the current directory project into the container
``` docker run --rm -it -v `pwd`:/var/ghgsat -w /var/ghgsat cross_env ```
* Cross Compiling for arm
``` make build-cross will produce a an arm ELF file ```
* In order to verify the output :
``` readelf -h bin/sexpr ```
* Try the binary in Emulation with qemu :
``` qemu-arm bin/sexpr test/example.sexp ```
* Try it natively :
``` scp bin/sexpr username@host:/your_path and run it there ```
