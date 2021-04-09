#!/usr/bin/env perl

use strict;
use warnings;

use constant kMinusOne  => -1;
use constant kReallyBig => 2147483647; # kMaxInt

my $corrFactor = 1.019;
my $lumErr = 0.06;


(my $scriptname = $0) =~ s|.+/||;

# Option parcing
while (@ARGV && $ARGV[0] =~ /^-/) {
   (my $arg = shift) =~ s/^-+//;
   die "I don't understand '-$arg'.  Aborting.\n";
}

die "Usage: $scriptname goodrun.list lum1.output [lumi2.output] ...\n"
  unless (@ARGV >= 2);

my $grl = shift;

###########################
## Load Luminosity Files ##
###########################
my %lumiHash = ();   
while (<>) {
   chomp;
   s/#.+//;
   if (/^(\d+)\s*:\s*(.+)/) {
      my $run = $1;
      my $rest = $2;
      push @{$lumiHash{$run}}, split (/\s+/, $rest);
   }
}

########################
## Load Good Run List ##
########################
open (GRL, "<", $grl) or die "Can't open good run list '$grl'\n.";
my %grlHash;
while (<GRL>) {
   chomp;
   s/#.+$//; # rid of comments
   s/^\s+//; # leading spaces
   s/\s+$//; # following spaces
   my @words = split /\s+/, $_;
   addToHash (@words) if (@words >= 3);
}

##########################
## Calculate Luminosity ##
##########################
my $lumi = 0.;
foreach my $run (sort keys %grlHash) {
   # first, make sure we have lumi information for this run
   next unless ( $lumiHash{$run} );
   # get the GRL an Lumi arrays
   my $grlAref  = $grlHash{$run};
   my $lumiAref = $lumiHash{$run};
   my $lumiMax = scalar @$lumiAref - 1;
   foreach my $pair (@$grlAref) {
      my $lower = $pair->[0];
      my $upper = $pair->[1];
      # if $lower > $lumiMax, don't bother
      next if ($lower > $lumiMax);
      # remember, upper won't be -1, but rather kReallyBig, so
      if ($upper > $lumiMax) {
         $upper = $lumiMax;
      } # if too big
      for my $loop ($lower..$upper) {
         $lumi += $lumiAref->[$loop];
      }
   }
}

my $prettyLumi = commify (sprintf ("%.2f", $lumi / 1000));

printf "Uncorrected lumi: %10s pb\n", $prettyLumi;
###########################################################################


sub addToHash {
   my $run  = shift;
   my $lower = shift;
   my $upper = shift;
   # change the kMinusOne to kReallyBig to make the logic simpler
   if (kMinusOne == $upper) {
      $upper = kReallyBig;
   }
   my $pair = [];
   push @$pair, $lower, $upper;
   my $aref  = $grlHash{$run};
   # does this run exist?
   if (! $aref) {
      # This run is not in the list at all.  Let's add it and be done
      # with it
      push @{$grlHash{$run}}, $pair;
      return;
   }
   # If we're still here, that means that this run is already in the
   # good run list.  If the current list already has the entire range,
   # then don't bother.
   return if ( containsEntireRange ($aref) );
   # If the new entry is the entire range, then put that in and be
   # done with it.
   if ( entireRange ($pair) ) {
      @$aref = ($pair);
   }
   # If we are still here, we need to merge the entries
   my @newlist = ();
   foreach my $entry (@$aref) {
      my $overlapPair = overlap ($pair, $entry);
      if ($overlapPair) {
         $pair = $overlapPair;
      } else {
         push @newlist, $entry;
      }
   } # foreach my $entry
   push @newlist, $pair;
   @$aref = ();
   push @$aref, sort pairWise @newlist;
}

sub printHash {
   foreach my $run (sort {$a <=> $b} keys %grlHash) {
      my $aref = $grlHash{$run};
      foreach my $pair (@$aref) {
         my $lower = $pair->[0];
         my $upper = $pair->[1];
         if (kReallyBig == $upper) {
            $upper = kMinusOne
         }
         print "$run $lower $upper\n";
      } # foreach pair
   } # foreach run
}

sub pairWise {
   return $a->[0] <=> $b->[0] || $a->[1] <=> $b->[1];
}

sub containsEntireRange {
   my $aref = shift;
   # loop over all entries in this range
   foreach my $entry (@$aref) {
      return "true" if ( entireRange ($entry) );
   } # foreach entry
   # if we're still here, then we didn't find it
   return;        
}

sub entireRange {
   my $pair = shift;
   return "true" if (0 == $pair->[0] && kReallyBig == $pair->[1]);
   return;
}

sub overlap {
   my $pair1 = shift;
   my $pair2 = shift;
   # does the beginning or end of pair2 fall in between the range of
   # pair1?
   if ( ($pair1->[0] - 1 <= $pair2->[0]) &&
        ($pair1->[1] + 1 >= $pair2->[0]) ||
        ($pair1->[0] - 1 <= $pair2->[1]) &&
        ($pair1->[1] + 1 >= $pair2->[1]) ) {
      # we've got an overlap
      # Who has a smaller first piece
      if ($pair2->[0] < $pair1->[0]) {
         $pair1->[0] = $pair2->[0];
      }
      # Who has a larger second piece
      if ($pair2->[1] > $pair1->[1]) {
         $pair1->[1] = $pair2->[1];
      }
      return $pair1;
   }
   # if we're here, we don't have an overlap, so return false
   return;
}

sub stringPair {
   my $pair = shift;
   my $retval = $pair->[0]." ".$pair->[1];
}

sub commify {
   my $text = reverse $_[0];
   $text =~ s/(\d\d\d)(?=\d)(?!\d*\.)/$1,/g;
   return scalar reverse $text;
}
