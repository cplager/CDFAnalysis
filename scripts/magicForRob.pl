#!/usr/bin/env perl

use warnings;
use strict;

# Option parcing
while (@ARGV && $ARGV[0] =~ /^-/) {
   (my $arg = shift) =~ s/^-+//;
   die "I don't understand '-$arg'.  Aborting.\n";
}

my $count = 0;
my $line = 0;
my $mode = "";
my @errors;

my @wholeFile = <>;
my $wholeFileAsOneLine = join ("", @wholeFile);
$wholeFileAsOneLine =~ s/(\r\n|\015\012|\012|\015)/\n/g;
my @lines = split /\n/, $wholeFileAsOneLine;

foreach $_ (@lines) {
   ++$line;
   chomp;
   my $orig = $_;
   if (/^\s*-\s*/) {
      print "%\n\n";
      $mode = "title";
      my @parts = split /\;/, $';
      if (2 != @parts) {
         push @errors, "Error with line $line: $orig\n";
      } else {
         foreach my $part (@parts) {
            $part =~ s/^\s+//;
            $part =~ s/\s+$//;
         }
         print "- $parts[0] ; $parts[1]\n";
      }
      next;
   } # title line
   if (/^\s*\+\s*/) {
      $mode = "author";
      my @parts = split /\;/, $';
      if (@parts < 3) {
         push @errors, "Error with line $line: $orig\n";
      } else {
         foreach my $part (@parts) {
            $part =~ s/^\s+//;
            $part =~ s/\s+$//;
         }
         if ((3 == @parts) || ($parts[3] !~ /\S/) ) {
            $parts[3] = sprintf "collab%02d", ++$count;
         }
         print "+ $parts[0] ; $parts[1]; $parts[2]; $parts[3]\n";
      }
   }
}

print "%\n";

if (@errors) {
   print "\n\n\n";
   print "WARNING: You should fix these errors first.\n";
   print "Remember, all title lines should be\n";
   print "- title ; time\n";
   print "  and all author lines should be:\n";
   print "+ Person; time; title\n";
   print join ("\n", @errors);
}
