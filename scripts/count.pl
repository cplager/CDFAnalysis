#!/usr/bin/env perl

use warnings;
use strict;

(my $exe = $0) =~ s|.+/||g;
my $lower = 1;
my $form = "%d";
my $reverse = "";
while (@ARGV && $ARGV[0] =~ /^\-/) {
   (my $arg = shift) =~ s/^-+//;
   if ($arg =~ /^f/i) {
      $form = shift;
      next;
   }
   if ($arg =~ /^s/i) {
      $lower = shift;
      next;
   }
   if ($arg =~ /^r/i) {
      $reverse = "true";
      next;
   }
   die "Don't understand '-$arg'\n";
}

die "Usage: $exe [-options] upper [lower]\nOptions:
-f form   => use 'form' format for printing
-r        => reverse
-s XX     => start at XX
" unless @ARGV;


(my $upper = shift) =~ s/\D//;
$lower = shift if (@ARGV);
$lower =~ s/\D//g;

die "upper '$upper' and lower '$lower' do not make sense\n" 
  unless ($lower < $upper);

## if ($reverse) {
##    ($lower, $upper) = ($upper, $lower);
## }

if ($reverse) {
   for (my $loop = $upper; $loop >= $lower; --$loop) {
      printf "$form ", $loop;
   }
} else {
   for my $loop ($lower..$upper) {
      printf "$form ", $loop;
   }
}

print "\n";
