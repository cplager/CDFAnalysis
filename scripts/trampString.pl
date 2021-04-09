#!/usr/bin/env perl

use warnings;
use strict;
use constant newtramp => "newtramp";
use constant oldtramp => "oldtramp";
use constant hobo     => "hobo";

# extentions to check if there is no direct match
my @extentions = qw (cc c hh h pl pm);


chomp (my $user = `whoami`);
chomp (my $host = `hostname`);

my $mode = newtramp;
while (@ARGV && $ARGV[0] =~ /^-/) {
   my $arg = shift @ARGV;
   $arg =~ s/^-+//;
   if ($arg =~ /^n/i) {
	  $mode = newtramp;
      next;
   }
   if ($arg =~ /^o/i) {
	  $mode = oldtramp;
      next;
   }
   if ($arg =~ /^ho/i) {
	  $mode = hobo;
      next;
   }
}

my $file = $ARGV[0] || $ENV{PWD};

# does file start with '/' indicating full path?
if ($file !~ m|^/|) {
   $file = $ENV{PWD}."/$file";
}

# lets see if this file exists?
my $notthere = "";
if (! -f $file) {
   $notthere = "true";
} elsif (`uname` =~ /cygwin/i) {
   # Cygwin hack - the problem is that cygwin matches 'bla' and 'bla.'
   # to 'bla.exe'.  I don't want these matches, so I check explicity
   # that the file names really match
   my @files = glob ("$file*");
   my $found = "";
   foreach my $trial (@files) {
      if ($trial eq $file) {
         $found = "true";
         last;
      } # if match
   } # foreach listed file
   $notthere = "true" unless ($found);
}

if ($notthere) {
   # does the file end in '.'
   my $tempfile = $file;
   if ($tempfile !~ /\.$/) {
      $tempfile .= ".";
   }
   foreach my $extention (@extentions) {
      if (-f $tempfile.$extention) {
         $file = $tempfile.$extention;
         last;
      } # if file exists
   } # foreach file
} # if file exists

if ($mode eq newtramp) {
   print "/$user\@$host:$file\n";
} elsif ($mode eq oldtramp) {
   print "/[$user\@$host]$file\n";
} elsif ($mode eq hobo) {
   print "$file\n";
}
