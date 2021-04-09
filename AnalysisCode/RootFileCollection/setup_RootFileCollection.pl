#!/usr/bin/env perl

use strict;
use warnings;

my $rfc = "RootFileCollection";

# This script should be run inside the "parent" or Release directory,
# but NOT inside the RFC directory.

my $basedir = $ENV{PWD};
(my $lastdir = $basedir) =~ s|.+/||g;

my $problem = "";
while ($lastdir eq $rfc) {
   my $oldbase = $basedir;
   $basedir =~ s|/$||; # get rid of the trailing '/'
   $basedir =~ s|/[^/]+$||;
   if ($oldbase eq $basedir) {
      $problem = "true";
      last;
   }
   ($lastdir = $basedir) =~ s|.+/||g;
}

chdir $basedir;

if (($lastdir eq $rfc) || (! -d $rfc) || $problem) {
   die "You must run this script inside the top level directory (e.g., 'myRelease/',\nabove the '$rfc' directory.\n";
}

# include directory
print "Checking for 'include/'\n";
if (! -e "include") {
   my $success = mkdir "include";
   die "Couldn't make 'include' directory\n" unless ($success);
}
chdir "include/";
system "ln -s ../RootFileCollection/RootFileCollection";
chdir "$basedir";

# shlib directory
print "Checking for 'shlib/'\n";
if (! -e "shlib") {
   my $success = mkdir "shlib";
   die "Couldn't make 'shlib' directory\n" unless ($success);
}



