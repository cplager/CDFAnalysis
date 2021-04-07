#!/usr/bin/env perl

use strict;
use warnings;

my $linkdef = "LinkDef.h";
my $outputbase = "dict/";
my $dotobase = "src/dict_";
my $class = "___CLASS___";

my $base = "#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class $class;

#endif\n";

# ok, read linkdef to see which classes we need to do this for

open (SOURCE, $linkdef) or die;
my $doto = "";
while (<SOURCE>) {
   if (/class\s+(\w+)\s*;/) {
	  my $newclass = $1;
	  (my $string = $base) =~ s|$class|$newclass|;
	  $doto .= " $dotobase$newclass.o";
	  my $filename = "$outputbase$newclass\_linkdef.h";
	  open (TARGET, ">$filename") or die;
	  print TARGET $string;
	  close TARGET;
   }
}
print "$doto\n";




