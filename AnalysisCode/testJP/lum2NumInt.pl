#!/usr/bin/env perl

use warnings;
use strict;

use constant {kExtraPerLum     => 28.258,
              kMaxInteractions => 20};


# Option parcing
while (@ARGV && $ARGV[0] =~ /^-/) {
   (my $arg = shift) =~ s/^-+//;
   die "I don't understand '-$arg'.  Aborting.\n";
}


sub fillNumInt (\@$$) {
   my $aref = shift;
   my $instLum = shift;
   my $weight  = shift; # integrated luminosity
   my $numExtra = $instLum / kExtraPerLum;
   for my $loop (0..kMaxInteractions) {
      my $prob = $weight * poissonProb ($loop, $numExtra);
      $aref->[$loop] += $prob;
   } # for loop
}

# only returns reliable number IF $obs is an integer
sub poissonProb {
   my $obs = shift;
   my $expected = shift;
   return ($expected ** $obs) * exp (-1 * $expected) / factorial ($obs);
}

# only returns reliable number IF $obs is a non-negative integer
sub factorial {
   my $value = shift;
   if ($value <= 1) {
      return 1;
   }
   return $value * factorial ($value - 1);
}

#########################################################################

my @interactions;

while (<>) {
   chomp;
   s/^\s+//; #leading spaces
   my ($weight, $lum) = split /\s+/;
   next unless ($lum);
   if ($lum > 1e30) {
      $lum /= 1e30;
   }
   fillNumInt (@interactions, $lum, $weight);
}

# normalize so 1st bin is 1
my $factor = $interactions[0];
die unless ($factor);
foreach my $int (@interactions) {
   $int /= $factor;
}

print "@interactions\n";
