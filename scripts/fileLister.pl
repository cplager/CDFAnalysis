#!/usr/bin/env perl

use warnings;
use strict;

# Do you only want '.root' files?
my $dotroot = "true"; 
# Do you want the list sorted numerically?
my $sortNumerically = "true"; 

(my $thisscript = $0) =~ s|.+/||g;
my $help = "$thisscript [--args] [outoutlist]
--all    => list ALL files, not just '.root'
--help   => This help screen
--nonum  => list files alphabetically, not numerically
--sub    => Look for files one directory down as well
";


my $subdirs;
while (@ARGV && $ARGV[0] =~ /^-/) {
   (my $arg = shift) =~ s/^-+//;
   if ($arg =~ /^all/i) {
      $dotroot = "";
      next;
   }
   if ($arg =~ /^sub/i) {
      $subdirs = "true";
      next;
   }
   if ($arg =~ /^nonum/i) {
      $sortNumerically = "";
      next;
   }
   if ($arg =~ /^help/i) {
      die $help;
      next;
   }
   die "I don't recognize '-$arg'.  Aborting\n";
}


my $outputname = shift || "";

chomp (my $host = $ENV{HOST} || `hostname`);
my $dir  = $ENV{PWD};
if (! $outputname) {
   ($outputname = $dir) =~ s|/$||;
   $outputname =~ s|.+/||g;
}
if ($outputname !~ /\./) {
   $outputname .= ".rootlist";
}

# give it an extention if it needs it
if ($host !~ /\./) {
   $host .= ".fnal.gov";
}
$host .= "/"; # rootd wants two //s

my $prefix = "root://";

# is this a diskpool directory
if ($dir =~ m|^/pnfs/|) {
   $host = "dcap fcdfrdc3 ";
   $prefix = "";
}


my @files;
if ($dotroot) {
   @files = glob ("*.root");
   if ($subdirs) {
      push @files, glob ("*/*.root");
   }
} else {
   @files = glob ("*.*");
   if ($subdirs) {
      push @files, glob ("*/*.*");
   }
}

if (! @files) {
   print "No root files here.  Aborting.\n";
   exit;
}

@files = sort byNumbers (@files) if ($sortNumerically);

open (TARGET, ">$outputname") or die "Can't open $outputname for output\n";

foreach my $file (@files) {
   print TARGET "$prefix$host$dir/$file\n";
   #print TARGET "$file\n"; # debug only
}

close TARGET;
print "Wrote file list $outputname\n";


sub byNumbers {   
   my ($aCharRef, $aNumRef) = splitByNumbers ($a);
   my ($bCharRef, $bNumRef) = splitByNumbers ($b);
   my $aSize = @$aCharRef;
   my $bSize = @$bCharRef;
   my $size;
   if ($aSize < $bSize) {
      $size = $aSize;
   } else {
      $size = $bSize;
   }
   for my $loop (0..$size - 1) {
      my $comp = $aCharRef->[$loop] cmp $bCharRef->[$loop];
      return $comp if $comp;
      $comp = $aNumRef->[$loop] <=> $bNumRef->[$loop];
      return $comp if $comp;
   } # for loop
   # if we're still here
   return $a cmp $b;
}

sub splitByNumbers {
   my $line = shift;
   my $numRef  = [];
   my $charRef = [];
   # strip off any leading numbers
   if ($line =~ /^(\d+)/) {
      push @$charRef, "";
      push @$numRef, $1;
      $line = $';
   }
   # now we are sure that we have a string that starts with a non digit
   while ($line =~ /^(\D+)(\d+)/) {
      push @$charRef, $1;
      push @$numRef, $2;
      $line = $';
   }
   # is there anything left?
   if (length ($line)) {
      # there is, but it must be all characters
      push @$charRef, $line;
      push @$numRef, "";      
   }
   return ($charRef, $numRef);
}

# for debugging purposes
sub recombineString {
   my $charRef = shift;
   my $numRef  = shift;
   my $entries = @$charRef;
   my $retval = "";
   for my $loop (0..$entries - 1) {
      $retval .= $charRef->[$loop];
      $retval .= $numRef->[$loop];
   }
   return $retval;
}
