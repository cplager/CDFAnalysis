#!/usr/bin/perl

use warnings;
use strict;
use CountBallots;


while (<>) {
    chomp;
    s/\#.+//; # remove comments
    if (/:/) {
        CountBallots->new($_);
    }
}


#print "All Ballots:\n";
#CountBallots::printAllVotes();

##  my %results = CountBallots::countVotes();
##  
##  print "total: ", CountBallots::totalVotes(),"\n";
##  CountBallots::printNiceResults(%results);

CountBallots::findWinner();
