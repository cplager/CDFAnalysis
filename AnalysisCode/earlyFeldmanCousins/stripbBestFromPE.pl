#!/usr/bin/perl

use strict;
use warnings;

my $b;
my $const;
my $maxb;
while (<>) {
    if (/Fii?tt?ing.+:\s(\S+)$/) {
        $b = $1;
        next;
    }
    if (/1\s+Constant\s+(\S+)/) {
        $const = $1;
        next;
    }
    if (/2\s+Max_bValue\s+(\S+)/) {
        $maxb = $1;
        printf ("%.3f  %.4f %6.1f\n", $b, $maxb, $const);
        next;
    }
}
