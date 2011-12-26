#! /usr/bin/perl
use warnings;
use strict;

$|++;
print "argv", map { chr } qw(0 4 0 0 0 0);

my $n = read STDIN, (my $str), 4;

print "write",
  (map { chr } qw(0 12 4 0 0 0)),
  $str,
  (map { chr } qw(3 0 0 0));
