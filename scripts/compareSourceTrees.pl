#!/usr/bin/env perl

use warnings;
use strict;

my @avoidDirs = qw (CVS dict doc test ups objects ti_files .backup);
my @exts = qw (cc C h hh icc);

die "Usage: $0 basedir1 basedir2\n" unless (@ARGV >= 2);

my $basedir1 = shift;
my $basedir2 = shift;

my $currentdir = $ENV{PWD};

chdir $basedir1;
chomp ($basedir1 = `pwd`);
chdir $currentdir;
chdir $basedir2;
chomp ($basedir2 = `pwd`);

print "1 $basedir1 2 $basedir2\n";

# start traversing
chdir $basedir1;
my @dirs = (".");
my $counter = 0;
print "Collection directories\n";
while ($counter < @dirs) {
   chdir "$basedir1/$dirs[$counter]";
   chomp (my $dir = `pwd`);
   $dir =~ s|$basedir1|.|;
   #print "dir $dir\n";
   my @files = glob ("*");
   FILE: foreach my $file (@files) {
      # only directories
      next unless (-d $file);
      # no soft links
      next if (-l $file);
      foreach my $avoid (@avoidDirs) {
         next FILE if ($avoid eq $file);
      }
      my $thisdir = "$dir/$file";
      # does this directory exist in $basedir2
      if (! -e "$basedir2/$thisdir") {
         warn "$thisdir exists only in $basedir1\n";
         next;
      }
      push @dirs, $thisdir;
   }
   ++$counter;
}

foreach my $dir (@dirs) {
   $dir =~ s|^./||;
   #print "   checking $dir\n";
   my @files = ();
   chdir "$basedir1/$dir";
   foreach my $ext (@exts) {
      push @files, glob ("*.$ext");
   }
   chdir "$basedir2/$dir";
   foreach my $ext (@exts) {
      push @files, glob ("*.$ext");
   }
   @files = uniqueList (@files);
   foreach my $file (@files) {
      $file = "$dir/$file";
      # does this file exist in dir1?      
      if (! -e "$basedir1/$file" ) {
         print "** $file doesn't exist in $basedir1\n";
         next;
      }
      if (! -e "$basedir2/$file" ) {
         print "** $file doesn't exist in $basedir2\n";
         next;
      }
      chomp (my $diff = `diff -q $basedir1/$file $basedir2/$file`);
      if ($diff) {
         print "** $file is different\n";
      }
   } # foreach file
}

sub uniqueList {
   my %hash;
   foreach my $item (@_) {
      $hash{$item} = $item;
   }
   return keys %hash;
}
