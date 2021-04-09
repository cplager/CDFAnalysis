#!/usr/bin/perl

use warnings;
use strict;

my @lines;
while (<>) {
    unshift @lines, $_;
}
print @lines;
