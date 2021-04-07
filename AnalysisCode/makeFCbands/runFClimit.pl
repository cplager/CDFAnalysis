#!/usr/bin/env perl

use strict;
use warnings;
use Data::Dumper;

use constant 
  {
     kMakeVert   => "makeVert",
     kFitVert    => "fitVert",
     kFitPbest   => "fitPbest",
     kLoadPes    => "loadPes",
     kMakeLikeli => "makeLikeli",
     kFitLikeli  => "fitLikeli",
     kMakeAcc    => "makeAcc",
     kPlotBands  => "plotBands",
     kPlotLikeli => "plotlikeli",
     kFitUpper   => "fitUpperBandEdge",
     kPlotExpect => "plotExpectedLimit",
  };

my @options = ( kMakeVert, kFitVert, kFitPbest, kLoadPes,
                kMakeLikeli, kFitLikeli, kMakeAcc, 
                kPlotBands, kPlotLikeli, kFitUpper, kPlotExpect);

# I can't use '=>' here because it puts strings around the kLoadPes.
# I need to use ',' instead.
my %useOnlyHash =
  (
   kLoadPes , 1,
   kMakeAcc , 1,
  );

#print Dumper (%useOnlyHash),"\n";

my $min = 0;
my $max = 60;
my $format = "bf%02d0";
my $dir = "/cdf/home/cplager/analysis/614";
my $maindir = "$dir/makeFCbands";
my $runCmd = "$dir/runExe.bash makeFCbands";
my $queueCmd = "clpQueue.pl addjob $runCmd";

chdir $maindir;

# Option parcing
my ($parce, $fixLargeError, $addOptions, $debug) = (0, 0, "", 0);
while (@ARGV && $ARGV[0] =~ /^-/) {
   (my $arg = shift) =~ s/^-+//;
   if ($arg =~ /^h/i) {
      help();
      exit;
   }
   if ($arg =~ /^parce/i) {
      $parce = 1;
      next;
   }
   if ($arg =~ /^debug/i) {
      $debug = 1;
      next;
   }
   if ($arg =~ /^fixlarge/i) {
      $fixLargeError = 1;
      next;
   }
   if ($arg =~ /^noq/i) {
      $queueCmd = $runCmd;
      next;
   }
   if ($arg =~ /^opt/i) {
      $addOptions .= shift @ARGV;
      $addOptions .= " ";
      next;
   }
   die "I don't understand '-$arg'.  Aborting.\n";
}

die "Usage: $0 [option] bla1.txt [bla2.txt]" unless @ARGV >= 2;

my $optionLabel = shift @ARGV;
my $option;
foreach my $key (@options) {
   if ($key =~ /$optionLabel/i) {
      $option = $key;
   }
} # foreach key

die "Don't understand '$optionLabel'.\n" unless $option;

my @txtFiles = @ARGV;
if ($parce) {
   ###################
   ## Parce Command ##
   ###################
   foreach my $txt (@txtFiles) {
      (my $log = $txt) =~ s/\.txt$//i;
      $log = "logs/$log";
      $log .= "_$option";
      my @logfiles;
      if ($useOnlyHash{$option}) {
         foreach my $loop ($min..$max) {
            push @logfiles, "$log\_$loop.log";
         }
      } # if broken up into different sections
      else {
         push @logfiles, "$log.log";
      } # if run all at once
      my $match = '^\*\s+\w';
      my $cmd = "grep -P -h \'$match\' @logfiles";
      #print "cmd $cmd\n";
      my $output = `$cmd`;
      if ($fixLargeError) {
         $output =~ s/e\+08/e\+02/g;
      }
      if ($debug) {
         print "Txt: $txt\n$output\n\n";
         next;
      }
      my $targetFile = "$txt\_new";
      open (SOURCE, "<", $txt) or die;
      open (TARGET, ">", $targetFile) or die;
      my ($done, $found, $doComment);
      while (<SOURCE>) {
         # If we're done, just copy the rest of the file
         if ($done) {
            print TARGET;
            next;
         }

         # if we've printed the new stuff, but haven't yet hit a blank
         # line, then save all of the old information as a comment.
         if ($doComment) {
            if (/\S/ && ! /^s*#/) {
               print TARGET "# $_";               
            } else {
               $done = "true";
               $doComment = "";
               print TARGET;
            }
            next;
         } # if do comment

         # if we've found what we're looking for, but haven't yet
         # ended the comment, keep looking
         if ($found && ! /^s*#/) {
            print TARGET $output,"\n";
            if (/\S/) {
               print TARGET "# $_";
            } else {
               print TARGET;
            }
            $doComment = "true";
            next;
         } # if found but not yet printed

         # If we haven't yet found what we're looking for, keep looking
         print TARGET;
         if (/\($option/i) {
            $found = "true";
         }
      }
      #print "$txt - @logfiles\n$output\n";
      
      system "mv $txt $txt\_old";
      system "mv $targetFile $txt";
   } # foreach txt      

} # parce command
else  {
   ###################
   ## Queue Command ##
   ###################
   foreach my $txt (@txtFiles) {
      (my $log = $txt) =~ s/\.txt$//i;
      $log = "logs/$log";
      $log .= "_$option";
      if ($useOnlyHash{$option}) {
         my $form = "onlyName=".$format;
         for my $loop ($min..$max) {            
            my $only = sprintf ($form, $loop);
            my $cmd = "$queueCmd $log\_$loop control=$txt $option=1 $only $addOptions";
            print $cmd,"\n";
            system $cmd unless $debug;
         }
      } # launch many jobs      
      else {
         my $cmd = "$queueCmd $log control=$txt $option=1 $addOptions";
         print $cmd,"\n";
         system $cmd unless $debug;
      } # launch one job
   } # foreach txt
} # queue command


sub help {
   die "Usage: $0 [options] command bla1.txt [bla2.txt]
--debug     => Print queue command without doing anything
--fixLarge  => Fix unreasonably large errors for 'fitVert'
--help      => This screen
--noQueue   => Run the jobs now instead of queueing them.
--option X  => Pass 'X' as an option the makeFCbands.  
--parce     => Parce log files of complete jobs and modify the .txt files 
Allowed commands: @options\n";
}
