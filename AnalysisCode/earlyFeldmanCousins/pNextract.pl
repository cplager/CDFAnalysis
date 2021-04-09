#!/usr/bin/perl

use warnings;
use strict;

open (P0, ">p0.txt") or die;
open (P1, ">p1.txt") or die;
open (P2, ">p2.txt") or die;
open (P3, ">p3.txt") or die;

my $b;
my @p;
my $saved = "true";
while (<>) {
    if (! /\S/) {
        # blank line (save things)
        if (! $saved && @p) {
            printf P0 ("%.2f %f\n", $b, $p[0]);
            printf P1 ("%.2f %f\n", $b, $p[1]);
            printf P2 ("%.2f %f\n", $b, $p[2]);
            printf P3 ("%.2f %f\n", $b, $p[3]);
        }
        $saved = "true";
        next;
    }
    if (/^b:\s*(\S+)/) {
        $b = $1;
        print "b $b\n";
        @p = ();
        $saved = "";
        next;
    }
    if (/^\s+\d\s+p(\d)\s+(\S+)/) {
        $p[$1] = $2;
        next;
    }
}
if (! $saved && @p) {
    printf P0 ("%.2f %f\n", $b, $p[0]);
    printf P1 ("%.2f %f\n", $b, $p[1]);
    printf P2 ("%.2f %f\n", $b, $p[2]);
    printf P3 ("%.2f %f\n", $b, $p[3]);
}
