#!/usr/bin/env perl

use strict;
use warnings;

# Option parcing
my ($treename, $nosam, $filelist, $listNeededSam);
while (@ARGV && $ARGV[0] =~ /^-/) {
   (my $arg = shift) =~ s/^-+//;
   if ($arg =~ /^treename/i) {
      $treename = shift;
      next;
   }
   if ($arg =~ /^filelist/i) {
      $filelist = shift;
      next;
   }
   if ($arg =~ /^nosam/i) {
      $nosam = "true";
      next;
   }
   if ($arg =~ /^listsam/i) {
      $listNeededSam = "true";
      next;
   }
   if ($arg =~ /^h/i) {
      help();
   };
   die "I don't understand '-$arg'.  Aborting.\n";
}

my $temproot = "temproot$$.C";
if ($treename) {
   open (TEMPROOT, ">", $temproot) or die;
   print TEMPROOT "{\n   cout << \"entries \" << $treename->GetEntries() << endl;\n}\n";
   close TEMPROOT;
}
   
my $listlength = 0;
if ($filelist) {
   $listlength = getNumInFileList ($filelist);
}

help() unless (@ARGV);

my ($dataset, @samlist);
unless ($nosam) {
   $dataset = shift || "" unless $nosam;
   ###########################
   ## Get Filelist From SAM ##
   ###########################
   @samlist = `sam list files --dim="CDF.DATASET $dataset" --noSummary`;
   chomp @samlist;
   die "Must be a bad dataset ID.  No files listed\n" unless (@samlist);
}



my (%openHash, %closeHash);
my @logfiles = @ARGV;
my  $totalRecorded = 0;
my @notsame;
foreach my $file (@logfiles) {
   open (LOG, "<", $file) or die;
   my @usedFiles;
   my $output = "";
   my $localnum = 0;
   my $finishednum = 0;
   while (<LOG>) {
      if (/TreeFileInterface: Opening file\s+(\S+)\.\s.+contai\S+\s+(\d+)\s+events/) {
         my $events = $2;
         (my $file = $1) =~ s|.+/||g;
         #print "file $file - $events\n";
         $openHash{$file} = $events;
         next;
      } # if opening
      if (/TreeFileInterface: Finishing file\s+(\S+) reading (\d+) events/) {
         my $events = $2;
         (my $file = $1) =~ s|.+/||g;
         #print "file $file - $events\n";
         $closeHash{$file} = $events;
         push @usedFiles, $file;
         $localnum += $events;
         next;
      } # if finishing
      if (/Skimming to (\S+)/) {
         $output = $1;
      }
      if (/finished gracefully with (\d+) events/i) {
         $finishednum = $1;
      }
   } # while 
   if ($treename) {
      if (! $output || ! $finishednum) {
         warn "Didn't find what I need\n";
         next;
      }
      my $rootnum = getEntriesInRootFile ($output, $treename);
      #print "checking $output: $rootnum $finishednum\n";
      if ($rootnum != $finishednum) {
         warn "Rootfile '$output' does not have the number of events the log file says it should.\n";
         foreach my $file (@usedFiles) {
            push @notsame, $file;
            delete $openHash{$file};
            delete $closeHash{$file};
         } # foreach $file
      } # if number of events don't match
   } # if checking root files
   $totalRecorded += $finishednum;
} # foreach $file
unlink  $temproot if (-e $temproot);

my (@notclosed, @good, $numEvents);
foreach my $key (sort keys %openHash) {
   if (! exists $closeHash{$key}) {
      push @notclosed, $key;
      next;
   }
   if ($closeHash{$key} == $openHash{$key}) {
      $numEvents += $closeHash{$key};
      push @good, $key;
      next;
   } else {
      push @notsame, "$key : $openHash{$key} $closeHash{$key}";
   }   

}

my (%samHash, %goodHash);
$samHash {$_} = $_ foreach @samlist;
delete $samHash{$_} foreach @good;
my @needed = sort keys %samHash;
# print "good:\n", join("\n", @good), "\n" if (@good);
# print "notclosed:\n", join("\n", @notclosed), "\n" if (@notclosed);
# print "notsame:\n", join("\n", @notsame), "\n" if (@notsame);

printf("Summary:\n".
       "Number of Log Files:   %d\n".
       "Total Events seen:     %d\n".
       "Total Events Skimmed:  %d\n".
       "Num Good files:        %d\n".
       "Num not closed:        %d\n".
       "Num not same:          %d\n".
       "Sam files:             %d\n".
       "Needed SAM files:      %d\n",
       scalar @logfiles, $numEvents, $totalRecorded,
       scalar @good, scalar @notclosed, 
       scalar @notsame, scalar @samlist, scalar @needed);
printf("Filelist Length:       %d\n", $listlength) if ($filelist);

if ($listNeededSam) {
   printf "Needed SAM files (%d)\n", scalar @needed;
   print join ("\n", @needed), "\n";
}

sub getNumInFileList {
   my $filelist = shift;
   if (!open (FILELIST, "<", $filelist)) {
      warn "Can't open '$filelist'\n";
      return 0;
   }
   my $count = 0;
   while (<FILELIST>) {
      s/\#.+//g;
      ++$count if (/\S/);
   }
   return $count;
} 

sub getEntriesInRootFile {
   my $file = shift;
   my $treename = shift;
   my $command = "root -b -l -q $file $temproot 2>&1";
   #system $command;
   #print "command $command\n";
   my $output = `$command`;
   #print "output $output\n";
   if ($output =~ /entries (\d+)/) {
      return $1;
   } else {
      return 0;
   }
}

sub help {
   (my $scriptname = $0) =~ s|.+/||g;
   die "$scriptname [--options] SAMID log1 [log2 log3]
Options:
--filelist XX  counts number of files in given filelist
--help         This help screen
--listsam      List needed SAM files
--nosam        Drops need for 'SAMID' - used for counting events for jobs
               run over filelists
--treename XX  Tells script to check root files, counting number of events 
               in trees named 'XX'
";
}
