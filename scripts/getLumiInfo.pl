#!/usr/bin/env perl

use strict;
use warnings;

use Math::BigInt;
use DBI;

use constant kDatabase     => "dbi:Oracle:cdfofread";
use constant kDatabaseUser => "cdf_reader";
use constant kDatabasePass => "reader";

(my $scriptname = $0) =~ s|.+/||;

my $help = "Usage: $scriptname -options dataset [dataset.output]
--help      => This menu
--source XX => Use the 'XX' database (OFFLINE|ONLINE)
";

my ($source) = ("OFFLINE");
while (@ARGV && $ARGV[0] =~ /^-/) {
   (my $arg = shift) =~ s/^-+//;
   if ($arg =~ /^h/i) {
      die $help;
   }
   if ($arg =~ /^source/i) {
      $source = uc shift;
      next;
   }
   die "I don't understand '-$arg'.  Aborting.\n";
}

if (($source ne "OFFLINE") && ($source ne "ONLINE")) {
   die "Source = '$source' is not valid.  Use 'OFFLINE' or 'ONLINE'\n";
}

die "$help\n" 
  unless (@ARGV);

my $dataset = shift;
my $output  = shift || "$dataset.output";

######################################
## Prepare Database.                ##
##   stolen from K. Lannon   2007   ##
##   stolen from J. Nielsen  2004   ##
##   stolen from E. Wicklund 2001   ##
######################################

my $dbhandle = DBI->connect( kDatabase, kDatabaseUser, kDatabasePass );

# Prepare a SELECT statement to select a range of runsections
my $select = "SELECT LUM_INTEGRAL_$source, SECTION_NUMBER FROM ".
  "FILECATALOG.CDF2_RUNSECTIONS ".
  "WHERE RUN_NUMBER = ? AND SECTION_NUMBER >= ? AND SECTION_NUMBER <= ?";

# This is what's used to actually query the DB below
my $selectionHandle = $dbhandle->prepare($select);


###########################
## Get Filelist From SAM ##
###########################
my @filelist = `sam list files --dim="CDF.DATASET $dataset" --noSummary`;
chomp @filelist;

## # For debugging only
## @filelist = @filelist[0..0];

# Run section luminosity hash
my %lumiHash = ();   

my @problems;
foreach my $file (@filelist) {
   
   print "Looking up file $file\n";

   # Regex's stolen from K. Lannon 2007

   # Get all the metadata for this file (and strip out newlines)
   chomp (my $metaData = `sam dump file --filename=$file`);
   $metaData =~ s/\n\s+//g;

   my @parents;
   if ($metaData =~ /'parents'\s+:\s+NameOrIdList\(\[(('[0-9a-z\.]+',\s+)*'[0-9a-z\.]+')\]\)/) {
      @parents = split /,\s+/, $1;
   } else {
      my $problem = "$file: metaData '$metaData' doesn't match pattern";
      warn $problem;
      push @problems, $problem;
      next;
   }

   #print "$file @parents\n";

   foreach my $parent (@parents) {

      print "  Parent file $parent\n";

      # Extract the list of "lumBlockRange" numbers, where a
      # "lumBlockRange" is a pair of numbers giving the minimum and
      # maximum run/section range for the file.  The format of each
      # "lumBlockRange" number is (runNumber) << 16 + section.

      # Get all the metadata for the parent file (and strip out newlines)
      chomp (my $metaDataParent = `sam dump file --filename=$parent`);
      $metaDataParent =~ s/\n\s+//g;
      
      # Extract lumi information
      my @lumiBlocks;
      if ($metaDataParent =~ /'lumBlockRangeList'\s+:\s+LumBlockRangeList\(\[((LumBlockRange\([0-9]+L,\s+[0-9]+L\),\s+)*LumBlockRange\([0-9]+L,\s+[0-9]+L\))\]\)/) {
         @lumiBlocks = split /\),\s+/, $1;
      } # if metadataparent
      else {
         my $problem = 
           "$file,$parent: parent metadata '$metaDataParent' doesn't match parent.";
         warn $problem;
         push @problems, $problem;     
         next;
      }
      
      foreach my $lumiBlock (@lumiBlocks) {
         my ($runSectionMin, $runSectionMax);
         if ($lumiBlock =~ /LumBlockRange\(([0-9]+)L,\s+([0-9]+)L/) {
            $runSectionMin = $1;
            $runSectionMax = $2;
         } # if lumiBlock
         else {
            # no match
            next;
         }

         # Run and section BigInts
         my $runSectionMinBI = Math::BigInt->new($runSectionMin);
         my $runSectionMaxBI = Math::BigInt->new($runSectionMax);

         # Split into run and section info
         my $runMinBI     = $runSectionMinBI >> 16;
         my $sectionMinBI = $runSectionMinBI & 0xffff;
         my $runMaxBI     = $runSectionMaxBI >> 16;
         my $sectionMaxBI = $runSectionMaxBI & 0xffff;

         # Turn into regular numbers
         my $runMin     = $runMinBI->numify();
         my $sectionMin = $sectionMinBI->numify();
         my $runMax     = $runMaxBI->numify();
         my $sectionMax = $sectionMaxBI->numify();

         # runMin and runMax should always be the same, or I'm not
         # sure what to do!
         if ($runMin != $runMax) {
            die "Extracted run number for the min and max range of the lum\n".
              " block ($lumiBlock) are not equal\n".
              "RunMin = $runMin\n RunMax = $runMax\n";
         }
         my $runNumber = $runMin;

         print "     $runNumber ($sectionMin, $sectionMax)\n";

         # DB access wizardry to pull out the luminosity. Attach the
         # specific parameters to this DB query
         $selectionHandle->bind_param(1, $runNumber);
         $selectionHandle->bind_param(2, $sectionMin);
         $selectionHandle->bind_param(3, $sectionMax);
         
         # Execute the DB query
         $selectionHandle->execute();

         # Associate variables with the columns in the table retured
         # by the query
         my $lum = 0;
         my $section = 0;
         $selectionHandle->bind_col(1,\$lum);
         $selectionHandle->bind_col(2,\$section);

         # Loop over the rows returned by the query
         while ($selectionHandle->fetch) {
            # Is luminosity negative or zero?
            if ($lum < 0.0) {
               print "ERROR: Run $runNumber, Section $section has ".\
                 "negative luminosity: $lum\n".
                 "Treating this as zero luminosity.\n";
               $lum = 0.0;
            } # negative luminosity
            elsif ($lum == 0.) {
               print "Warning: Run $runNumber Section $section has 0 lumi.\n";
            }
            $lumiHash{$runNumber}->[$section] = $lum;
         } # while fetch
      } # foreach lumiBlock
   } # foreach parent
} # foreach file

if (@problems) {
   printf "Problems: %d\n", scalar @problems;
   print join ("\n", @problems),"\n";
}

####################
## Fill In Blanks ##
####################
foreach my $run (sort keys %lumiHash) {
   my $aRef = $lumiHash{$run};
   my $maxSection = scalar @$aRef - 1;
   for my $loop (0..$maxSection) {
      # is this a blank section?
      if (! $aRef->[$loop]) {
         # make it 0
         $aRef->[$loop] = "0";
      } # fill in blank
   } # foreach loop
} # foreach run

#######################
## Write Output File ##
#######################
open (TARGET, ">", $output) or die "Can't open '$output' for output\n.";
foreach my $run (sort keys %lumiHash) {
   my $aRef = $lumiHash{$run};
   print TARGET "$run: @$aRef\n";
} # foreach run
close TARGET;
