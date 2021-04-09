#!/usr/bin/perl

use warnings;
use strict;
use CountBallots;

my $first = CountBallots->new("fred:1073060213:liss:kim:proudfoot");
my $second = CountBallots->new("barney:1073067281:proudfoot:kim:liss");
my $second = CountBallots->new("barney:1073067281:proudfoot:kim:liss");
my $third = CountBallots->new("homer:1073076704:kim:proudfoot:no preference");
my $fourth = CountBallots->new("Evelyn:1073076707:kim:proudfoot:no preference");
my $fourth = CountBallots->new("Evelyn:1073076707:kim:proudfoot:no preference");


print "first: $first\n";
CountBallots::printAllVotes();

my %results = CountBallots::countVotes();

print "total: ", CountBallots::totalVotes(),"\n";
CountBallots::printNiceResults(%results);

CountBallots::findWinner();
