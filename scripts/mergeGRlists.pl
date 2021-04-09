#!/usr/bin/env perl

use warnings;
use strict;
use constant kMinusOne  => -1;
use constant kReallyBig => 2147483647; # kMaxInt

my %hash;

while (<>) {
   chomp;
   s/#.+$//; # rid of comments
   s/^\s+//; # leading spaces
   s/\s+$//; # following spaces
   my @words = split /\s+/, $_;
   addToHash (@words) if (@words >= 3);
}

printHash();

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
   my $aref  = $hash{$run};
   # does this run exist?
   if (! $aref) {
      # This run is not in the list at all.  Let's add it and be done
      # with it
      push @{$hash{$run}}, $pair;
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
   foreach my $run (sort {$a <=> $b} keys %hash) {
      my $aref = $hash{$run};
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
