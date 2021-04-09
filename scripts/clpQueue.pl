#!/usr/bin/env perl

use strict;
use warnings;
use POSIX qw(:errno_h);
#use Errno qw(EAGAIN);

# Load CLPQueue.pm.  Assume it is either in the current directory or
# the same directory as the script.
(my $dir = $0) =~ s|[^/]+$||;
$dir =~ s|/$||;
my $line = sprintf ("use lib '%s'; use CLPQueue;", $dir);
my $retval = eval $line;
die "Unable to load CLPQueue:\n$@\n" if ($@);

(my $thiscommand = $0) =~ s|.+/||g;

# Option parcing
while (@ARGV && $ARGV[0] =~ /^-/) {
   (my $arg = shift) =~ s/^-+//;
   die help() if ($arg =~ /^h/i);
   die "I don't understand '-$arg'.  Use '--help' for options. Aborting.\n";
}

my $arg = shift;

if (!$arg) {
   die help();
}

# cleardie
if ($arg =~ /^cleardie/i || $arg =~ /^cd/i) {
   setDieStatus(0);
   if (dieStatus()) {
      print "Die status: set\n";
   } else {
      print "Die status: cleared\n";
   }
   exit;
}

# clearpause
if ($arg =~ /^clearpause/i || $arg =~ /^cp/i) {
   setPauseStatus(0);
   if (should_I_pause()) {
      print "Pause status: set\n";
   } else {
      print "Pause status: cleared\n";
   }
   exit;
}      

# setdie
if ($arg =~ /^setdie/i || $arg =~ /^sd/i) {
   setDieStatus(1);
   if (dieStatus()) {
      print "Die status: set\n";
   } else {
      print "Die status: cleared\n";
   }
   exit;
}

# setpause
if ($arg =~ /^setpause/i || $arg =~ /^sp/i) {
   setPauseStatus(1);
   if (should_I_pause()) {
      print "Pause status: set\n";
   } else {
      print "Pause status: cleared\n";
   }
   exit;
}

# status
if ($arg =~ /^status/i || $arg =~ /^st/i || $arg =~ /^s$/i) {
   if (dieStatus()) {
      print "Die status:   set\n";
   } else {
      print "Die status:   cleared\n";
   }
   if (should_I_pause()) {
      print "Pause status: set\n";
   } else {
      print "Pause status: cleared\n";
   }
   listAllJobs();
   exit;
}

# count
if ($arg =~ /^co/i) {
   printJobCounts();
   exit;
}

# listjobs
if ($arg =~ /^listjobs/i || $arg =~ /^lj/i) {
   print "Running jobs:\n";
   listAllJobs();
   exit;
}
   
# killqueue
if ($arg =~ /^killqueue/i || $arg =~ /^kq/i) {
   if (! should_I_pause() && ! dieStatus()) {
      die "You must set pause or die modes to use this function.\n";      
   }
   getTheLock();
   killQueueJobs (@ARGV);
   releaseLock();
   print "done\n";
   exit;
}

# killrunning
if ($arg =~ /^killrunning/i || $arg =~ /^kr/i) {
   queueKillJob (@ARGV);
   exit;
}

# pauselist
if ($arg =~ /^pauselist/i || $arg =~ /^pl/i) {
   setPauseStatus(1);
   if (should_I_pause()) {
      print "Pause status: set\n";
   } else {
      print "Pause status: cleared\n";
   }
   getTheLock();
   listAllJobs();
   releaseLock();
   print "done\n";
   exit;
}

# ps jobs on this computer
if ($arg =~ /^ps/i) {
   getTheLock();
   psRunningJobs();
   releaseLock();
   print "done\n";
   exit;
}

# addjob
if ($arg =~ /^addjob/i || $arg =~ /^aj/i) {
   getTheLock();
   addJob (@ARGV);
   releaseLock();
   print "done\n";
   exit;
}


# start daemon
if ($arg =~ /^da/i) {
   print "starting daemon on this computer\n";



   $SIG{CHLD} = 'IGNORE';
   FORK: 
   {
      if (my $pid = fork) {         
         # parent here nothing to do
         exit;
      } elsif (defined $pid) {     # $pid is zero here if defined
         # child here
         for my $handle (*STDIN, *STDOUT, *STDERR) {
            open($handle, "+<", "/dev/null")
              || die "can't reopen $handle to /dev/null: $!";
         }
         use POSIX;

         POSIX::setsid( )
             or die "Can't start a new session: $!";

         ## my $time_to_die = 0;
         ## 
         ## sub signal_handler {
         ##    $time_to_die = 1;
         ## }
         ## 
         ## $SIG{INT} = $SIG{TERM} = $SIG{HUP} = \&signal_handler;
         # trap or ignore $SIG{PIPE}
         no warnings;
         exec "$dir/$CLPQueue::runQueue -fork";
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
   exit;
}

# If we're still here, then they didn't give us a command we recognize.
die "Don't recognize '$arg' command.\n";

sub getTheLock {
   $| = 1;
   print "Getting lock.";
   while (! getLock()) {
      quickSleep ();
      print ".";
   }
   print " Done.\n";
   $| = 0;
}

sub listAllJobs {
   print "Running jobs:\n";
   if (!listRunningJobs()) {
      print "  None\n";
   }
   print "Jobs in queue:\n";
   if (!listQueueJobs()) {
      print "  None\n";
   }
}

sub printJobCounts {
   printf "Running: %2d  Queued: %2d\n", listRunningJobs ("quiet"),
     listQueueJobs("quiet");
}

sub help {
   return "Usage: $thiscommand --options commands
commands:
(aj) addjob cmd 1 2  - adds a single job to queue
(cp) clearpause      - clears pause mode
(cd) cleardie        - clears die mode
(co) countJobs       - prints number of jobs running and queued
(da) daemon          - starts the queue daemon running on this computer
(kq) killqueue XX YY - kills queue jobs listed 
                       (must be in 'pause' or 'die' mode)
(kr) killrun  XX     - kills job running
(lj) listjobs        - lists all jobs
(pl) pauselist       - set the pause and then list the jobs 
                       (use before 'killqueue')
(sp) setpause        - set pause mode
(sd) setdie          - set die mode
(st) status          - status of queue

options:
--help               - this screen

Notes: 

1) 'pause' and 'die' mode only affect jobs that are going to be
launched.  They have no effect on jobs that are currently running.  
";


}
