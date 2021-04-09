#!/usr/bin/env perl

use warnings;     # Comment out this line if you're getting complaints
                  # Only causes problems on REALLY OLD versions of perl
use strict;

my $version = "makePlotWithFit.pl v1.0 cplager+perl\@fnal.gov";



my $help = "Usage: $0 -args

Arguments:
-col K       --> draw 'K'th column as well (only with '-s' option) 
-comma       --> use commas in between values
-debug       --> do not delete temporary root script
-filename XX --> use filename 'XX' four output
-h           --> this Help file
-incmax XX   --> increase the maximum by a factor of XX
-legend XX   --> makes a TLegend where 'XX' is \"x1, y1, x2, y2\"
-legname X Y --> Column 'X' is named 'YY'
-logx        --> use log (x) instead of x
-logy        --> use log (y) instead of y
-nofit       --> do NOt FIT
-noabs       --> do not take the abs of the numbers
-ns          --> No stat box
-p N         --> use Polynomial of degree N (1 <= N <= 9)
-prefix XX   --> Prepend 'XX' to output file
-s I J       --> take inputs from Stdin and use columns J vs I
-start XX    --> Wait until 'XX' appears in a comment before grabbing numbers
-stop  XX    --> Do not use any numbers after 'XX' appears in a comment
-t title     --> use 'title' as your title and 'title.eps' as your filename
-withabs     --> use the absolute value of the x and y values
-xl label    --> use 'label' as your x axis label
-yl label    --> use 'label' as your y axis label
\n$version\n\n";

my @colors = (2, 3, 4, 6, 102, 103, 104, 106, 152, 153, 154, 156, 1);
my @markers = (21, 22, 23, 29, 20);

my $tempfile = "tempRootScript$$.C";
my $title = "Errors";
my $eps = "out.eps";
my $degree = 3;
my @extracols;
my ($incmax, $incmin) = (1, 1);
my ($useStdin, $colX, $colY, $noAbs, $xLabel, $yLabel, $nostat,
	$saveRootFile, $logx, $logy, $comma, $nofit, $start, $stop,
    $legend) = ();
my %legendName;
my $prefix = "";
my $filename = "";
my $dontlook = "";
$noAbs = "true";
while (@ARGV && $ARGV[0] =~ /^-/) {
   my $arg = shift @ARGV;
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
   if ($arg =~ /-nofit/i) {
	  $nofit = "true";
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
   if ($arg =~ /-file/i) {
      $filename = shift @ARGV;
      next;
   }
   if ($arg =~ /-legend/i) {
      $legend = shift @ARGV;
      next;
   }
   if ($arg =~ /-legnam/i) {
      my $col = shift @ARGV;
      my $name = shift @ARGV;
      $legendName{$col} = $name;
      next;
   }
   if ($arg =~ /-incmax/i) {
      $incmax = shift @ARGV;
      next;
   }
   if ($arg =~ /-prefix/i) {
      $prefix = shift @ARGV;
      next;
   }
   if ($arg =~ /-start/i) {
      $start = shift @ARGV;
      $start =~ s/_/ /g;
      $dontlook = "true";
      #warn "start $start\n";
      next;
   }
   if ($arg =~ /-stop/i) {
      $stop = shift @ARGV;
      $stop =~ s/_/ /g;
      #warn "stop $stop\n";
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
   if ($arg =~ /-col/i) {
      my $col = shift;
      push @extracols, $col;
      next;
   }
   if ($arg =~ /-xl/i) {
      $xLabel = shift;
      next;
   }
   if ($arg =~ /-debug/i) {
	  $saveRootFile = "true";
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
   if ($arg =~ /-noabs/i) {
      $noAbs = "true";
      next;
   }
   if ($arg =~ /-withabs/i) {
      $noAbs = "";
      next;
   }
}

my @colarray;
my (@xArray, @yArray);
my ($minY, $maxY) = (1e99, -1e99); # extreme values;
if ($useStdin) {
   my $max = $colX;
   if ($colY > $max) {
      $max = $colY;
   }
   if (@extracols) {
      # set up the annnonomous arrays
      foreach my $col (@extracols) {
         # first, check max
         if ($col > $max) {
            $max = $col;
         }
         # now set up the array
         my $aref = [];
         push @colarray, $aref;
      }                         # foreach @extracols
   }                            # if extracols
	
   while (<>) {
      chomp;
      if ($start && /$start/) {
         $dontlook = "";
         next;
      }
      if ($stop && /$stop/) {
         $dontlook = "true";
         next;
      }
      next if $dontlook;
      s/\#.+$//;                # get rid of comments
      s/^\s+//;                 # rid of leading spaces
      next unless /\S/;         # ignore empty lines
      my @words = split(/\s+/, $_);
      if (@words < $max) {
         #print "Not enough words: $_\n";
         next;
      }
      my ($xVal, $yVal);
      if (defined $words[$colX] && defined $words[$colY]) {
         $xVal = $words[$colX];
         $yVal = $words[$colY];
      } else {
         next;
      }
      # check to see if they are both numbers
      if ( ($xVal !~ /^([+-]?)(?=\d|\.\d)\d*(\.\d*)?([Ee]([+-]?\d+))?$/) ||
           ($yVal !~ /^([+-]?)(?=\d|\.\d)\d*(\.\d*)?([Ee]([+-]?\d+))?$/) ) {
         # one of them isn't a number
         warn "'$xVal' or '$yVal' isn't a number\n";
         next;
      }
      if (!$noAbs) {
         $xVal = abs ($xVal);
         $yVal = abs ($yVal);
      }
      if ($yVal > $maxY) {
         $maxY = $yVal;
      }
      if ($yVal < $minY) {
         $minY = $yVal;
      }
      push @xArray, $xVal;
      push @yArray, $yVal;
      if (@extracols) {
         my $count = 0;
         foreach my $col (@extracols) {
            my $aref = $colarray[$count++];
            my $value = "N/A";
            if (defined $words[$col]) {
               $value = $words[$col];
            }
            my $yVal = $value;
            # make sure this is a number
            if ($yVal !~ /^([+-]?)(?=\d|\.\d)\d*(\.\d*)?([Ee]([+-]?\d+))?$/) {
               # here we go
               warn "'$yVal' isn't a number\n";
               # get rid of X and Y values
               pop @xArray;
               pop @yArray;
               # Get rid of column values already added.
               # Note: Normally, we'd go from 0..$count-1, but since
               # we haven't added it for this column, we go to
               # $count-2
               for my $loop (0..($count-2)) {
                  my $aref = $colarray[$loop];
                  #warn "about to pop $aref $loop\n";                    
                  pop @$aref;
               }
               last;
            }                   # if yVal not a number
            if (!$noAbs) {
               $yVal = abs ($yVal);
            }                   # if noAbs
            if ($yVal > $maxY) {
               $maxY = $yVal;
            }
            if ($yVal < $minY) {
               $minY = $yVal;
            }
            push @$aref, $yVal;
         }                      # foreach col
      }                         # if extracols
   }                            # while <>
} else {
   die "You haven't selected any columns to plot\n";
}

if ($logx) {
   foreach (@xArray) {
      $_ = log ($_)
   }
}
if ($logy) {
   foreach (@yArray) {
      $_ = log ($_)
   }
}

##############################
## ######################## ##
## ## Create Root Script ## ##
## ######################## ##
##############################

open (TARGET, ">$tempfile") or die;
my $size = @xArray;
print TARGET "{\n   gROOT->SetStyle(\"Plain\");";
if ($nostat) {
} else {
   print TARGET "   gStyle->SetOptFit(0011);
   gStyle->SetStatH(0.1);          // height of box
   gStyle->SetStatW(0.1);         // width of box \n";
}
print TARGET "   double x[$size] = {",join(", ", @xArray),"};\n";
print TARGET "   double y[$size] = {",join(", ", @yArray),"};\n";
print TARGET "   TGraph *tg = new TGraph ($size, x, y);\n";
my $count = 0;
foreach my $aref (@colarray) {
   print TARGET "   double y$count"."[$size] = {",join(", ", @$aref),"};\n";
   print TARGET "   TGraph *tg"."$count = new TGraph($size, x, y$count);\n";
   ++$count;
}                               # foreach colarray
print TARGET "   tg->SetTitle(\"$title\");\n";
print TARGET "   tg->SetMarkerStyle(20);\n";
if ($xLabel) {
   print TARGET "   tg->GetXaxis()->SetTitle(\"$xLabel\");\n";
}
if ($yLabel) {
   print TARGET "   tg->GetYaxis()->SetTitle(\"$yLabel\");\n";
}
# do we need to change the Y axis
if (@colarray) {
   my $diffY = $maxY - $minY;
   $maxY += $diffY * 0.10;
   $minY -= $diffY * 0.10;
   $maxY += ($incmax - 1) * ($maxY - $minY);
   print TARGET "   tg->SetMaximum($maxY);\n";
   print TARGET "   tg->SetMinimum($minY);\n";   
}

print TARGET "   tg->Draw(\"ALP\");\n";

$count = 0;
if (! $filename) {
   if ($prefix) {
      $filename = $prefix.$eps;
   } else {
      $filename = $eps;
   }
}
$filename .= ".eps" unless ($filename =~ /\.eps$/i);

foreach my $aref (@colarray) {
   my $color  = $colors [$count % @colors];
   my $marker = $markers[$count % @markers];
   #print "marker $marker\ncolor $color\n";
   print TARGET "   tg"."$count->SetLineColor($color);\n";
   print TARGET "   tg"."$count->SetMarkerStyle($marker);\n";
   print TARGET "   tg"."$count->SetMarkerColor($color);\n";
   print TARGET "   tg"."$count->Draw(\"LP\");\n";
   ++$count;
}                               # foreach colarray
# do we do a legend?
if ($legend) {
   print TARGET "   TLegend *leg = new TLegend($legend);\n   leg->SetFillColor(kWhite);";
   my $name = $legendName{$colY} || "column $colY";
   print TARGET "   leg->AddEntry (tg, \"$name\", \"lp\");\n";
   my $count = 0;
   foreach my $col (@extracols) {
      my $name = $legendName{$col} || "column $col";
      print TARGET "   leg->AddEntry (tg$count, \"$name\", \"lp\");\n";
      ++$count;
   }
   print TARGET  "   leg->Draw(\"same\");\n";
}

print TARGET "   tg->Fit(\"pol$degree\");\n" unless ($nofit);
print TARGET "   c1->Print(\"$filename\");\n";
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
   }                            # else if ...
}                               # foreach output
print "$line\n";
if (!$saveRootFile) {
   unlink $tempfile;
}

