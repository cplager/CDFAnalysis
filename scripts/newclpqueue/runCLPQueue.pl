#!/usr/bin/env perl

use strict;
use warnings;
use Errno qw(EAGAIN);

# Load CLPQueue.pm.  Assume it is either in the current directory or
# the same directory as the script.
(my $dir = $0) =~ s|[^/]+$||;
my $line = sprintf ("use lib '%s'; use CLPQueue;", $dir);
my $retval = eval $line;
die "Unable to load CLPQueue:\n$@\n" if ($@);

should_I_die(); # failsafe to prevent runaway daemons

# Option parcing
my $fork;
my $allArgs;
my $repeatEvery = 5;
# copy all arguments onto '$allArgs' so they will be available for
# 'forked' copy
while (@ARGV && $ARGV[0] =~ /^-/) {
   (my $arg = shift) =~ s/^-+//;
   if ($arg =~ /fork/i) {
      $fork = "true";
      $allArgs .= " -fork";
      next;
   }
   if ($arg =~ /repeat/i) {
      my $temp = shift || "";
      if ($temp =~ /(\d+)/) {
         $repeatEvery = $1;
         $allArgs .= " -repeat $repeatEvery";
      }
      next;
   }
   die "I don't understand '-$arg'.  Aborting.\n";
}

# sleep first thing so that if multiple jobs on different computers
# are started at the same time, we can get some offset between them.
quickSleep();
 
while (! getLock()) {
   quickSleep ();
}

# If we want to temporarily stop job submission
launchNewJobs() unless should_I_pause();

releaseLock();

exit unless $fork;

# Is this suposed to repeat itself
$SIG{CHLD} = 'IGNORE';
FORK: 
{
   if (my $pid = fork) {         
      # parent here nothing to do
      exit;
   } elsif (defined $pid) {     # $pid is zero here if defined
      # child here
      # parent process pid is available with getppid
      # close output
      open (STDOUT,">/dev/null");
      open (STDERR,">&STDOUT");
      sleep $repeatEvery;
      exec "$0 $allArgs";
      # should never get here
      exit;
   } elsif ($! == EAGAIN) {
      # EAGAIN is the supposedly recoverable fork error
      sleep (5);
      redo FORK;
   } else {
      # weird fork error
      die "Can't fork: $!\n";
   }
} # FORK block
