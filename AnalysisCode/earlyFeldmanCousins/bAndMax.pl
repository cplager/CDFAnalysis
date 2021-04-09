#!/usr/bin/perl

use warnings;
use strict;

my $mode = "maxb";
while ($ARGV[0] && ($ARGV[0] =~ /^-/)) {
    my $arg = shift;
    if ($arg =~ /-meas_gen_const/i) {
        $mode = "bmaxconst";
        next;
    }
    if ($arg =~ /-bmax/i) {
        $mode = "bmax";
        next;
    }
    if ($arg =~ /-bconst/i) {
        $mode = "bconst";
        next;
    }
    if ($arg =~ /-bmean/i) {
        $mode = "bmean";
        next;
    }
    if ($arg =~ /-negsig/i) {
        $mode = "negsig";
        next;
    }
    if ($arg =~ /-possig/i) {
        $mode = "possig";
        next;
    }
    if ($arg =~ /-negfrac/i) {
        $mode = "negfrac";
        next;
    }
    if ($arg =~ /-posfrac/i) {
        $mode = "posfrac";
        next;
    }
    if ($arg =~ /-negSF/i) {
        $mode = "negSF";
        next;
    }
    if ($arg =~ /-posSF/i) {
        $mode = "posSF";
        next;
    }
    if ($arg =~ /-file/i) {
        $mode = "file";
        next;
    }
    if ($arg =~ /-gauss8/i) {
        $mode = "gauss8";
        next;
    }
    if ($arg =~ /-gauss/i) {
        $mode = "gauss";
        next;
    }
}
#print "mode $mode\n";


my $b = 0;
my $max = 0;
my $constant = 0;
my $mean = 0;
my ($negsig, $possig, $negfrac, $posfrac, $negSF, $posSF);
my ($zeroConst, $zeroWidth) = (0,0);
my $empty = "true";
while (<>) {
    if (/Using.+\D(\d+)/) {
        printEverything() if (! $empty);        
        $empty = "";
        $b = $1 / 100.;
        next;
    }
    if (/Fiiting for.+eps:\s+([\d\.]+)/) {
        printEverything() if (! $empty);        
        $empty = "";
        $b = $1;
        next;
    }
    if (/^mean\s*(\S+)/) {
        $mean = $1;
        next;
    }
    if (/^\s*1\s*Constant\s*(\S+)/) {
        $constant = $1;
        next;
    }
    if (/^\s*2\s*Max_bValue\s*(\S+)/) {
        $max = $1;
        next;
    }
    if (/^\s*3\s*Negative_Sigma1\s*(\S+)/) {
        $negsig = $1;
        next;
    }
    if (/^\s*4\s*Positive_Sigma1\s*(\S+)/) {
        $possig = $1;
        next;
    }
    if (/^\s*5\s*Negative_Frac\s*(\S+)/) {
        $negfrac = $1;
        next;
    }
    if (/^\s*6\s*Positive_Frac\s*(\S+)/) {
        $posfrac = $1;
        next;
    }
    if (/^\s*7\s*Negative_SigFrac\s*(\S+)/) {
        $negSF = $1;
        next;
    }
    if (/^\s*8\s*Positive_SigFrac\s*(\S+)/) {
        $posSF = $1;
        next;
    }
    if (/^\s*9\s*Zero_Const\s*(\S+)/) {
        $zeroConst = $1;
        next;
    }
    if (/^\s*10\s*Zero_Width\s*(\S+)/) {
        $zeroWidth = $1;
        next;
    }
}

printEverything();

sub printEverything {
    printf "%4.2f  %.4f\n", $b, $mean if ($mode eq "bmean");
    printf("%.4f  %4.2f\n", $max, $b) if ($mode eq "maxb");
    printf "%4.2f  %.4f\n", $b, $constant if ($mode eq "bconst");
    printf("%4.2f  %.4f\n", $b, $max) if ($mode eq "bmax");
    # bmaxconst - remember that this is run on fits where you 
    # have a distribution of b0s for a given bMeasured. This means
    # that $max and $b are switched.  So to get:
    # bMeasured, b0, probability 
    # in a file, you need to print $b $max, constant.
    printf("%4.4f  %.4f  %.4f\n", $b, $max, $constant) 
        if ($mode eq "bmaxconst");
    # other parameters
    printf "%4.2f  %.4f\n", $b, $negsig if ($mode eq "negsig");
    printf "%4.2f  %.4f\n", $b, $possig if ($mode eq "possig");
    printf "%4.2f  %.4f\n", $b, $negfrac if ($mode eq "negfrac");
    printf "%4.2f  %.4f\n", $b, $posfrac if ($mode eq "posfrac");
    printf "%4.2f  %.4f\n", $b, $negSF if ($mode eq "negSF");
    printf "%4.2f  %.4f\n", $b, $posSF if ($mode eq "posSF");
    my $bhundred = $b * 100;
    print "+ $bhundred $constant $max $negsig $possig $negfrac $posfrac $negSF $posSF 0. 0.02\n" if ($mode eq "gauss8");
    print "+ $bhundred $constant $max $negsig $possig $negfrac $posfrac $negSF $posSF $zeroConst $zeroWidth\n" if ($mode eq "gauss");
    if ($mode eq "file") {
        my $nice = $b * 100;
        open (SOURCE, ">txt/tmp$nice.txt") or die;
        for (my $loop = -.04; $loop <= 0.04; $loop += 0.0025) {
            printf SOURCE ("%7.4f %8.2f\n", $loop, splitGauss($loop));
        } # for loop
        close SOURCE;
    }
}

sub splitGauss {
    my $arg = shift;
    my $sigma1 = $negsig;
    my $frac   = $negfrac;
    my $sigma2 = $sigma1 * $negSF;
    if ($arg > 0) {
        $sigma1 = $possig;
        $frac   = $posfrac;
        $sigma2 = $sigma1 * $posSF;
    }
    return $constant * $frac * exp( -0.5 * $arg ** 2 / ($sigma1 ** 2)) +
        $constant * (1 - $frac) * exp( -0.5 * $arg ** 2 / ($sigma2 ** 2));
}
    
