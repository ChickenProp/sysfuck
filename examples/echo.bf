#! /usr/bin/bf
[
This program is intended to be run under sfwrap, as in
    $ sfwrap /pathto/echo.bf [ args... ]
As the name suggests, it will simply echo its arguments to stdout. I think it
should work under 8- and 16-bit brainfucks, but I've only tested it with one
interpreter, bff (http://www.swapped.cc/bff/) which is 32-bit.

The general layout is:

Write some useful strings (argc, argv, strlen, write) into early memory for easy
printing. These take cells 1-19, with a 0 in cell 0.

Call 'argc' and store NUMARGS, the number of arguments in cells 21-24, with null
bytes in cells 20 and 25. Also store an INDEX, initially 0, in cells 26-29, with
a null byte in cell 30. To work with these, we use algorithms similar to those
found at http://www.esolangs.org/wiki/Brainfuck_bitwidth_conversions, except +
and - have been modified slightly to work with 16- and 32-bit cells.  (These
algorithms use a little-endian byte order, which is convenient because so do
we.) The null bytes at 20, 25 and 30 are used in these algorithms, but are
always returned to 0.

Then we go into a simple "while NUMARGS >= 0" loop to print the
arguments. Because argc counts argv[0], which we don't want to print, we need to
decrement NUMARGS before entering the loop. We also need to print spaces, and a
newline at the end. For the spaces we use a flag NOTFIRST in cell 31.

So we decrement NUMARGS, then start the loop:
  while NUMARGS != 0:
    if NOTFIRST != 0:
      print a space
    else:
      set NOTFIRST = 1
    increment INDEX
    read ARG = argv(INDEX) into cells 32-35
    read LEN = strlen(ARG) into cells 36-39
    write LEN bytes starting at position ARG to fd 1 (stdout)
    read and discard the result
    decrement NUMARGS
  print a newline

There are many opportunities to improve the memory layout and algorithms
used. That's because it's easier to keep going with a slightly suboptimal
architechure than it is to rewrite it.

Also, we don't check the return value of the write() call, or do any other form
of error detection. Some lengths just aren't worth going to.
]

setup strings: argc argv strlen write
  put 'a' (97) into index 0
    > ++++++++++ [<+++++++++>-] < +++++++
  move to indices 1 through 19
    [->+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+>+ <<<<<<<<<<<<<<<<<<<]
  now we have \0 a a a etc and are at index 0
  put the correct characters into indices appropriately
  'a' >
  'r' > +++++++++++++++++
  'g' > ++++++
  'c' > ++
  'a' >
  'r' > +++++++++++++++++
  'g' > ++++++
  'v' > +++++++++++++++++++++
  's' > ++++++++++++++++++
  't' > +++++++++++++++++++
  'r' > +++++++++++++++++
  'l' > +++++++++++
  'e' > ++++
  'n' > +++++++++++++
  'w' > ++++++++++++++++++++++
  'r' > +++++++++++++++++
  'i' > ++++++++
  't' > +++++++++++++++++++
  'e' > ++++
      [<]

we are at index 0 and the buffer looks like:
  "\0argcargvstrlenwrite\0" (cells 0 through 20)

get the number of arguments with '\0argc\0\0'
  .>.>.>.>.[>]..

read the number of arguments into cells 21 through 24
  >,>,>,>,

put 1 in cell 26
  >>+-

decrement NUMARGS
  <<<<<
  [<+>>>>>+<<<<-]<[>+<-]+>>>>>[<<<<<->>>>>[-]]<<<<<[-
   >>
   [<<+>>>>>+<<<-]<<[>>+<<-]+>>>>>[<<<<<->>>>>[-]]<<<<<[-
    >>>
    [<<<+>>>>>+<<-]<<<[>>>+<<<-]+>>>>>[<<<<<->>>>>[-]]<<<<<[-
     >>>+>[<-]<[+++++++++++++++[>++++++++++++++++<-]<]
     >>-<<<<++++++++++++++++[>>>++++++++++++++++<<<-]
    ]
    >>>-<<<++++++++++++++++[>>++++++++++++++++<<-]
   ]
   >>-<<++++++++++++++++[>++++++++++++++++<-]
  ]>-

while NUMARGS != 0
  [>>>>+>>>>>+<<<<<<<<<-]>>>>>>>>>[<<<<<<<<<+>>>>>>>>>-]<<<<<
  [[-]<<<<<+>>>>>]<<<
  [>>>+>>>>>+<<<<<<<<-]>>>>>>>>[<<<<<<<<+>>>>>>>>-]<<<<<
  [[-]<<<<<+>>>>>]<<
  [>>+>>>>>+<<<<<<<-]>>>>>>>[<<<<<<<+>>>>>>>-]<<<<<
  [[-]<<<<<+>>>>>]<
  [>+>>>>>+<<<<<<-]>>>>>>[<<<<<<+>>>>>>-]<<<<<
  [[-]<<<<<+>>>>>]<<<<<
  [[-]>

if NOTFIRST != 0 print space else set NOTFIRST = 1
(this clobbers ARG but that's okay)
  >>>>>>>>>>
  >[-]+>[-]<<[
   >+++[>++++++++<-]>.[-]<
  ]>[
   -<+>>
  ]

increment INDEX
  <<<<<<<
  +<
  ++++++++++++++++[>>>>>++++++++++++++++<<<<<-]>[-<+>]>>>>
  [-<<<<+<->>>>>]+<<<<<[[+>-<]>>>>>-<<<<<]>>>>>[-<<<<[-]>>>>]<<<<
  [<+>>>>>+<<<<-]<[>+<-]+>>>>>[<<<<<->>>>>[-]]<<<<<[-
   >>+<<
   ++++++++++++++++[>>>>>++++++++++++++++<<<<<-]>>[-<<+>>]>>>
   [-<<<+<<->>>>>]+<<<<<[[+>>-<<]>>>>>-<<<<<]>>>>>[-<<<[-]>>>]<<<
   [<<+>>>>>+<<<-]<<[>>+<<-]+>>>>>[<<<<<->>>>>[-]]<<<<<[-
    >>>+<<<
    ++++++++++++++++[>>>>>++++++++++++++++<<<<<-]>>>[-<<<+>>>]>>
    [-<<+<<<->>>>>]+<<<<<[[+>>>-<<<]>>>>>-<<<<<]>>>>>[-<<[-]>>]<<
    [<<<+>>>>>+<<-]<<<[>>>+<<<-]+>>>>>[<<<<<->>>>>[-]]<<<<<[-
     >>>>+<<<<
     ++++++++++++++++[>>>>>++++++++++++++++<<<<<-]>>>>[-<<<<+>>>>]>
     [-<+<<<<->>>>>]+<<<<<[[+>>>>-<<<<]>>>>>-<<<<<]>>>>>[-<[-]>]<<<<<
    ]
   ]
  ]>


print \0 ARGV \0 \4
  <<<<<< .
  <[<]>>>>> .>.>.>. [>] .++++ . ----

print INDEX
  >>>>>>.>.>.>.

read ARG into cells 32 to 35
  >>>,>,>,>,

print \0 STRLEN \0 \4
  <<<<<<<<<<<<<<<
  . <<<<<<<<<<< .>.>.>.>.>. [>] . ++++.----

print ARG
  >>>>>>>>>>>>.>.>.>.

read STRLEN into cells 36 to 39
  >,>,>,>,

print \0 WRITE \0 \12 \1 \0 \0 \0
  <<<<<<<<<<<<<<<<<<<
  . <<<<< .>.>.>.>.> . ++++++++++++ . ----------- . - . . .

print ARG LEN
  >>>>>>>>>>>> .>.>.>.>.>.>.>.

read and discard result
  ,,,,

decrement NUMARGS
  <<<<<<<<<<<<<<<<<<
  [<+>>>>>+<<<<-]<[>+<-]+>>>>>[<<<<<->>>>>[-]]<<<<<[-
   >>
   [<<+>>>>>+<<<-]<<[>>+<<-]+>>>>>[<<<<<->>>>>[-]]<<<<<[-
    >>>
    [<<<+>>>>>+<<-]<<<[>>>+<<<-]+>>>>>[<<<<<->>>>>[-]]<<<<<[-
     >>>+>[<-]<[+++++++++++++++[>++++++++++++++++<-]<]
     >>-<<<<++++++++++++++++[>>>++++++++++++++++<<<-]
    ]
    >>>-<<<++++++++++++++++[>>++++++++++++++++<<-]
   ]
   >>-<<++++++++++++++++[>++++++++++++++++<-]
  ]>-

end while
  [>>>>+>>>>>+<<<<<<<<<-]>>>>>>>>>[<<<<<<<<<+>>>>>>>>>-]<<<<<
  [[-]<<<<<+>>>>>]<<<
  [>>>+>>>>>+<<<<<<<<-]>>>>>>>>[<<<<<<<<+>>>>>>>>-]<<<<<
  [[-]<<<<<+>>>>>]<<
  [>>+>>>>>+<<<<<<<-]>>>>>>>[<<<<<<<+>>>>>>>-]<<<<<
  [[-]<<<<<+>>>>>]<
  [>+>>>>>+<<<<<<-]>>>>>>[<<<<<<+>>>>>>-]<<<<<
  [[-]<<<<<+>>>>>]<<<<<
  ]>

print newline
  ++++++++++.
