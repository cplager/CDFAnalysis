#!/usr/bin/perl

use warnings;
use strict;


my @files;

my $target = shift @ARGV;

my $tempstring = "__temp_cp2p_$$.";

foreach (@ARGV) {
   if (/.e?ps/i) {
	  push @files, $_;
   }
}

die "Usage: $0 target.pdf one.ps two.eps\n" 
  unless (@files && $target=~ /pdf$/i);

my $count = 0;
print "converting (e)ps files to pdf.\n";
foreach (@files) {
   my $tempfile = "$tempstring$count.pdf";
   my $command = "ps2pdf $_ $tempfile";
   #print "com $command\n";
   system $command;
   ++$count;
}

my $convert = "gs -dBATCH -dNOPAUSE -q -sDEVICE=pdfwrite -sOutputFile=$target ";
--$count;
for my $loop (0..$count){
   my $tempfile = "$tempstring$loop.pdf";
   $convert .= "$tempfile ";
}

#print "command $convert\n\n";
print "Making $target.\n";
system $convert;

print "Cleaning up. ";
system "rm $tempstring*";
print "Done\n";
