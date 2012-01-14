#! /usr/bin/perl
use warnings;
use strict;

use HOP::Lexer 'make_lexer';
use v5.10.0;

$|++;

sub hex2str {
	my $hex = shift;
	
	if (length($hex) % 2) {
		$hex = '0' . $hex;
	}
	pack( 'H' . length($hex), $hex);
}

my @lexer_tokens = (
		    [ 'STR', qr/'.*?'|".*?"/, \&string_token ],
		    [ 'HEX', qr/[0-9abcdefABCDEF]+/ ],
		    [ 'SPACE', qr/\s*/, sub { () } ],
		   );

sub string_token {
	my ($name, $str) = @_;
	$str =~ s/^['"]//;
	$str =~ s/['"]$//;
	return [ $name, $str ];
}

my $lexer = make_lexer(sub { <> }, @lexer_tokens);

while (my $tok = $lexer->()) {
	last unless $tok;

	my ($type, $content) = @$tok;
	given ($type) {
		when('HEX') { print hex2str($content); }
		when('STR') { print $content; }
	}
}
