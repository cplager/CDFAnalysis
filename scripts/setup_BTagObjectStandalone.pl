#!/usr/bin/env perl

use strict;
use warnings;

######################################################################
## This script is designed to move "superfluous" files out of the way
## to allow the standalone makefile 'Makefile.standalone' to
## successfully compilie this library into a shared library.  As it is
## setup, the script moves ALL source (.cc, .hh) files out of the way
## EXCEPT those that are explicitly listed in '@filesToKeep'.
##
## Charles Plager - 060818


# name of extraDir 
my $extraDir = "superfluous";

# directories/files to move (add to as necessary)
my @dirsToMove = qw (mistag GNUmakefile);

# files in include and src directories to keep (add to as necessary)
my @filesToKeep = qw (PredictedDistribution TH6 TMistagMatrix
                      jetMistagSummer2003 jetMistagWinter2004);

# directory and extention maps used with filesToKeep (add to as
# necessary)
my %toKeepHash = 
  # directory     extention
  (src         => ".cc",
   BTagObjects => ".hh",
   dict        => "_linkdef.h");

# cleanup commands (executed in order; add to as necessary)
my @cleanupCommands = 
  (
   "rm -f include",
   "ln -s BTagObjects include",
   "ln -s Makefile.standalone Makefile",
   # move this script out of the way so it doesn't get run again.
   "mv $0 $extraDir",
  );

####################################################################
##      You shouldn't need to modify anything below this line.    ##
####################################################################

# make sure I'm in the right place
chomp (my $current = `pwd`);

die "You must be in parentDir/BTagObjects to run this script.\n"
  if ($current !~ m|/BTagObjects$| ||
      $current =~ m|/BTagObjects/BTagObjects$|);


mkdir $extraDir;
mkdir "objects";

# move superfluous directories
foreach my $dir (@dirsToMove) {
   system "mv $dir $extraDir/";
}

# move all src and include files except those you want to keep
foreach my $dir (keys %toKeepHash) {
   my $ext = $toKeepHash{$dir};
   mkdir "$extraDir/$dir";
   chdir $dir;
   my @files = glob ("*$ext");
   foreach my $file (@files) {
      (my $base = $file) =~ s/$ext$//;
      my $found = "";
      foreach my $keep (@filesToKeep) {
         if ($base eq $keep) {
            $found = "true";
            last;
         } # if match
      } # foreach keep
      # if we found this file on the list, go on to the next file
      next if ($found);
      # if we're still here, move the file out of the way
      system "mv $file ../$extraDir/$dir";
   } # foreach file
   chdir $current;
}

# cleanup commands
foreach my $command (@cleanupCommands) {
   system $command;
}
