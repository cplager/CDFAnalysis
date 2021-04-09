#!/usr/bin/env perl

use warnings;
use strict;

(my $dir = $0) =~ s|[^/]+$||;
my $line = sprintf ("use lib '%s'; use CLPThreads; use PS2PNG;", $dir);
my $retval = eval $line;
die "Unable to load libraries:\n$@\n" if ($@);


my $dpi = 150;
my $useTransparent = "";
my $maxThreads = 1;

(my $script = $0) =~ s|.+/||g;
my $help = "Usage: $script -args (file1.eps)
-dpi N   => Sets dpi to N ($dpi default)
-force   => Force the creation even if the gif files already exist
-gif     => Convert to GIF
-hhelp   => This help screen
-m match => Select files that start with pattern 'match'
-max N   => Set maximum threads to N ($maxThreads default)
-no      => Only files that do not have any gif files are converted
            (i.e. does not convert files where eps file is Newer than gif file)
-only    => convert Only the files explicitly listed
-trans   => White becomes transparent
-verbose => verbose (says which files are NOT converted)\n";

my ($force, $verbose, $newOnly, $only, @masks, @addfiles);
my $ext = ".png";

# Option parcing
while (@ARGV && $ARGV[0] =~ /^-/) {
   (my $arg = shift) =~ s/^-+//;
   if ($arg =~ /^h/i) {
      print "$help\n";
      exit;
   }
   if ($arg =~ /^f/i) {
      $newOnly = "";
      $force = "true";
      next;
   }
   if ($arg =~ /^gif/i) {
      $ext = ".gif";
      next;
   }
   if ($arg =~ /^n/i) {
      $newOnly = "true";
      $force = "";
      next;
   }   
   if ($arg =~ /^v/i) {
      $verbose = "true";
      next;
   }
   if ($arg =~ /^o/i) {
      $only = "true";
      next;
   }
   if ($arg =~ /^tr/i) {
      $useTransparent = "white";
      next;
   }
   if ($arg =~ /^max/i) {
      $maxThreads = shift @ARGV;
      next;
   }
   if ($arg =~ /^dpi/i) {
      $dpi = shift @ARGV;
      next;
   }
   if ($arg =~ /^m/i) {
      my $mask = shift @ARGV;
      push @masks, $mask;
      next;
   }
   die "I don't understand '-$arg'.  Aborting.\n";
}

if (! @masks) {
   push @masks, "";
}

setOptions ("dpi",         $dpi);
setOptions ("transparent", $useTransparent);


my @epsFiles;
if (! $only) {
   foreach my $mask (@masks) {
      push @epsFiles, glob ("$mask*.eps");
   }
}

push @epsFiles, @addfiles;

# Loop over all eps files and add the appropriate ones to the list
my $thread = CLPThreads->new (\&convertPs2Png);
foreach my $eps (@epsFiles) {
   (my $gif = $eps) =~ s|\.eps$|$ext|i;
   next if ($gif eq $eps);
   if (! -e $gif || ((-M $eps < -M $gif) && !$newOnly) || $force) {
      if ($newOnly && -e $gif) {
         warn "Danger: $gif\n";
         next;
      }
      $thread->addJob ($eps, $gif);
      #convertPs2Png ($eps, $gif);
   } else {
      print "$gif is newer than $eps\n" if $verbose;
   }
} # foreach eps

$thread->run ($maxThreads);

