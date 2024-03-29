Boggler
=====

This is a simple program that I wrote many years ago (~1996) for
generating and solving Boggle boards. No guarantees are made regarding
the suitability of this program for any purpose. Do with it what you
will.

Here are basic instructions on how it can be used:

Compile the program with `make`. I've tested it with GNU Make 3.81 and
gcc version 4.6.3 on Ubuntu 12.04.

Generate a game board with the following command. This will produce a
5x5 board on standard output.

    ./boggler -g -w

To solve a board, do the following (where *`test`* is the name of a
file containing a board generated by boggler.)

    ./boggler -p test -s -d wordlist

The general input/output format was roughly inspired to interoperate
with a paleolithic version of [Tcl/Tk](http://www.tcl.tk/). My
original plan had been to put a GUI front end on `boggler`, and you
can see the residual code from that effort in the `tkboggler` file.

# Licensing and Copyright

Copyright (c) Michael Schaeffer

Licened under the Eclipse Public Licence v2.0, found at the root of
this repository in the [`LICENSE`](LICENSE) file.
