#!/usr/bin/env perl

use strict;
use warnings;

my $age = 1;

my $normalization = 1;
my $unit = "days";
# Option parcing
while (@ARGV && $ARGV[0] =~ /^-/) {
   (my $arg = shift) =~ s/^-+//;
   if ($arg =~ /^day/i) {
      $age = shift || -1;
      $unit = "days";
      next;
   }
   if ($arg =~ /^hour/i) {
      $age = shift || -1;
      $age /= 24;
      $normalization = 24;
      $unit = "hours";
      next;
   }
   if ($arg =~ /^min/i) {
      $age = shift || -1;
      $age /= 24 * 60;
      $normalization = 24 * 60;
      $unit = "minutes";
      next;
   }
   if ($arg =~ /^sec/i) {
      $age = shift || -1;
      $age /= 24 * 60 * 60;
      $normalization = 24 * 60 * 60;
      $unit = "second";
      next;
   }
   die "I don't understand '-$arg'.  Aborting.\n";
}

my $dir = shift || "";

die "Usage: $0 [-args] targetDir file1 [file2]\n" if (! -d $dir);

die "You must give a valid age of files\n" if ($age < 0);

foreach my $file (@ARGV) {
   my $thisage = -M $file;
   if ($thisage > $age) {
      $thisage *= $normalization;
      printf "moving %s age %.2f %s\n", $file, $thisage, $unit;
      system "mv $file $dir";
   }
}
