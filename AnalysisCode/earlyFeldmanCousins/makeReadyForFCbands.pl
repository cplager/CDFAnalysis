#!/usr/bin/perl

use warnings;
use strict;

my @list;
while (<>) {
    chomp;
    s/\#.+//;
    my @words = split(/\s+/, $_);
    if (@words >= 3) {
        push @list, [@words[0..2]];
    }
}

foreach my $entry (@list) {
    printf ("%3d  %f\n", $entry->[0], $entry->[1]);
}

foreach my $entry (reverse @list) {
    printf ("%3d  %f\n", $entry->[0], $entry->[2]);
}

my $entry = $list[0];
printf ("%3d  %f\n", $entry->[0], $entry->[1]);
