#!/usr/bin/env perl

use warnings;
use strict;
use ExtUtils::Installed;

my $instmod = ExtUtils::Installed->new();
foreach my $module ($instmod->modules()) {
   my $version = $instmod->version($module) || "???";
   print "$module -- $version\n";
}

