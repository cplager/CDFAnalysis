#!/usr/bin/env perl

use strict;
use warnings;


my ($all, $diffOnly, $summOnly, $quiet, $data, $noentry) = ();
$diffOnly = "true";
my $help = "Usage: $0 [-options] first.log second.log
-all     => shows differences as well as similar events
-data    => Use run and event number only (not entryIndex)
-diff    => shows differences only
-help    => this screen
-noentry => No entry index is given, list starts with run number
-quiet   => only print run and event numbers, not rest of lines
-summ    => returns only summary of results
-veto XX => vetos lines that contain 'XX'\n";


#####################################################################

sub printStuff (\@){
   my $aRef = shift;
   foreach my $entry (@$aRef) {
      print "$entry\n";
   }
}

sub printStuffHash (\@\%){
   my $aRef = shift;
   my $href = shift;
   foreach my $entry (@$aRef) {
      if ($quiet) {
         print "$entry\n";
      } else {
         my $rest = $href->{$entry};
         $entry =~ s|,||g;
         if ($data) {
            my ($run, $event) = split (/\s+/, $entry);
            printf ("%6d, %8d - %s\n", $run, $event, $rest);
         } else {
            my ($entryIndex, $run, $event) = split (/\s+/, $entry);
            printf ("%7d : %6d, %8d - %s\n", $entryIndex, $run, $event, $rest);
         }
      }
   }
}

sub sortReasonably {
   # $a and $b
   (my $acopy = $a) =~ s/[^\d\s]//g;
   (my $bcopy = $b) =~ s/[^\d\s]//g;
   my @aparts = split /\s+/, $acopy;
   my @bparts = split /\s+/, $bcopy;
   while (@aparts && @bparts) {
      my $apart = shift @aparts;
      my $bpart = shift @bparts;
      my $comp = $apart <=> $bpart;
      return $comp if $comp;
   }
   # if we're still here, then we can't tell the difference
   return 0;
}
#####################################################################



my @vetolist;
while ($ARGV[0] && $ARGV[0] =~ /^-{1,2}/) {
   (my $arg = shift @ARGV) =~ s/^-+//;
   if ($arg =~ /^h/i) {
      die "$help\n";
   }        
   if ($arg =~ /^veto/i) {
      my $word = shift @ARGV;
      push @vetolist, $word;
      next;
   }
   if ($arg =~ /^data/i) {
      $data = "true";
      next;
   }
   if ($arg =~ /^all/i) {
      $all = "true";
      $diffOnly = "";
      next;
   }
   if ($arg =~ /^diff/i) {
      $diffOnly = "true";
      next;
   }
   if ($arg =~ /^quiet/i) {
      $quiet = "true";
      next;
   }
   if ($arg =~ /^summ/i) {
      $summOnly = "true";
      next;
   }
   if ($arg =~ /^noe/i) {
      $noentry = "true";
      $data    = "true";
      next;
   }
}

die "Usage: $0 first.log second.log\n" unless (@ARGV >= 2);

my $first = shift;
my $second = shift;
my %firstHash = my %secondHash = my %file = ();
my @secondOnly = my @firstOnly = my @both = ();

open (FIRST, $first) or die;
open (SECOND, $second) or die;

my $key = "";
FIRST: while (<FIRST>) {
   chomp;
   s/\#.+//g;                   # remove comments;
   foreach my $word (@vetolist) {
      if (/$word/i) {
         # ahh, a forbidden word
         next FIRST;
      }
   }   
   if ($noentry) {
      if (/\D*(\d+)\D+(\d+)/i) {
         $key = "$1, $2";
         (my $rest = $') =~ s/^\s*//;
         $firstHash{$key} = $rest;
      }
   }
   elsif ($data) {
      if (/\D*(\d+)\D+(\d+)\D+(\d+)/i) {
         $key = "$2, $3";
         (my $rest = $') =~ s/^\s*//;
         $firstHash{$key} = $rest;
      }
   } else {
      if (/\D*(\d+)\D+(\d+)\D+(\d+)/i) {
         $key = "$1, $2, $3";
         (my $rest = $') =~ s/^\s*//;
         $firstHash{$key} = $rest;
      }
   }
}

SECOND: while (<SECOND>) {
   chomp;
   s/\#.+//g;                   # remove comments;
   foreach my $word (@vetolist) {
      if (/$word/i) {
         # ahh, a forbidden word
         next SECOND;
      }
   }
   if ($noentry) {
      if (/\D*(\d+)\D+(\d+)/i) {
         $key = "$1, $2";
         (my $rest = $') =~ s/^\s*//;
         $secondHash{$key} = $rest;
      }
   }
   elsif ($data) {
      if (/\D*(\d+)\D+(\d+)\D+(\d+)/i) {
         $key = "$2, $3";
         (my $rest = $') =~ s/^\s*//;
         $secondHash{$key} = $rest;
      }
   } else {
      if (/\D*(\d+)\D+(\d+)\D+(\d+)/i) {
         $key = "$1, $2, $3";
         (my $rest = $') =~ s/^\s*//;
         $secondHash{$key} = $rest;
      }
   }
}

# find First only and both
foreach my $key (keys %firstHash) {
   if (exists $secondHash{$key}) {
      # both
      push @both, $key;
   } else {
      # first only
      push @firstOnly, $key;
   }
}

# find Second only
foreach my $key (keys %secondHash) {
   if (! exists $firstHash{$key}) {
      # Second only
      push @secondOnly, $key;
   }
}



if (@firstOnly && ! $summOnly) {
   print "# First Only:\n";
   @firstOnly = sort sortReasonably @firstOnly;
   printStuffHash (@firstOnly, %firstHash);
   print "\n";

}

if (@secondOnly && ! $summOnly) {
   print "# Second Only:\n";
   @secondOnly = sort sortReasonably @secondOnly;
   printStuffHash (@secondOnly, %secondHash);
   print "\n";
}

if (@both && ! $diffOnly && ! $summOnly) {
   print "# Both:\n";
   printStuff (@both);
   print "\n";
}

printf("First only %d, Second only %d, both %d\n",
       scalar @firstOnly, scalar @secondOnly, scalar @both);


