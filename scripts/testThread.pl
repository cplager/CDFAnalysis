#!/usr/bin/env perl

use strict;
use warnings;

use threads;
use Thread::Queue;

my $queue = new Thread::Queue;

# Option parcing
while (@ARGV && $ARGV[0] =~ /^-/) {
   (my $arg = shift) =~ s/^-+//;
   die "I don't understand '-$arg'.  Aborting.\n";
}


for my $loop (2..8) {
   print "$loop\n";
   $queue->enqueue ($loop);
}
#$queue->enqueue (undef);



while (my $pend = $queue->pending) {
   my @threadList = threads->list;
   my $num = @threadList;
   print "[$pend, $num] ";
   sleep 1;
   if ($num < 4) {
      my $thr = threads->new (\&runByThread);
      #$thr->detach;
   }
}

#threads->exit();

# cleanup
foreach my $thr (threads->list) {
   $thr->join();
}

sub runByThread {
   #my $queue = shift;
   my $entry = $queue->dequeue_nb;
   if ($entry) {
      print "\nThread $entry\n";
      sleep 5;
   }
   print "\ndone\n";
   threads->self()->join();
}

