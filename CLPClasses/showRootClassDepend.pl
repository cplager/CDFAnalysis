#!/usr/bin/env perl

use strict;
use warnings;
my $linkdef = "LinkDef.h";

my $base = $ENV{PWD};

my %linkdef;
my @names;
open (SOURCE, $linkdef) or die;
while (<SOURCE>) {
   chomp;
   if (/pragma\s+link\s+C\+\+\s+class\s+(\w+)/) {
	  $linkdef {$1} = $1;
	  push @names, $1;
   }
}


chdir "include";
my @files = glob ("*h");
chdir $base;

foreach my $file (@files) {
   (my $name = $file) =~ s/\.h*//i;   
   if (! $linkdef{$name}) {
	  push @names, $name;
   }
}


foreach my $name (@names) {
   print "\n\nInclude: $name\n";
   if ($linkdef{$name}) {
	  print "     Included in $linkdef\n";
   }
   open (SOURCE, "include/$name.h") or die;
   while (<SOURCE>) {
	  chomp;
	  if (/class\s+$name\s*:?\s*/) {
		 my $rest = $';
		 
		 if ($rest =~ /\S/) {
			print "     derives from: $rest\n";
		 } else {
			print "     no base class\n";
		 }
	  } # if class
	  if (/ClassDef\s*\((\S+),/) {
		 my $class = $1;
		 s/^\s+//;
		 print "     $_\n";
		 if ($name ne $class) {
			print "*****WARNING: Mismatch*****\n";
		 }
	  }
   }
   close SOURCE;
   open (SOURCE, "src/$name.cc") or next;
   while (<SOURCE>) {
	  chomp;
	  if (/ClassImp\s*\((\S+)\)/) {
	     my $class = $1;
		 s/^\s+//;
		 print "     $_\n";
		 if ($name ne $class) {
			print "*****WARNING: Mismatch*****\n";
		 }
	  }
   }
   close SOURCE;
}
