#!/usr/bin/env perl

use warnings;
use strict;

while (@ARGV && $ARGV[0] =~ /^-/) {
   (my $arg = shift) =~ s/^-+//;
   warn "I don't understand '-$arg'.\n";
}
my $usage = "Usage: $0 [-args] rootd.list numSections thisSection\nWill create 'rootd_short.list'.";
die "$usage\n" 
  unless (@ARGV >= 3);

my $file = shift;
(my $numSections = shift) =~ s/\D+//g;
(my $thisSection = shift) =~ s/\D+//g;
my $outfile = shift || $file;

if (($numSections < $thisSection) || ! $numSections) {
   die "$usage\nNote: 'numSections' >= 1 && 'numSections >= 'thisSection'\n";
}

my $ext = ".list";
if ($outfile =~ /(\.\w+)$/) {
   $ext = $1;
   $outfile = $`;
}
$outfile .= "_short$ext";

open (SOURCE, $file) or die "Can't open $file for input\n";

my @list;
while (<SOURCE>) {
   chomp;
   s/#.*$//;
   next unless /\S/;
   s/^\s+//; # get rid of leading spaces
   s/\s.+$//; #get rid of everything after a space;
   push @list, $_;
}

#print "@list\n";
#print join ("\n", @list),"\n";
my $entries    = @list;
my $persection = int ($entries / $numSections);
my $extra      = $entries % $numSections;
--$thisSection; # we want 0..n-1 not 1..n
my $start = $persection * $thisSection;
my $num   = $persection - 1;
if ($thisSection < $extra) {
   $start += $thisSection;
   ++$num;
} else {
   $start += $extra;
};
my $stop = $start + $num;
my @array = @list[$start..$stop];
#print "$start $stop: @array\n";
open TARGET, ">$outfile" or die "Can't open $outfile for output\n";
print TARGET join ("\n", @array), "\n";
close TARGET;

print "Wrote $outfile using files ", $start + 1, " to ", $stop, " out of $entries total files.\n";
