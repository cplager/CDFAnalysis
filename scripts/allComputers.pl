#!/usr/bin/env perl

use strict;
use warnings;

my %computersHash;
$computersHash{ucla} = ["b0pcucla09", "b0pcucla10", "b0pcucla11", 
                       "b0pcucla12", "b0pcucla13", "b0pcucla20", "b0pcucla21"];
$computersHash{fcdflnx} = ["fcdflnx4", "fcdflnx5", "fcdflnx6", 
                           "fcdflnx7", "fcdflnx8", "fcdflnx9"];

my $which = "ucla";
while (@ARGV && $ARGV[0] =~ /^-/) {
   (my $arg = shift) =~ s/^-+//;
   if ($arg =~ /^c/i) {
      $which = shift;
   }
}
                           
my $aref = $computersHash{$which} || "";

die "'$which' is not a valid choice\n" unless ($aref);

my $command = "@ARGV";

foreach my $computer (@$aref) {   
   print "\n$computer> $command\n";
   system "ssh $computer $command";   
}
print "\n";
