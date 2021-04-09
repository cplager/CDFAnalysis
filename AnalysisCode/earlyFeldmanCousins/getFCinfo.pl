#!/usr/bin/perl

use warnings;
use strict;

my $prefix = "fc";
my $f68 = $prefix."68.txt";
my $f90 = $prefix."90.txt";
my $f95 = $prefix."95.txt";

open (FC68, ">$f68") or die;
open (FC90, ">$f90") or die;
open (FC95, ">$f95") or die;

while (<>) {
    next unless /^b/;
    my @parts = split (/\s+/, $_);
    next unless (@parts >= 7);
    my $limit;
    if ($parts[0] =~ /b\(([\d\.]+)\)/) {
        $limit = $1;
    } else {
        next;
    }
    my $b = $parts[1];
    (my $num = $parts[4]) =~ s/\D+//g;
    next unless (2 == $num) || (4 == $num);
    my $lower = 5;
    my $upper = $lower + $num - 1;
    foreach my $part (@parts[$lower..$upper]) {
        $part = 0 if ($part < 0);
    }
    my $stuff = join (" ", @parts[$lower..$upper]);
    if (68.28 == $limit) {
        print FC68 "$b $stuff\n";
    } elsif (90 == $limit) {
        print FC90 "$b $stuff\n";
    } elsif (95 == $limit) {
        print FC95 "$b $stuff\n";
    } 
    #print;
}

close FC68;
close FC90;
close FC95;

