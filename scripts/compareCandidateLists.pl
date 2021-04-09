#!/usr/bin/perl

use warnings;
use strict;

# Load Candidate.pm.  Assume it is either in the current directory or
# the same directory as the script.
(my $dir = $0) =~ s|[^/]+$||;
my $line = sprintf ("use lib '%s'; use Candidate;", $dir);
my $retval = eval $line;
die "Unable to load Candidate:\n$@\n" if ($@);

die "Usage: $0 first.log second.log\n" unless (@ARGV >= 2);

# get arguments first
my @vetolist;
while ($ARGV[0] && ($ARGV[0] =~ /^-+/)) {
   (my $arg = shift @ARGV) =~ s/^-+//;
   if ($arg =~ /^h/i) {
      help();
   }        
   if ($arg =~ /^veto/i) {
      my $word = shift @ARGV;
      push @vetolist, $word;
      next;
   }
   if ($arg =~ /^data/i) {
      Candidate::setNoEntryMode("true");
      next;
   }
   if ($arg =~ /^noentry/i) {
      Candidate::setNoEntryMode("true");
      next;
   }
}

my $first = shift;
my $second = shift;
my %firstHash = my %secondHash = ();
my (@firstOnly, @secondOnly, @both);
open (FIRST, $first)   or die "Can't open '$first'.  Aborting.\n";
open (SECOND, $second) or die "Can't open '$second'.  Aborting.\n";

FIRST: while (<FIRST>) {
   chomp;
   tr/A-Z/a-z/;
   # is this only a comment line
   if (/^\s*#/) {
      s/^\s*//;
      # o.k. Is this a definition line
      if (/^##\s+/) {
         # now do something with it
         #print "setting names $'\n";
         my $names = $';
         Candidate::setNames ($names);
         if ($names !~ /\bentry\b/) {
            # automatically set noentry mode
            Candidate::setNoEntryMode("true");
         }
         next;
      }
      # an accuracy line
      if (/^#\s*acc\w*\s+(\S+)\s+(\S+)/) {
         Candidate::setAccuracy ($1, $2);
         next;
      }
      # an equivalents line
      if (/^#\s*equiv\w*\s+(\S.+)/) {
         Candidate::setEquivalents ($1);
         next;
      }
      # a swap line
      if (/^#\s*swap\w*\s+(\S+)\s+(\S+)/) {
         Candidate::addSwap ($1, $2);
         next;
      }
      # veto list
      if (/^#\s*veto\s+(\S+)/) {
         push @vetolist, $1;
      }
      next;
   }
   foreach my $word (@vetolist) {
      if (/$word/i) {
         # ahh, a forbidden word
         next FIRST;
      }
   }
   next unless (/\S/);          # make sure there is something there
   my $cand = Candidate->new($_);
   if (! $cand->isOK) {
      print "problem $_\n";
   }
   next unless $cand->isOK;
   push @{ $firstHash{ $cand->key } },  $cand;
}

SECOND: while (<SECOND>) {
   chomp;
   tr/A-Z/a-z/;
   # is this only a comment line
   if (/^\s*#/) {
      next;
   }
   foreach my $word (@vetolist) {
      if (/$word/i) {
         # ahh, a forbidden word
         next SECOND;
      }
   }
   next unless (/\S/);          # make sure there is something there
   my $cand = Candidate->new($_);
   next unless $cand->isOK;
   push @{ $secondHash{ $cand->key } }, $cand;
}

# find First only and both
foreach my $key (sort keys %firstHash) {
   if (exists $secondHash{$key}) {
	  # both
	  push @both, $key;
   } else {
	  # first only
	  my $aref = $firstHash{$key};
	  foreach my $item (@$aref) {
		 push @firstOnly, "$item";
	  }                         # foreach item
   }                            # else if
}                               # foreach key

# find Second only
foreach my $key (sort keys %secondHash) {
   if (! exists $firstHash{$key}) {
	  # Second only
	  my $aref = $secondHash{$key};
	  foreach my $item (@$aref) {
		 push @secondOnly, "$item";
	  }                         # foreach item
   }                            # else if
}                               # foreach key

my $totalGood = 0;
my @badList = ();
if (@both) {
   foreach my $key (@both) {
	  my $first = $firstHash{$key};
	  my $second = $secondHash{$key};
	  my ($firstString, $secondString, $good, $bad) =
		Candidate::checkEquivalents($first, $second);
	  push @firstOnly,  split (/\n/, $firstString)  if $firstString;
	  push @secondOnly, split (/\n/, $secondString) if $secondString;
	  push @badList,    split (/\n/, $bad)          if $bad;
	  $totalGood += $good;
   }
   print "\n";
}

if (@firstOnly) {
   print "# First Only:\n";
   print join ("\n", sort @firstOnly);
   print "\n\n";

}

if (@secondOnly) {   
   print "# Second Only:\n";
   print join ("\n", sort @secondOnly);
   print "\n\n";
}

if (@badList) {   
   print "# Mismatched:\n";
   print join ("\n", sort @badList);
   print "\n\n";
}

warn sprintf("First events %d, Second events %d, First only %d, Second only %d, both %d (good %d, bad %d)\n",
             scalar keys %firstHash, scalar keys %secondHash, 
             scalar @firstOnly, scalar @secondOnly, $totalGood + @badList,
			 $totalGood, scalar @badList);

############################################################################
sub help {
   (my $script = $0) =~ s|.+/||g;
   print "Usage: $script [--options] file1.candlist file2.candlist\n";
   exit;
}


##  my $cand  = Candidate->new("141572 2 e2 TCE        e3   TCE        91.89");
##  my $cand2 = Candidate->new("141572 2 e3 TCE        e2   TCE        91.89");
##  my $other = Candidate->new($cand);
##  
##  print "cand  $cand\nother $other\n";
##  
##  if (my $diff = Candidate::areDifferent($cand, $cand2)) {
##     print "different: $diff\n";
##  } else {
##     print "same\n";
##  }
