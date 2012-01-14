# sysfuck: bridging the gap between "Turing complete" and "actually useful".

## Introduction

Quoth [wikipedia](http://en.wikipedia.org/wiki/Turing_tarpit):

> In any Turing complete language, it is possible to write any computer program

Bollocks!

Okay, this is true in an abstract sense. Any *computation*, if it can be performed at all, can be performed in any Turing complete language. But a program is not, in general, just a computation. A program exists in an environment, and some programs need to interact with that environment.

Pick a Turing complete language like brainfuck or befunge or unlambda. How many of the standard unix utilities can you write in it?

Well, if you ignore the `--help` and `--version` options, you can write `true`.

You could write usable replacements for `uniq` and `wc` and a few others by splitting their command-line options into seperate programs. (So you'd have `uniq-c`, `uniq-d`, `uniq-cd`, `uniq-i`, `uniq-ci`....) You could write a usable replacement for `yes` which reads a string from stdin instead of accepting it on the command line.

But `echo` - `ls` - `test` - even `false` - are all impossible. (Yes okay, you could do the same thing to `echo` that you did to `yes`. But then you haven't replaced `echo`, you've replaced `cat`.)

This Will Not Do.

And with sysfuck, It No Longer Does.

Sysfuck is a utility for bridging the gap between "Turing complete" and "actually useful". Pick a Turing complete language, and with sysfuck, it will be able to interact with the environment. Specifically, it will be able to make syscalls. Since syscalls are the basic tools for interacting with the environment, this means you'll be able to do anything that you could do in (for example) C.

You don't need to invent a new language, very like your favourite Turing complete language but actually useful. You don't need to reimplement or even recompile your favourite Turing complete language.

(Technically, Turing completeness is not sufficient. It's conceivable that your favourite Turing complete language is only capable of saying "yes" and "no". If that is the case, turn back now! For you will find no solace here. The way is closed to you; you cannot pass.)

But assuming you have a language which can read from standard input, and write any computable string to standard output, read on...

## How to use it

Most of the time, you won't call the `sysfuck` program directly. When you write a program that needs to take advantage of sysfuck, you call it using the program `sfwrap`, like so:

    $ sfwrap program arg1 arg2 ...

In your program, printing works mostly like it would normally. If you print a byte that isn't 0x00 (ascii NUL), it will show up on the terminal, or wherever stdout was redirected to. If you want to send a NUL byte, print two of them in a row.

To make a syscall, print

* a NUL byte;

* followed by the name of the syscall;

* followed by another NUL byte;

* followed by a single byte, giving the size of the data that you want to pass by the syscall (eg. 0x0c for 12 bytes);

* followed by the data itself.

The data will usually contain four bytes for each argument that the syscall expects. The first four bytes give the first argument; the next four give the second argument; and so on. Most (maybe all?) arguments to syscalls are of `int`, `long`, or pointer type, or some type equivalent to one of these (such as `size_t`). Since `sysfuck` is compiled in 32-bit mode, these all take four bytes even on a 64-bit system. If you find a syscall that takes a `char` argument, you would only pass one byte for it.

For example, to make the C syscall

    write(2, 0x12345678, 260);

you would print, assuming a little-endian architecture, the following hexadecimal bytes:

    00 'write' 00 0c 02 00 00 00 78 56 34 12 04 01 00 00

('write' means to print the five bytes `77 `(w) `72` (r) etc.) Here `0c` is twelve, the number of bytes that follow; `02 00 00 00` is the first argument, 2; `78 56 34 12` is the second argument; and `04 01 00 00` is the final argument.

The syscall's return value is written to your program's standard input as four bytes. If the above call succeeded, it would return 260, so you would read the bytes `04 01 00 0`0. If it only wrote 20 bytes, you would read `14 00 00 00`.

If the data is smaller than the syscall expects, it will be padded with NUL bytes at the end. So the above call could also have been

    00 'write' 00 0a 02 00 00 00 78 56 34 12 04 01

Note that `0a` matches the number of bytes *sent*, not the number of bytes *expected*.

As well as syscalls, there are some other commands available. You call them in exactly the same way.

* `memread(char *ptr, int n)` - this reads `n` bytes from memory location `ptr` and sends them to your program's standard input.

* `memwrite(char *ptr, ...)` - this does the opposite of `memread`. After `ptr`, you pass a number of bytes, which get written to memory location `ptr`. For example, sending

    00 'memwrite' 00 06 78 56 34 12 ca fe

will cause the two bytes `ca fe` to be written to memory location 0x12345678. Because this function is variadic, the number of bytess sent is significant; null padding is not performed. Nothing is sent to standard input.

* `strlen(char *ptr)` - this is just an interface to the C standard library function.

* `argc(void)` - this returns `argc`, the number of command-line arguments sent to the program. The first argument is the program name, so `argc` is always at least 1.

* `argv(int n)` - this returns the n'th command-line argument, where the zeroth argument is the program name.

* `getenv(...)` - like `memwrite()`, you pass a string directly instead of a pointer to one. This returns a pointer to the value of the environment variable named by that string, so

    00 'getenv' 00 04 'HOME'

will return a pointer to a null-terminated string containing your home directory. Due to implementation details, you can only pass a string of up to 25**4** characters in length; if you try to pass a string of length 255, the final byte will be truncated.

* `stdout(int fd)` - set the file descriptor that output from the program gets sent to. The default is 1.

## Examples

Three examples are provided in the `examples/` directory, to be run under `sfwrap`.

* `echo.bf` is a replacement for `echo` in brainfuck, but without any fancy features. You should either compile it or edit the shebang (`#!`) line to point to whatever brainfuck interpreter you use; if you run it as `sfwrap my-brainfuck-interpreter echo.bf` then "echo.bf" will be treated as one of the arguments to echo. (See "Caveats" below.)

It's been tested with the [bff](http://www.swapped.cc/bff/) interpreter, but hopefully works with others. Unfortunately `bff` doesn't handle command line arguments nicely, so the current shebang line uses my [`cmd`](https://github.com/ChickenProp/cmd) utility to avoid that.

* `asciisf` is a perl script that lets you interact with `sysfuck` in a human-readable manner. It takes input in the format `name(data)`. Here `name` is the name of the syscall, and `data` is a whitespace-separated sequence of hex numbers and quoted strings. So `memwrite(12345678 'hello' 0)` sends `00 'memwrite' 00 0a 78 56 34 12 'hello' 00` to `sysfuck`.

`asciisf` doesn't use Readline, but you can do `rlwrap sfwrap asciisf`. (But not `sfwrap rlwrap asciisf`.)

* `test-sfwrap.pl` is just a simple nonexhaustive test program. It should print "this should get printed" to stdout; then "with a null byte(\0) on fd 5" to file descriptor 5 (call `sfwrap examples/test-sfwrap.pl 5>&1` to see this); followed by the first three bytes of its `argv[0]`.

Also in examples is `bindump.pl`, even though it's not really an example. It's useful for testing `sfwrap` on a lower level than `asciisf` can. It's basically a reverse hexdump, and takes input in the form of hex numbers and quoted strings, like `asciisf`'s data. (But with `bindump.pl`, hex numbers greater than `ff` are printed big-endianly, which is uncool.)

## Building and installing

If you have SCons installed, you can build sysfuck by simply running `scons`. If not, you can do it yourself with these shell commands:

    ./gen_str_to_syscall.sh
    gcc -o callbacks.o -c -m32 -Wall callbacks.c
    gcc -o sysfuck.o -c -m32 -Wall sysfuck.c
    gcc -o sysfuck sysfuck.o callbacks.o

You should probably copy `sfwrap` and `sysfuck` into your PATH. If `sysfuck` is not in your PATH, you can tell `sfwrap` where to find it with `sfwrap -c /path/to/sysfuck program ...`.

## How it works

`sysfuck` is essentially a UNIX filter: it takes input (as described above), and produces output (as described above). But in between it makes syscalls. Additionally, it reads and writes on file descriptors 3 and 4 instead of stdin and stdout respectively. This is so that stdin and stdout can still be used to talk to the terminal when `sysfuck` is being controlled by another process.

`sfwrap program args...` executes `program` with the specified arguments, but under the following conditions:

* stdin and stdout are redirected to talk to a `sysfuck` process. This process has file descriptors 3 and 4 opened to talk to `program`, and its argv is replaced with `program args...`. (In particular, "sysfuck" does not appear in the argv; `argv[0]` is "program".)

* File descriptors 3 and 4 are opened to wherever stdin and stdout went originally. If you need to use sysfuck, you probably can't use these, but they're there just in case. `asciisf` uses them.

* The utility `stdbuf` is used to stop `program` from performing output buffering, to prevent deadlock. This affects programs which use the functions in `stdio.h`, which is most of them.

(Buffering causes deadlock because if `program` attempts to make a syscall and read the result, but the output is buffered so that no data is actually sent, then both `program` and `sysfuck` will hang waiting for the other to say something.)

If `program` exits, then `sysfuck` will detect an EOF on its read handle and will exit with status 0. If `sysfuck` exits (possibly from calling `exit()` or from a segmentation fault), then SIGHUP is sent to `program` unless that process has already quit; `sfwrap` then exits with the same status as `sysfuck`.

## Caveats

* Be aware that many syscalls are wrapped by glibc, and sysfuck does not provide the wrapped version. One difference is that syscalls signal errors by returning negative error codes, where glibc sets `errno`. For example, a C call to `write()` might return -1 and set `errno` to `EBADF`; the syscall itself will simply return `-EBADF`. Sometimes this has other consequences: the direct syscall `getpriority()` returns a value `n` between 1 and 40 on success; the glibc wrapper returns `20 - n`. Consulting man pages is advised.

* There's no way to get the value of macros like EBADF. You just have to know  (or look up) what they are.

* I don't know whether it's possible to usefully use `fork()`, since you would immediately get two processes reading from and writing to the same pipes.

* `sfwrap` has no way to distinguish between `sfwrap interpreter program` and `sfwrap program`. So specifying an interpreter on the command line will produce a different `argv` to specifying an interpreter with a shebang (`#!`) line or using a compiled program. You can also specify `sfwrap` in the shebang line, like

    #! /usr/bin/sfwrap interpreter

In this case, `interpreter` will be included in the `argv`.

* When using `sfwrap`, a keyboard interrupt will by default simply cause the program to exit with status 0. I'm not sure whether this can be overriden.
