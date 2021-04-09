#!/usr/bin/perl

use warnings;
use strict;

my $trials = 1_000_000;
for (my $xsec = 2; $xsec <= 15; ++$xsec) {
    my $output = `./calcCorrelations.exe $xsec $trials`;
    if ($output =~ /background\s*(\S+)/) {
        printf "%2d %s\n", $xsec, $1;
    }
}
