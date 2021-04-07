#!/usr/bin/env perl

use warnings;
use strict;
use Data::Dumper; # very useful for quick debugging
use constant LF => 0;
use constant ME => 1;
use constant NN => 2;


my $syst     = "fit syst";
my $no       = "no fit syst";
my $nofit    = "";
my $pull     = "";
my $smear    = "";
my $pullmode = 0;
my $beta     = 0;
my $single   = "";
my %errorsHoA;

# Option parcing
while (@ARGV && $ARGV[0] =~ /^-/) {
   (my $arg = shift) =~ s/^-+//;
   if ($arg =~ /^pullmode/) {
      $pullmode = shift || 0;
      next;
   }
   if ($arg =~ /^pull/) {
      $pull = "true";
      next;
   }
   if ($arg =~ /^smear/) {
      $smear = "true";
      next;
   }
   if ($arg =~ /^nofit/) {
      $syst  = "syst";
      $no    = "no syst";
      $nofit = 1;
      next;
   }
   if ($arg =~ /^beta/i) {
      $beta = shift;
      $single = "true";
      next;
   }
   die "I don't understand '-$arg'.  Aborting.\n";
}

loadErrors() if ($pull);

my $prefix = "k";
my $which  = "Mean";
my $suffix = "Unbias[kNumRows][kNumMeas] =";
if (! $nofit) {
   $prefix = "kFit";
}
if ($pull) {
   $which = "Pull";
   if (1 == $pullmode) {
      $which .= "Lower";
   } elsif (2 == $pullmode) {
      $which .= "Ave";
   } elsif (3 == $pullmode) {
      $which .= "Upper";
   } # else $pullmode
} elsif ($smear) {
   $which = "smear";
   $syst  = "fit smear";
   $no    = "smear";
}

print "const double $prefix$which$suffix\n{\n" unless $single;

my ($lf, $lfno, $me, $meno, $nn, $nnno);
while (<>) {
   # beta
   if (/beta\s+(\S+)/ && ! $single) {
      $beta = $1;
      next;
   }
   # LF
   if (/LF $syst\s+(\S+)/) {
      $lf = $1;
      next;
   }
   # LF no
   if (/LF $no\s+(\S+)/) {
      $lfno = $1;
      next;
   }
   # ME
   if (/ME $syst\s+(\S+)/) {
      $me = $1;
      next;
   }
   # ME no
   if (/ME $no\s+(\S+)/) {
      $meno = $1;
      next;
   }
   # NN
   if (/NN $syst\s+(\S+)/) {
      $nn = $1;
      next;
   }
   # NN no
   if (/NN $no\s+(\S+)/) {
      $nnno = $1;
      next;
   }
   # print
   if (! /\S/ && $beta && ! $single) {
      printLine();
      $beta = 0;
   } # if empty line
} # while <>
printLine() if ($beta);
print "};\n" unless $single;

sub printLine {
   if ($pull) {
      my $key = sprintf ("%0.1f", $beta);
      my $aref = $errorsHoA{$key} || die;
      my ($lfErr, $meErr, $nnErr) = (1, 1, 1);
      if (1 == $pullmode) {
         # lower error
         $lfErr = $aref->[2 * LF];
         $meErr = $aref->[2 * ME];
         $nnErr = $aref->[2 * NN];
      } elsif (2 == $pullmode) {
         $lfErr = 0.5 * ($aref->[2 * LF] + $aref->[2 * LF + 1]);
         $meErr = 0.5 * ($aref->[2 * ME] + $aref->[2 * ME + 1]);
         $nnErr = 0.5 * ($aref->[2 * NN] + $aref->[2 * NN + 1]);
      } elsif (3 == $pullmode) {
         # upper error
         $lfErr = $aref->[2 * LF + 1];
         $meErr = $aref->[2 * ME + 1];
         $nnErr = $aref->[2 * NN + 1];
      }
      if ($single) {
         printf("  unbiasLF=%.4f unbiasME=%.4f unbiasNN=%.4f\n",
                $lf * $lfErr, $me * $meErr, $nn * $meErr);
      } else {
         printf("   {%.4f, %.4f, %.4f}, //  %.1f\n", 
                $lf * $lfErr, $me * $meErr, $nn * $meErr, $beta);
      }
   } else {
      printf("   {%.4f, %.4f, %.4f}, //  %.1f\n", 
             $lf - $lfno, $me - $meno, $nn - $nnno, $beta);
   }
} 

sub loadErrors {
   open (ERRORS, "<", "posNegErrVsBeta.txt") or die;
   while (<ERRORS>) {
      s/\#.+$//;
      next unless (/\S/);
      chomp;
      my @words = split /\s+/, $_;
      my $key = shift @words;
      next unless (@words >= 6);
      push @{$errorsHoA{$key}}, @words;
   } # while <ERRORS>
   ## # for debugging only
   ## print Dumper (%errorsHoA),"\n";
}
