#!/usr/bin/env perl

use strict;
use warnings;

## This script checks out the required packages, both from TopMods and
## other packages to set up the top analysis framework.  Except for
## bug fixes, you should only need to modify the variables in the
## first part of the script to add any changes.
##
## Charles Plager 060817


# Minimum CDF release (numbers only)
my $minRelease = "6.1.4"; 
# Current directory is defined as 'parentDir'
my $parentDir = $ENV{PWD};
# TopMods tag
my $topModsTag = "";

# HighLevelObjects tags.  The first one is what is setup by default
my @HLOTags = qw (top_614_1invfb_a top_533_nt5);
my %HLOFiles = 
  (
   # File in TopMods                           Target directory in
   #                                           'HighLevelObjects/'
   "src/HighLevelObjects.cc"                => "src",
   "TopMods/HighLevelObjects.hh"            => "HighLevelObjects",
   "TopMods/CollectionAndViewTypeFinder.hh" => "HighLevelObjects",
   "TopMods/TopLepType.hh"                  => "HighLevelObjects",
   "dict/HighLevelObjects_linkdef.h"        => "dict",
  );

# Which versions of which packages to add. A blank ("") version means
# take it from the head.
my %packages = 
  (
   # Package             Version
   JetUser            => "jetCorr06b",
   BTagObjects        => "",
   RootFileCollection => "",
  );

# Any commands needed to setup a given package.  Will be run in
# $parentDir/$packge directory
my %packageCommands = 
  (
   # Package          Command
   JetUser     => "rm -f GNUmakefile; mv Makefile.standalone Makefile;".
                  "ln -s ../JetUser/libJetUser.so $parentDir/shlib",
   BTagObjects => "./setup_BTagObjectStandalone.pl",
  );

# Any directories that need to be setup
my @necessaryDirs = qw (include shlib scripts filelists good_run_lists);

# Any last cleanup commands (from the 'parentDir')
my @lastCommands =
(
 "ln -s ../HighLevelObjects/HighLevelObjects include/TopMods",
);

#####################################################################
#####################################################################
#####################################################################

# You shouldn't need to modify anything from here on down.

my $release  = $ENV{DEFAULT_SRT_BASE_RELEASE} || "0";
my $tempdir  = "tempdir.$$";

# make sure they have at least '$minRelease' setup
(my $numRelease = $minRelease) =~ s/\D//g;
$release =~ s/\D//g;
die "You must setup cdfsoft $minRelease or greater\n" 
  if ($release < $numRelease);

# setup the necessary directories
foreach my $dir (@necessaryDirs) {
   mkdir $dir if (! -d $dir);
}

# remove 'CVS' directory from parentDir
system "rm -rf ./CVS" if (-d "./CVS");

# Check out the packages listed above
foreach my $package (sort keys %packages) {
   chdir $parentDir;
   my $version = $packages{$package};
   if ($version) {
      $version = "-r $version";
   }
   my $cvs = "cvs checkout $version $package";
   system $cvs;
   my $command = $packageCommands{$package};
   if ($command) {
      chdir $package;
      print "\n\nExecuting commands for $package\n";
      system "pwd";
      system $command;
   }
} # foreach package

######################################
## Get TopMods and HighLevelObjects ##
######################################
chdir $parentDir;
getVersionFromCVS ("TopMods/AnalysisCode", "$parentDir/");
(my $defaultTag = $HLOTags[0]) =~ s/top_//;
foreach my $file (keys %HLOFiles) {
   my $targetDir = "$parentDir/HighLevelObjects/$HLOFiles{$file}";
   foreach my $tag (@HLOTags) {
      (my $tagName = $tag) =~ s/top_//;
      (my $targetFile = $file) =~ s|.+/||g;
      $targetFile = "$targetDir/$targetFile"."_$tagName";
      getVersionFromCVS ("TopMods/$file", $targetFile, $tag);
   } # foreach tag
   $file =~ s|.+/||g;
   # soft link in the default version
   system "ln -s $file\_$defaultTag $targetDir/$file";
} # foreach file
             
# setup include directory last
chdir $parentDir;
my @files = glob ("*");
foreach my $file (@files) {
   # if this isn't a directory, don't bother
   next unless (-d $file);
   # is this an include directory?
   if (-d "$file/$file") {
      system "ln -s ../$file/$file include/$file" unless (-e "include/$file");
   }
} # foreach file

# Final commands
chdir $parentDir;
foreach my $command (@lastCommands) {
   system $command;
}

#######################
## ################# ##
## ## Subroutines ## ##
## ################# ##
#######################

sub getVersionFromCVS {
   my $file    = shift;
   my $target  = shift;
   my $version = shift || "";
   if ($version) {
      $version = "-r $version";
   }
   my $cvs = "cvs checkout -d $tempdir $version $file";
   system $cvs;
   system "rm -rf $tempdir/CVS";
   system "cp -r $tempdir/* $target";
   system "rm -rf $tempdir";
}
