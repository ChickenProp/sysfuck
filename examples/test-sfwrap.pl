#! /usr/bin/perl
# This simply prints one line, then another to fd 5, then the first three
# letters of its argv[0].

use warnings;
use strict;

$|++;
print "this should get printed\n";
print "\0stdout", map { chr } qw(0 1 5);
print "with a null byte (\0\0) on fd 5\n";
print "\0argv", map { chr } qw(0 4 0 0 0 0);

my $n = read STDIN, (my $ptr), 4;

print "\0write",
  (map { chr } qw(0 12 1 0 0 0)),
  $ptr,
  (map { chr } qw(3 0 0 0));
