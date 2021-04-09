#!/usr/bin/env perl

use warnings;
use strict;

# Load ErrorItem.pm.  Assume it is either in the current directory or
# the same directory as the script.
(my $dir = $0) =~ s|[^/]+$||;
my $line = sprintf ("use lib '%s'; use ErrorItem;", $dir);
my $retval = eval $line;
die "Unable to load ErrorItem:\n$@\n" if ($@);

my $errorListFile = "$dir/known_errors.txt";
my $version = "checkMClog.pl v1.00 cplager 060519";
(my $thisscript = $0) =~ s|.+/||g;

while ($ARGV[0] && $ARGV[0] =~ /^-/) {
   my $arg = shift @ARGV;
   $arg =~ s/^-+//;
   # help
   if ($arg =~ /^h/i) {
      help();
      exit;
   } 
   if ($arg =~ /^f/i) {
      $errorListFile = shift @ARGV;
      next;
   }
   # if we're still here, I don't understand the argument.  Fess up now
   die "I don't understand '$arg'.  Aborting.\n";
} # while args

# do we have files to check?
help() unless (@ARGV);


ErrorItem::readErrorItemListFromFile($errorListFile);
#ErrorItem::printErrorItemList();

foreach my $file (@ARGV) {
   print "\n\n\n-----Checking $file-----\n";
   my @list = ErrorItem::checkLogFile($file);
   my $errors = @list;
   if ($errors) {
      print "errors $errors\n";
      ErrorItem::printErrorItemList (@list);
   } else {
      print "  ** No Errors or Warnings Found **\n";
   }
}


sub help {
   no warnings;
   print "Usage: $thisscript [-args] file1 file2
--file XX      -> use 'XX' as the error list file
--help         -> This screen
$ErrorItem::version\n$version\n\n";
   exit;
}
