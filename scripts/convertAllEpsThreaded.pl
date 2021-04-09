#!/usr/bin/perl

use warnings;
use strict;
use threads;
use Thread::Queue;

my $dpi = 150;
my $useTransparent = "";
my $maxThreads = 8;

my $help = "Usage: $0 -args (file1.eps)
-dpi N   => Sets dpi to N ($dpi default)
-force   => Force the creation even if the gif files already exist
-gif     => Convert to GIF
-hhelp   => This help screen
-m match => Select files that start with pattern 'match'
-max N   => Set maximum threads to N ($maxThreads default)
-no      => Only files that do not have any gif files are converted
            (i.e. does not convert files where eps file is Newer than gif file)
-only    => convert Only the files explicitly listed
-trans   => White becomes transparent
-verbose => verbose (says which files are NOT converted)\n";

my ($force, $verbose, $newOnly, $only, @masks, @addfiles);
my $ext = ".png";
## default resolution of resulting graphics files
my %optionsHash;

## If the following files are not in path, please put full path in the
## strings below:
## Image encoding and decoding commands for GIF and PNG output.
my $cmdGIFencode = 'ppmtogif';
my $cmdGIFdecode = 'giftopnm';
my $cmdPNGencode = 'pnmtopng';
my $cmdPNGdecode = 'pngtopnm';
## Leave blank for no embedding source by default, "true" otherwise
my $embed = "true";

my $queue = new Thread::Queue;


while (@ARGV && $ARGV[0] =~ /^-/) {
   my $arg = shift @ARGV;
   $arg =~ s/^-+//;
   if ($arg =~ /^h/i) {
      print "$help\n";
      exit;
   }
   if ($arg =~ /^f/i) {
      $newOnly = "";
      $force = "true";
      next;
   }
   if ($arg =~ /^gif/i) {
      $ext = ".gif";
      next;
   }
   if ($arg =~ /^n/i) {
      $newOnly = "true";
      $force = "";
      next;
   }   
   if ($arg =~ /^v/i) {
      $verbose = "true";
      next;
   }
   if ($arg =~ /^o/i) {
      $only = "true";
      next;
   }
   if ($arg =~ /^tr/i) {
      $useTransparent = "white";
      next;
   }
   if ($arg =~ /^max/i) {
      $maxThreads = shift @ARGV;
      next;
   }
   if ($arg =~ /^dpi/i) {
      $dpi = shift @ARGV;
      next;
   }
   if ($arg =~ /^m/i) {
      my $mask = shift @ARGV;
      push @masks, $mask;
      next;
   }
   die "I don't understand '--$arg'.  Aborting.";
}

if (! @masks) {
   push @masks, "";
}

$optionsHash{dpi}         = $dpi;
$optionsHash{transparent} = $useTransparent;


my @epsFiles;
if (! $only) {
   foreach my $mask (@masks) {
      push @epsFiles, glob ("$mask*.eps");
   }
}

push @epsFiles, @addfiles;

#print "epsFiles @epsFiles\n";

foreach my $eps (@epsFiles) {
   (my $gif = $eps) =~ s|\.eps$|$ext|i;
   next if ($gif eq $eps);
   if (! -e $gif || ((-M $eps < -M $gif) && !$newOnly) || $force) {
      if ($newOnly && -e $gif) {
         warn "Danger: $gif\n";
         next;
      }
      $queue->enqueue( "$eps $gif");
      #convertPs2Png ($eps, $gif);
   } else {
      print "$gif is newer than $eps\n" if $verbose;
   }
}

while (my $pend = $queue->pending) {
   my @threadList = threads->list;
   my $num = @threadList;
   if ($num < $maxThreads) {
      my $numWanted = $maxThreads - $num;
      for my $loop (1..$numWanted) {
         my $thr = threads->new (\&sendToConvert);
      }
      #$thr->detach;
   }
   # sleep doesn't work for less than 1 second.  So I use 'select'
   # instead.
   select (undef, undef, undef, 0.1); # sleep 0.1
}

# cleanup
foreach my $thr (threads->list) {
   $thr->join();
}
foreach my $thr (threads->list) {
   $thr->join();
}

while (my $eps = $queue->dequeue_nb) {
   print "eps $eps\n";
}

sub sendToConvert {
   my $entry = $queue->dequeue_nb;
   return unless $entry;
   my ($eps, $gif) = split / /, $entry;
   print "Converting $eps to $gif\n";
   convertPs2Png ($eps, $gif);
   threads->self()->join();
}


sub convertPs2Png {
   my $ps_file = shift;
   my $output_file = shift;

   my $detail_default = 1.0;
   my $margin_default = 10;
   my $dpi_default = 100;
   my $rotate_default = 0;
   my $xsize_inch_default = 12;
   my $ysize_inch_default = 12;
   my $text_alpha_bits_default = 4;
   my $graphics_alpha_bits_default = 4;
   my %valuesHash;
   $valuesHash {dpi} = $dpi_default;
   $valuesHash {detail_ratio} = $detail_default;
   $valuesHash {margin_width} = $margin_default;
   $valuesHash {rotate_angle} = $rotate_default;
   $valuesHash {xsize_inch} = $xsize_inch_default;
   $valuesHash {ysize_inch} = $ysize_inch_default;
   $valuesHash {text_alpha_bits} = $text_alpha_bits_default;
   $valuesHash {graphics_alpha_bits} = $graphics_alpha_bits_default;

   # overwrite the defaults with the options hash
   foreach my $key (keys %optionsHash) {
      $valuesHash{$key} = $optionsHash{$key};
   }

   # convert command
   my $convert_command;
   if ($output_file =~ /\.gif$/i) {
      $convert_command = $cmdGIFencode;
   } elsif ($output_file =~ /\.png$/i) {
      $convert_command = $cmdPNGencode;
   } else {
      die "Target must be a .png or .gif file.  Aborting.\n";
   }

   my $reduction_ratio = sprintf (".3f", 1 / $valuesHash{detail_ratio}); 

   #dpi of working field

   my $work_dpi = $valuesHash{detail_ratio} * $valuesHash{dpi}; 

   #size of working field in pixels
   my $pix_Xsize =  $work_dpi * $valuesHash{xsize_inch};
   my $pix_Ysize =  $work_dpi * $valuesHash{ysize_inch};

   unlink $output_file;

   my $gscommand = "gs < /dev/null  -sDEVICE=ppmraw -dTextAlphaBits=$valuesHash{text_alpha_bits} -dGraphicsAlphaBits=$valuesHash{graphics_alpha_bits} -sOutputFile=- -g$pix_Xsize"."x"."$pix_Ysize -r$work_dpi -q -dNOPAUSE $ps_file | pnmcrop -white";

   # rotate command
   if ($valuesHash{rotate_angle}) {
      $gscommand .= " | pnmrotate $valuesHash{rotate_angle}";
   }

   # scale command
   if (1 != $valuesHash{detail_ratio}) {
      $gscommand .= " | pnmscale $valuesHash{detail_ratio}";
   }

   # margin command
   if ($valuesHash{margin_width}) {
      $gscommand .= " | pnmmargin -white $valuesHash{margin_width}";
   }

   my $color_reduction_command = "pnmdepth 15";
   #$color_reduction_command = "cat";

   # transparent option
   my $transparent_option = "";
   if ($valuesHash{transparent}) {
      $transparent_option = "-transparent $valuesHash{transparent}";
   }


   $gscommand .= "| $color_reduction_command | ppmquant 256 | $convert_command -interlace $transparent_option -";

   system "$gscommand > $output_file";

   return;
}
