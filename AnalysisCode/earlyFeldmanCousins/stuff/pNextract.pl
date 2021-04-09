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
        if (! $saved) {
            print P0 "$b $p[0]\n";
            print P1 "$b $p[1]\n";
            print P2 "$b $p[2]\n";
            print P3 "$b $p[3]\n";
        }
        $saved = "true";
        next;
    }
    if (/^b: (\S+)/) {
        $b = $1;
        $saved = "";
        next;
    }
    if (/^\s+\d\s+p(\d)\s+(\S+)/) {
        $p[$1] = $2;
        next;
    }
}
