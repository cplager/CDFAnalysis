#!/usr/bin/env perl

use warnings; # Comment out this line if you're getting complaints
              # Only causes problems on REALLY OLD versions of perl
use strict;

################################
## Define User Variables Here ##
################################

my $lumi = 200;

## # Dil 
## my $acc = 0.01031;
## my $AL = $acc * $lumi;
## my $back = 7.7;
## my $accPctErr = 0.135; # 13.5% acceptance error
## my $backErr = 2.7; # 2.7 events

## # Had 
## my $acc = 0.03964;
## my $AL = $acc * $lumi;
## my $back = 264.7;
## my $accPctErr = 0.08; # 13.5% acceptance error
## my $backErr = 2.7; # 2.7 events

## # secVtx
## my $acc = 0.03032;
## my $AL = $acc * $lumi;
## my $back = 13.8;
## my $accPctErr = 0.08; # 8% acceptance error
## my $backErr = 2.0; # 2.0 events

# largeLJ (fake NN)
my $acc = 0.0665;
my $AL = $acc * $lumi;
my $back = 428;
my $accPctErr = 0.08; # 8% acceptance error
my $backErr = 18.4; # 18.4 events

my $xsecStart = 0;
my $xsecEnd = 1.0;
my $xsecStep = 0.5;
my $power = 3;
#######################################

# choose subroutine you want run here
my $subroutine = "accept_error";

######################################

#############################
## ####################### ##
## ## Error Subroutines ## ##
## ####################### ##
#############################

# statistical sqrt error 
sub sqrt_error {
    my $xsec = shift;
    return sqrt ($xsec * $AL + $back) / $AL;
}

sub accept_error {
    my $xsec = shift;
    my $xsecHigh = $xsec / (1 - $accPctErr);
    my $xsecLow  = $xsec / (1 + $accPctErr);
    return ($xsecHigh - $xsecLow) / 2;
}

sub upper_accept_error {
    my $xsec = shift;
    my $xsecHigh = $xsec / (1 - $accPctErr);
    #my $xsecLow  = $xsec / (1 + $accPctErr);
    return ($xsecHigh - $xsec);
}

sub lower_accept_error {
    my $xsec = shift;
    #my $xsecHigh = $xsec / (1 - $accPctErr);
    my $xsecLow  = $xsec / (1 + $accPctErr);
    return ($xsec - $xsecLow);
}

sub bkgnd_error {
    my $xsec = shift;
    my $observed = $xsec * $AL;
    my $xsecHigh = ($observed + $backErr) / $AL;
    my $xsecLow  = ($observed - $backErr) / $AL;
    return ($xsecHigh - $xsecLow) / 2;
}

########################
## ################## ##
## ## Main Program ## ##
## ################## ##
########################

my $help = "Usage: $0 -args

Arguments:
-h        --> this Help file
-ns       --> No Stat box
-p N      --> use an Nth degree Polynomial
-s I J    --> take inputs from Stdin and use columns J vs I
-sub name --> use subroutine with name 'name'
-t title  --> use 'title' as your title and 'title.eps' as your filename
-xl label --> use 'label' as your x axis label
-yl label --> use 'label' as your y axis label
\n";

my $tempfile = "tempErrorFit.c";
my $title = "Errors";
my $eps = "out.eps";
my ($useStdin, $colX, $colY, $noAbs, $xLabel, $yLabel, $nostat);
while (@ARGV && $ARGV[0] =~ /^-/) {
    my $arg = shift @ARGV;
    if ($arg =~ /-sub/i) {
        $subroutine = shift;
        next;
    }
    if ($arg =~ /-h/i) {
        die $help;
    }
    if ($arg =~ /-t/i) {
        $title = shift;
        ($eps = $title) =~ s/\s+/_/g;
        $eps .= ".eps";
        next;
    }
    if ($arg =~ /-ns/i) {
        $nostat = "true";
        next;
    }
    if ($arg =~ /-xl/i) {
        $xLabel = shift;
        next;
    }
    if ($arg =~ /-p/i) {
        $power = shift;
        next;
    }
    if ($arg =~ /-yl/i) {
        $yLabel = shift;
        next;
    }
    if ($arg =~ /-s/i) {
        $useStdin = "true";
        $colX = shift @ARGV;
        $colY = shift @ARGV;
        # make sure colX and Y are digits
        if (($colX !~ /^\d+$/) || ($colY !~ /^\d+$/)) {
            die "colX : $colX and colY : $colY must both be non-negative integers\n";
        }
        next;
    }
    if ($arg =~ /-na/i) {
        $noAbs = "true";
        next;
    }
}

my (@xsecArray, @errorArray);
if ($useStdin)
{
    my $max = $colX;
    if ($colY > $max) {
        $max = $colY;
    }
    while (<>) {
        chomp;
        s/\#.+$//; # get rid of comments
        s/^\s+//; # rid of leading spaces
        my @words = split(/\s+/, $_);
        if (@words < $max) {
            #print "Not enough words: $_\n";
            next;
        }
        if ($noAbs) {
            push @xsecArray, $words[$colX];
            push @errorArray, $words[$colY];
        } else {
            push @xsecArray,  abs($words[$colX]);
            push @errorArray, abs($words[$colY]);
        }
    } # while <>
} else {
    no strict 'refs';
    for (my $xsec = $xsecStart; $xsec <= $xsecEnd; $xsec += $xsecStep) {
        push @xsecArray, $xsec;
        push @errorArray, &$subroutine($xsec);
    } # for $xsec
}
open (TARGET, ">$tempfile") or die;
my $size = @xsecArray;
print TARGET "{\ngROOT->SetStyle(\"Plain\");\n";
if ($nostat) {
    print TARGET "gStyle->SetOptFit(0000);\n";
} else {
    print TARGET "gStyle->SetOptFit(0011);
gStyle->SetStatH(0.1);          // height of box
gStyle->SetStatW(0.1);         // width of box \n";
}
print TARGET "   double x[$size] = {",join(", ", @xsecArray),"};\n";
print TARGET "   double y[$size] = {",join(", ", @errorArray),"};\n";
print TARGET "   TGraph tg ($size, x, y);\n";
print TARGET "   tg.SetTitle(\"$title\");\n";
print TARGET "   tg.SetMarkerStyle(20);\n";
if ($xLabel) {
    print TARGET "   tg.GetXaxis()->SetTitle(\"$xLabel\");\n";
}
if ($yLabel) {
    print TARGET "   tg.GetYaxis()->SetTitle(\"$yLabel\");\n";
}
print TARGET "   tg.Draw(\"ALP\");\n";
print TARGET "   tg.Fit(\"pol$power\");\n";
print TARGET "   c1->Print(\"$eps\");\n";
print TARGET "}\n";

my @output =  `root -l -b -q $tempfile`;
my $filled = "";
my $ok = "";
my $line = "";
#print @output,"\n";
foreach $_ (@output) {
    if (/NO.\s+NAME/ && ! $filled) {
        $ok = "true";
        next;
    }
    next unless $ok;
    # is this a parameter line?
    if (/^\s+(\d+)\s+(\S+)\s+(\S+)/) {
        printf "%2d)   %-10s %s\n", $1, $2, $3;
        $line .= "  $3";
    } else {
        # if not, then we must be done
        $filled = "true";
        $ok = "";
    }
}
print "$line\n";

