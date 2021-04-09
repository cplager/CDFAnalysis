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

my $xsecStart = 2;
my $xsecEnd = 15;
my $xsecStep = 0.5;

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
-axylog   --> set y axis to log
-comma    --> use commas in between values
-fr   I   --> make a FRequency histogram using column I from data
-h        --> this Help file
-logx     --> use log (x) instead of x
-logy     --> use log (y) instead of y
-nd       --> do Not Delete temporary root file (for debugging)
-nf       --> No Fit
-ns       --> No stat box
-p N      --> use Polynomial of degree N (1 <= N <= 9)
-s I J    --> take inputs from Stdin and use columns J vs I
-sub name --> use subroutine with name 'name'
-t title  --> use 'title' as your title and 'title.eps' as your filename
-xl label --> use 'label' as your x axis label
-yl label --> use 'label' as your y axis label
\n";

my $tempfile = "tempErrorFit.c";
my $title = "Errors";
my $eps = "out.eps";
my $degree = 3;
my ($useStdin, $colX, $colY, $noAbs, $xLabel, $yLabel, $nostat,
	$saveRootFile, $logx, $logy, $comma, $useFrequency, $nofit, $axylog);
while (@ARGV && $ARGV[0] =~ /^-/) {
    my $arg = shift @ARGV;
    if ($arg =~ /-sub/i) {
        $subroutine = shift;
        next;
    }
    if ($arg =~ /-h/i) {
        die $help;
    }
    if ($arg =~ /-ns/i) {
	  $nostat = "true";
	  next;
    }
    if ($arg =~ /-comma/i) {
	  $comma = "true";
	  next;
    }
    if ($arg =~ /-logx/i) {
	  $logx = "true";
	  next;
    }
    if ($arg =~ /-logy/i) {
	  $logy = "true";
	  next;
    }
    if ($arg =~ /-axylog/i) {
	  $axylog = "true";
	  next;
    }
    if ($arg =~ /-t/i) {
        $title = shift;
        ($eps = $title) =~ s/\s+/_/g;
		$eps =~ s/\W/-/g;
        $eps .= ".eps";
        next;
    }
    if ($arg =~ /-p/i) {
	  my $value = shift;
	  if ($value =~ /^\d$/ && $degree > 0) {
		$degree = $value;		
	  }
	  next;
    }
    if ($arg =~ /-xl/i) {
        $xLabel = shift;
        next;
    }
    if ($arg =~ /-nd/i) {
	  $saveRootFile = "true";
	  next;
    }
    if ($arg =~ /-nf/i) {
	  $nofit = "true";
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
    if ($arg =~ /-fr/i) {
        $useStdin = "true";
		$useFrequency = "true";
        $colX = shift @ARGV;
        $colY = $colX;
        # make sure colX and Y are digits
        if ($colX !~ /^\d+$/) {
            die "colX : $colX  must be non-negative integers\n";
        }
        next;
    }
    if ($arg =~ /-na/i) {
        $noAbs = "true";
        next;
    }
}

my (@xsecArray, @errorArray);
my $uninit = -92834;
my $minX = $uninit;
my $maxX = $uninit;
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
            print "Not enough words: $_\n";
            next;
        }
		my $xval = $words[$colX];
		my $yval = $words[$colY];
        if (! $noAbs) {
		   $xval = abs ($xval);
		   $yval = abs ($yval);
        }
		push @xsecArray, $xval;
		push @errorArray, $yval;
		if (($xval > $maxX) || ($maxX == $uninit)) {
		   $maxX = $xval;
		}
		if (($xval < $minX) || ($minX == $uninit)) {
		   $minX = $xval;
		}		
    } # while <>
} else {
    no strict 'refs';
    for (my $xsec = $xsecStart; $xsec <= $xsecEnd; $xsec += $xsecStep) {
        push @xsecArray, $xsec;
        push @errorArray, &$subroutine($xsec);
    } # for $xsec
}
if ($logx) {
  foreach (@xsecArray) {
	$_ = log ($_)
  }
}
if ($logy) {
  foreach (@errorArray) {
	$_ = log ($_)
  }
}

open (TARGET, ">$tempfile") or die;
my $size = @xsecArray;
print TARGET "{\ngROOT->SetStyle(\"Plain\");";
if ($nostat) {
} else {
  print TARGET "gStyle->SetOptFit(0011);
gStyle->SetStatH(0.1);          // height of box
gStyle->SetStatW(0.1);         // width of box \n";
}
print TARGET "   double x[$size] = {",join(", ", @xsecArray),"};\n";
print TARGET "   double y[$size] = {",join(", ", @errorArray),"};\n";
if ($useFrequency) {
   my $bins = sqrt (@xsecArray);
   print TARGET "   TH1F tg (\"hist\", \"$title\", $bins, $minX, $maxX);\n";
   foreach my $value (@xsecArray) {
	  print TARGET "   tg.Fill($value);\n";
   }
} else {
   # Use a TGraph
   print TARGET "   TGraph tg ($size, x, y);\n";
}
print TARGET "   tg.SetTitle(\"$title\");\n";
print TARGET "   tg.SetMarkerStyle(20);\n";
if ($xLabel) {
    print TARGET "   tg.GetXaxis()->SetTitle(\"$xLabel\");\n";
}
if ($yLabel) {
    print TARGET "   tg.GetYaxis()->SetTitle(\"$yLabel\");\n";
}
print TARGET "   tg.Draw(\"ALP\");\n";
print TARGET "   gPad->SetLogy();\n   tg.Draw(\"ALP\");\n" if ($axylog);
print TARGET "   tg.Fit(\"pol$degree\");\n" unless ($nofit);
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
    if (m/^\s+(\d+)\s+(\S+)\s+(\S+)/) {
        printf "%2d)   %-10s %s\n", $1, $2, $3;
		if ($comma) {
		  if ($line) {
			$line .= ", $3";
		  } else {
			$line = "$3";
		  }
		} else {
		  $line .= "  $3";	   
		}
    } else {
        # if not, then we must be done
        $filled = "true";
        $ok = "";
    }
}
print "$line\n";
if (!$saveRootFile) {
  unlink $tempfile;
}
