#! /usr/bin/perl
# This program is intended to be called from gen_str_to_syscall.sh .
# As input, it takes partially-preprocessed C source code. It extracts the
# __NR_<foo> macros and uses them to write a function 'str_to_syscall' which is
# written to stdout.

use warnings;
use strict;

sub printcase {
	my ($name) = @_;
	print qq{\tif (! strcmp("$name", str))\n\t\treturn __NR_$name;\n}
}

print "int str_to_syscall(const char *str) {\n";

while (<>) {
	if ( /^#define\s+__NR_(\w+)/ ) {
		printcase($1);
	}
}

print "\treturn -1;\n}\n";
