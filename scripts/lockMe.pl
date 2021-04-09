#!/usr/bin/env perl

use warnings;
use strict;


###########################################################################
## IMPORTANT: The locking mechanism here ONLY works on a single computer ##
## at a time.  Locks on one computer are NOT RESPECTED on another.       ##
###########################################################################
use Fcntl ':flock'; # import LOCK_* constants
use constant kNoStatus     => 0;
use constant kLockFileOpen => 1;
use constant kHaveLock     => 2;

my $lockHandle;
my $lockFile = "lockFile";
my $lockStatus = kNoStatus;

if (getLock()) {
   print "got lock\n";
   sleep 10;
   if (releaseLock()) {
      print "released lock\n";
   } else {
      print "crap\n";
   }
   sleep 10;
} else {
   print "didn't get lock\n";
}

sub getLock {
   # Do we need to open the lock file?
   if (kNoStatus == $lockStatus) {
      open ($lockHandle, ">>", $lockFile) or die "Cannot open $lockFile: $!";
      $lockStatus = kLockFileOpen
   }
   if (flock ($lockHandle, LOCK_EX | LOCK_NB)) {
      # got lock
      $lockStatus = kHaveLock;
      return "true";
   } else {
      # don't have lock
      close $lockHandle;
      $lockStatus = kNoStatus;
      return;
   }
}

sub releaseLock {
   if (kHaveLock != $lockStatus) {
      warn "You called releaseLock() without having the lock.\n";
      return;
   }
   flock ($lockHandle, LOCK_UN);
   close $lockHandle;
   return "true";
}
