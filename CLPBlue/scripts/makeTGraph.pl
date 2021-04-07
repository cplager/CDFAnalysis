#!/usr/bin/perl

use warnings;
use strict;

my (@x, @y);
while (<>) {
    if (/([\d\.]+)\s+([\d\.]+)/) {
        my $xval = $1;
        my $yval = $2;
        push @x, $xval;
        push @y, $yval;
    }
}
my $size = @x;
print "{\n";
print "   double x[$size] = {",join(", ", @x),"};\n";
print "   double y[$size] = {",join(", ", @y),"};\n";
print "   TGraph tge ($size, x, y);\n";
print "   tge.SetMarkerStyle(20);\n";
print "   tge.Draw(\"ALP\");\n";
print "   tge.Fit(\"pol3\");\n";
print "   c1->Print(\"out.eps\");\n";
print "}\n";

