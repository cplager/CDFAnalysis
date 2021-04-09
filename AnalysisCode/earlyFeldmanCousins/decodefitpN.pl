#!/usr/bin/perl

use warnings;
use strict;

my $pValue;
my @p;
while (<>) {
    if (/^p(\d)/) {
        $pValue = $1;
        next;
    }
    if (/^\s*\d+\s*p(\d)\s+(\S+)/) {
        $p[$1] = $2;
        next;
    }
    if ((/\*{5,}/ || (!/\S/)) && @p) {
        print "Double_t p$pValue\_[10] = {", join(", ", @p), "};\n\n";
        @p = ();
        next;
    }
}
