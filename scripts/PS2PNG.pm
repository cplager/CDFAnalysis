package PS2PNG;

use Exporter;
@ISA = qw (Exporter);

# Member functions that are visible by default
@EXPORT = qw (
                convertPs2Png 
                setOptions
            );

## Image encoding and decoding commands for GIF and PNG output.
my $cmdGIFencode = 'ppmtogif';
my $cmdGIFdecode = 'giftopnm';
my $cmdPNGencode = 'pnmtopng';
my $cmdPNGdecode = 'pngtopnm';

## Default Values
my $detail_default = 1.0;
my $margin_default = 10;
my $dpi_default = 100;
my $rotate_default = 0;
my $xsize_inch_default = 12;
my $ysize_inch_default = 12;
my $text_alpha_bits_default = 4;
my $graphics_alpha_bits_default = 4;

#############################################################################

my %optionsHash;

sub setOptions {
   my $option = shift;
   my $value  = shift;
   # This is a little bit silly.  We have two hashes, when we only
   # need one.  Eventually I'll move valueHash initialization to
   # happen when the package is initialized and then just set its
   # values here.
   $optionsHash{$option} = $value;
}

sub convertPs2Png {
   my $ps_file = shift;
   my $output_file = shift;

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

1;
