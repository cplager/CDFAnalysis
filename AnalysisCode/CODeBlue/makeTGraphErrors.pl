#!/usr/bin/perl

use warnings;
use strict;

my (@x, @ex, @y, @ey);
my $eps = "out.eps";

my $help = "Usage: $0 -args <STDIN>
where <STDIN> is formated like
2.0 +0.8 -0.6
   -or-
2.0  0.8  
   - with the -se argument

Arguments:
-h        --> this Help file
-ne       --> show No Error bars
-se       --> expect a single error argument
-t title  --> use 'title' as your title and 'title.eps' as your filename
\n";

my $title = "Errors";
my $noErrors = "";
my $singleErrors = "";

while (@ARGV && $ARGV[0] =~ /^-/) {
    my $arg = shift @ARGV;
    if ($arg =~ /-h/i) {
        die $help;
    }
    if ($arg =~ /-t/i) {
        $title = shift;
        ($eps = $title) =~ s/\s+/_/g;
        $eps .= ".eps";
        next;
    }
    if ($arg =~ /-se/i) {
        $singleErrors = "true";
        $noErrors = "true";
        next;
    }
    if ($arg =~ /-ne/i) {
        $noErrors = "true";
        next;
    }
}


while (<>) {
    if ($singleErrors) {
        if (/([\d\.]+)\s+([\d\.]+)/) {
            my $xval = $1;
            my $yval = $2;
            push @x, $xval;
            push @y, $yval;
            next;
        }
    } else {
        if (/([\d\.]+)\s+\+([\d\.]+)\s+\-([\d\.]+)/) {
            my $xval = $1;
            my $yp = $2;
            my $ym = $3;
            my $yval = ($yp + $ym) / 2;
            my $yerr = abs ($yp - $ym) / 2;
            push @x, $xval;
            push @ex, 0.;
            push @y, $yval;
            push @ey, $yerr;
            next;
        }
    }
}
my $size = @x;
print "{\ngROOT->SetStyle(\"Plain\");
gStyle->SetOptFit(0011);
gStyle->SetStatH(0.1);          // height of box
gStyle->SetStatW(0.1);         // width of box \n";
print "   double x[$size] = {",join(", ", @x),"};\n";
print "   double ex[$size] = {",join(", ", @ex),"};\n";
print "   double y[$size] = {",join(", ", @y),"};\n";
print "   double ey[$size] = {",join(", ", @ey),"};\n";
if ($noErrors) {
    print "   TGraph tge ($size, x, y);\n";
} else {
    print "   TGraphErrors tge ($size, x, y, ex, ey);\n";
}
print "   tge.SetTitle(\"$title\");\n";
print "   tge.SetMarkerStyle(20);\n";
print "   tge.Draw(\"ALP\");\n";
print "   tge.Fit(\"pol3\");\n";
print "   c1->Print(\"$eps\");\n";
print "}\n";

