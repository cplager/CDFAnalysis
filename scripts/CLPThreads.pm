package CLPThreads;

########################################################################
##  
## # Make an instance, passing in whatever subroutine you wish to be
## # called ('hello' in this case).
## my $thread = CLPThreads->new (\&hello);
## 
## # Tell the instance how you would like it called.  Add as many
## # arguments as needed.
## for my $loop (1..20) {
##    $thread->addJob ($loop);
## } # for loop
## 
## # Run the threads (in this case 4 at a time)
## $thread->run (4);
##  
########################################################################

use strict;
use warnings;

use threads;
use threads::shared;
use Thread::Queue;

# Shared variables
my %working      : shared;
my %instructions : shared;
my $subroutine   : shared;
my %queueHash    : shared;

# Unshared variables (main "thread" only)
my $idCount = 1;
my @threadArray;

# must pass in subroutine reference as '1st' argument
sub new {
   my $class = shift;
   my $self = {};
   bless ($self, $class);
   $self->{subref} = shift || die;
   $self->{id}     = $idCount++;
   $self->{count}  = 0;
   my $ref = new Thread::Queue;
   # push @queueArray, $ref;
   $queueHash{$self->{id}} = share ($ref);
   return $self;
}

# Pass in as many arugments as you wish
sub addJob {
   my $self = shift;
   my $key = sprintf ("%d_%d", $self->{id}, $self->{count});
   $self->{count} += 1;
   # We have to tell it that it will be sharing an anonymous array
   # reference.
   $instructions{$key} = &share([]);
   # Now that we've told it, copy the variables onto the array
   push @{$instructions{$key}}, @_;
   my $queue = $queueHash {$self->{id}};
   $queue->enqueue( $key );
}

# Pass in the number of threads you wish to run simultaneously
sub run {
   my $self = shift;
   my $numThread = shift || die;
   # print "nT $numThread\n";
   for my $loop (1..$numThread) {
      # print "starting thread $loop\n";
      my $key = sprintf ("%d_%d", $self->{id}, $loop);
      my $thr = threads->new (\&_runManyThread, $self->{id}, $key, 
                              $self->{subref});
      push @threadArray, $thr;
   } # forloop
}

#################################################################

sub _runManyThread {
   my $id = shift;
   my $key = shift;
   my $subref = shift;
   my $queue = $queueHash{$id};
   $working{$id} = $key;
   while ($queue->pending) {
      print "thread $key\n";
      _runNextJob ($subref, $queue);
   }
   delete $working{$id};
}

sub _runNextJob {
   my $subref = shift;
   my $queue  = shift;
   no strict "refs";
   my $key = $queue->dequeue_nb;
   return if (! $key);
   my $aref = $instructions{$key};
   # &$subref ($key, @$aref); # for debugging only
   &$subref (@$aref);
   return "done";
}

sub _cleanup {
   while (my $num = keys %working) {
      #print "    $num running\n";
      select (undef, undef, undef, 1); # sleep 0.1      
   } # while waiting for threads to finish
   foreach my $thread (@threadArray) {
      $thread->join();
   } # joining all threads
}

END {
   _cleanup();
}

1;
