#! /bin/sh
# Generates the file 'str_to_syscall.c', which contains a function
# 'str_to_syscall' which is used in sysfuck.c.

echo '#include <sys/syscall.h>'  \
    | cpp -fdirectives-only      \
    | perl gen_str_to_syscall.pl \
    > str_to_syscall.c
