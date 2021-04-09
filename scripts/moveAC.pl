#!/usr/bin/env perl

use warnings;
use strict;

use constant toLog   => "toLog";
use constant toGen   => "toGen";

# Load Date.pm.  Assume it is either in the current directory or
# the same directory as the script.
(my $dir = $0) =~ s|[^/]+$||;
my $line = sprintf ("use lib '%s'; use Date;", $dir);
my $retval = eval $line;
die "Unable to load Date:\n$@\n" if ($@);


my $remotebackup = "/cdf/home/cplager/bin/backupNeeded";

my %dirhash =
(
 "/home/cplager/analysis" => "/cdf/home/cplager/analysis/614",
);

my $logdir = "/cdf/home/cplager/log";
my $gendir = "/cdf/home/cplager/public_html/internal/Work/log";

my ($nobackup, $header);
while (@ARGV && $ARGV[0] =~ /^-/) {
   (my $arg = shift @ARGV) =~ s/^\-+//;
   if ($arg =~ /^header/i) {
      $header = "true";
      next;
   }
   next;
} # while args

my $tofrom = shift || "";

if($tofrom =~ /log/i) {
   $tofrom = toLog;
} elsif ($tofrom =~ /^togen/i) {
   $tofrom = toGen;
} else {
   @ARGV = ();
};

my ($date, $shortdate);
if ($tofrom eq toLog || $tofrom eq toGen) {
   my $today = Date->today;
   $date = $today->key;
   $shortdate = $today->short_key;
}

die "Usage: $0 toLog file1 [file2...]\n" unless (@ARGV);

my $current = $ENV{PWD};

my %filehash;
foreach my $file (@ARGV) {
   # don't copy '~' or '.bak' files
   next if ($file =~ /~$/ || $file =~ /\.bak$/);
   # don't allow '..'
   die "Can not use '..' in filename (\"$file\")\n" if ($file =~ m|\.\.|);
   print "\n";
   if ($file !~ m|^/|) {
      $file = "$current/$file";
   }
   if ($tofrom eq toLog) {
      (my $temp = $file) =~ s|.+/||g;
      my $remote = "$logdir/$shortdate/$date\_$temp";
      $filehash{$file} = $remote;
      printImageHtml ("$date\_$temp");
      next;
   } elsif  ($tofrom eq toGen) {
      (my $temp = $file) =~ s|.+/||g;
      my $remote = "$gendir/$shortdate/$date\_$temp";
      $filehash{$file} = $remote;
      printImageHtml ("$date\_$temp");
      next;
   }
   my $found = "";
   foreach my $dir (keys %dirhash) {
      if ($file =~ m|^$dir|) {
         $found = "true";
         (my $remote = $file) =~ s|$dir|$dirhash{$dir}|;
         $filehash{$file} = $remote;
         last;
      } # if file found
   } # foreach $dir
   die "Can't find where to put $file.\n" unless $found;
} # foreach file

foreach my $file (sort keys %filehash) {
   copyFile ($tofrom, $file);
}


##################################################################

sub printImageHtml {
   my $filename = shift;
   if ($filename =~ /\.gif$/i || $filename =~ /\.png$/i) {
      (my $eps = $filename) =~ s/\.gif$/.eps/i;
      $eps =~ s/\.png/.eps/i;
      #print "<a href=\"$eps\">  <img src=\"$filename\"> </a>\n";
      if ($header) {
         (my $pretty = $eps) =~ s/\.eps//;
         $pretty =~ s/^\d{6}_//;
         print " <h4> $pretty </h4>\n";
      }
      print " <p>  <img src=\"$filename\">  <br>\n";
   } else {
      (my $normal = $filename) =~ s/^\d{6}_//;
      print " <a href=\"$filename\"><tt>$normal</tt></a>\n";
   }
}

sub copyFile {
   my $tofrom = shift;
   my $file = shift;
   my $remote = $filehash{$file};
   warn "Can't find remote file for \"$file\".  Skipping.\n" unless $remote;
   # just to be safe, backup both local and remote copies
   backupRemote ($file);
   # copy to $host
   print "copying $file\n";
   my $command = "cp $file $remote";
   system $command;

}

sub backupRemote {
   my $file = shift;
   my $remote = $filehash{$file};
   my $base;
   if ($remote =~ m|([^/]+)$|) {
      $base = $`;
      $remote = $1;
   } else {
      return;
   }
   system "cd $base; $remotebackup -ifexists $remote";
}
