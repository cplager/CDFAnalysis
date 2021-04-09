package CLPQueue;

use strict;
use warnings;

use lib "/cdf/home/cplager/CPAN/lib";
#use lib "/uscms/home/cplager/CPAN/lib";
use File::NFSLock;
use Fcntl qw(LOCK_EX LOCK_NB);
use Errno qw(EAGAIN);
my $lock = 0;
my $smallpid;
my $lockdir;
my $lockfile;
my $pause;
my $host;
my $firstQuick = "true";
my $debug = "";
my $hiresSuccess;
my $lastSrand = 0;
my ($diefile, $pausefile, $pidsfilename, $allowedFile, $jobsfilename,
    $queuekillfile);

our $runQueue = "runCLPQueue.pl";

use Exporter;
our @ISA = qw (Exporter);

our @EXPORT = qw (
                  addJob
                  dieStatus
                  getLock
                  getMachineRequirements
                  getNjobs
                  getRunningJobs
                  killQueueJobs
                  launchNewJobs
                  listQueueJobs
                  listRunningJobs
                  pauseTime
                  psRunningJobs
                  queueKillJob
                  quickSleep
                  releaseLock
                  removeJobsFromPidList
                  setDieStatus
                  setPauseStatus
                  should_I_die
                  should_I_pause
                  startJob
                 );

BEGIN {
   # Ignore the children
   $SIG{CHLD} = 'IGNORE';
   $lockdir  = "$ENV{HOME}/.CLPQueue";
   $lockfile = "$lockdir/lockfile";
   chomp ($host = $ENV{HOSTNAME} || `hostname`);
   $host =~ s/\..+//;
   $smallpid = $$;
   while ($smallpid > 1) {
      $smallpid /= 10;
   }
   $pause = $smallpid * 10 + 5;
   if (! -d $lockdir) {
      mkdir $lockdir;
   }
   # Status files
   $diefile       = "$lockdir/dienow";
   $pausefile     = "$lockdir/pausenow";
   $pidsfilename  = "$lockdir/$host.pids";
   $allowedFile   = "$lockdir/allowed.file";
   $jobsfilename  = "$lockdir/jobs.list";
   $queuekillfile = "$lockdir/kill.queue";
   # lets make sure the following follows exist
   my @files = ($allowedFile, $jobsfilename, $pidsfilename, $queuekillfile);
   foreach my $file (@files) {
      system "touch $file" if (! -e $file);
   } # foreach $file
} # begin block

END {
   releaseLock() if (haveLock());
}

sub dieStatus {
   return (-e $diefile);
}

sub should_I_die {
   die "dying since $diefile exists" if (-e $diefile);
}

sub should_I_pause {
   return (-e $pausefile);
}

# 'setDieStatus(0)' clears status, '(1)' sets status
sub setDieStatus {
   my $status = shift;
   if ($status) {
      system "touch $diefile";
   } else {
      unlink $diefile;
   }
}

# 'setPauseStatus(0)' clears status, '(1)' sets status
sub setPauseStatus {
   my $status = shift;
   if ($status) {
      system "touch $pausefile";
   } else {
      unlink $pausefile;
   }
}

sub launchNewJobs {
   # don't even try it if we don't have the lock
   return unless haveLock();

   # Get the number of jobs allowed on this machine
   my $numAllowed = getMachineRequirements();
 
   my @runningJobs = getRunningJobs();
   my $numJobs    = @runningJobs;

   # Calculate how many jobs we want to start
   my $toStart    = $numAllowed - $numJobs;

   # Do we have any jobs to start?
   if ($toStart <= 0) {
      # no room to start anything
      return;
   }

   # if we're still here, then lets get some jobs to run.  Get the jobs
   # we want to start.
   my @jobs = getNjobs ($toStart);
   foreach my $job (@jobs) {
      startJob ($job);
   } # foreach job
}

sub quickSleep {
   my $time = shift || 0;
   my $factor = shift || 1;
   if ($firstQuick) {
      my $hires = "use Time::HiRes qw( usleep );";
      my $retval = eval $hires;
      if (! $@) {
         $hiresSuccess = "true";
      } else {
         $hiresSuccess = "";
      }
      $firstQuick = 0;
   }
   # should we call srand?
   if ($$ != $lastSrand) {
      srand ($$);
      $lastSrand = $$;
   }
   my $offset = rand() + $smallpid;
   if ($hiresSuccess) {
      usleep ( ($time + $offset * $factor) * 1e6 );
   } else {
      sleep (1 + $time + $offset * $factor);
   }
}

sub pauseTime {
   return $pause;
}

sub startJob {
   # don't even try it if we don't have the lock
   return unless haveLock();
   quickSleep();
   my $job = shift;
   # Ignore the children
   $SIG{CHLD} = 'IGNORE';
   FORK: 
   {
      if (my $pid = fork) {         
         # parent here
         print "parent starting: $$ $pid: $job\n";
         # Add child PID to PID files
         if (! open (PIDS, ">>", $pidsfilename) ) {
            warn "Can't open $pidsfilename\n";
            releaseLock();
            exit;
         }
         print PIDS "$pid $job\n";
         close PIDS;
         # all done
         return;
      }
      elsif (defined $pid) { # $pid is zero here if defined
         # child here
         # parent process pid is available with getppid
         # This process does NOT have the lock
         $lock = 0;
         my $pid = $$;
         print "child starting $job\n";
         # run the command after closing output
         open (STDOUT,">/dev/null");
         open (STDERR,">&STDOUT");
         system $job;
         # Job's finished.  Remove this job from the accounting
         system "touch $lockdir/removing.$pid" if ($debug);
         removeJobsFromPidList ($pid);
         system "touch $lockdir/removed.$pid" if ($debug);
         # Good.  Now just fade away
         exit;
      }
      elsif ($! == EAGAIN) {
         # EAGAIN is the supposedly recoverable fork error
         quickSleep (5 + $pause);
         redo FORK;
      }
      else {
         # weird fork error
         releaseLock();
         die "Can't fork: $!\n";
      }
   } # FORK block
}

sub removeJobsFromPidList {
   my @pids = @_;
   print "removing @pids from list\n";
   # UNLIKE most subroutines, we should NOT already have the lock.
   if (! haveLock()) {
      # Go get it, baby
      while (! getLock()) {
         system "touch $lockdir/gettinglock.$$" if ($debug);
         quickSleep ();
      }
   }
   # Get the PIDs
   if (! open (PIDS, "<", $pidsfilename) ) {
      warn "Can't open $pidsfilename\n";
      releaseLock();
      exit;
   }
   my %pids;
   while (<PIDS>) {
      chomp;
      s/#.+//; # get rid of comments
      if (/(\d+)/) {
         $pids{$1} = $_;
      }
   } # while PIDS
   close PIDS;
   # remove the current job
   foreach my $pid (@pids) {
      delete $pids{$pid};
   }
   # write out the rest
   if (! open (PIDS, ">", $pidsfilename) ) {
      warn "Can't open $pidsfilename\n";
      releaseLock();
      exit;
   }
   foreach my $pid (sort keys %pids) {
      print PIDS "$pids{$pid}\n";
   }
   close PIDS;   
   # give the lock back before leaving
   releaseLock();
}

sub listRunningJobs {
   my $quiet = shift || "";
   chomp (my $pwd = `pwd`);
   chdir $lockdir;
   my @files = glob ("*.pids");
   my $count = 0;
   foreach my $file (@files) {
      (my $name = $file) =~ s/\.pids$//;
      open (my $fh, "<", $file) or next;
      while (<$fh>) {
         chomp;
         if (/^(\d+)\s+(.+)/) {
            if ($quiet) {
               ++$count;
            } else {
               printf "  %2d) %s_%s        %s\n", ++$count, $name, $1, $2;
            }
         }
      }
      close $fh;
   } # foreach file
   return $count;
}

sub listQueueJobs {
   my $quiet = shift || "";
   if (! open (JOBS, "<", $jobsfilename) ) {
      warn "Can't open $jobsfilename\n";
      releaseLock() if haveLock();
      exit;
   }
   my $count = 0;
   while (<JOBS>) {
      chomp;
      next unless /\S/;
      if ($quiet) {
         ++$count;
      } else {
         printf "  %2d) %s\n", ++$count, $_;
      }
   } # while JOBS
   close JOBS;
   return $count;
}

sub addJob {
   my $job = "@_";
   return unless haveLock();
   if (! open (JOBS, ">>", $jobsfilename) ) {
      warn "Can't open $jobsfilename\n";
      releaseLock();
      exit;
   }
   print JOBS "$job\n";
}

sub queueKillJob {
   my @jobs = @_;
   # we need the lock.  If we don't already have it, get it.
   my $gotLock = "";
   if (! haveLock()) {
      while (! getLock()) {
         quickSleep ();
      }
      $gotLock = "true";
   }
   my %computers;
   foreach my $job (@jobs) {
      if ($job =~ /(.+)_(\d+)$/) {
         push @{$computers{$1}}, $2;
      } # if job
   } # foreach job
   my @localJobs;
   # Before going any further, take out the local pids to be killed
   if ($computers{$host}) {
      @localJobs = @{$computers{$host}};
      delete $computers{$host};
   }
   # All done.  Append to $queuekillfile
   if (! open (TOKILL, ">>", $queuekillfile)) {
      warn "Can't open $queuekillfile\n";
      releaseLock();
      exit;
   } 
   foreach my $computer (sort keys %computers) {
      my $aref = $computers{$computer};
      print "$computer @$aref\n";
      print TOKILL "$computer @$aref\n";
   }
   close TOKILL;
   killRunningJobs (@localJobs) if (@localJobs);
   # release the lock ONLY IF we got it in this subroutine
   releaseLock() if ($gotLock);
}

sub killRunningJobs {
   my @pids = @_;
   return unless haveLock();
   ##############################
   ## Load the PID Information ##
   ##############################
   if (!open (PS, 'ps -eao "%p %P %a" |')) {
      warn "Can't call ps\n";
      releaseLock() if (haveLock());
   }
   my %parentHash;
   my %childHash;
   my %commandHash;
   my %pidHash;
   $pidHash{$_} = 0 foreach @pids;
   while (<PS>) {
      if (/^\s*(\d+)\s+(\d+)\s+(.+)/) {
         my $pid = $1;
         my $parent = $2;
         my $command = $3;
         $parentHash{$pid} = $parent;
         push @{$childHash{$parent}}, $pid;
         if (defined $pidHash{$pid} && $command =~ /$runQueue/) {
            $commandHash{$pid} = 1;
         }
      } # if 
   } # while PS
   close PS;

   ##############################
   ## Find all of the children ##
   ##############################
   my @toKill;
   foreach my $pid (@pids) {
      next unless ($commandHash{$pid});
      my @parents;
      push @parents, $pid;
      while (my $parent = shift @parents) {
         my @children = @{$childHash{$parent}};
         next unless @children;
         push @toKill, @children;
         push @parents, @children;
      } # while parent
   } # foreach pid
   if (@toKill) {
      print "Killing pids @toKill\n";
      kill (9, @toKill) if (@toKill);
   } else {
      print "Found nothing to kill.\n";
   }   
}

sub killQueueJobs {
   my @tokill = @_;
   return unless haveLock();
   if (! open (JOBS, "<", $jobsfilename) ) {
      warn "Can't open $jobsfilename\n";
      releaseLock();
      exit;
   }
   my $count = 0;
   my %jobsHash;
   while (<JOBS>) {
      chomp;
      next unless /\S/;
      $jobsHash{++$count} = $_;
   } # while JOBS
   close JOBS;
   foreach my $index (@tokill) {
      $index =~ s/^0+//;
      if ($jobsHash{$index}) {
         delete $jobsHash{$index};
      } else {
         warn "'$index' is not a current job\n";
      } # else if
   } # foreach
   if (! open (JOBS, ">", $jobsfilename) ) {
      warn "Can't open $jobsfilename\n";
      releaseLock();
      exit;
   }
   foreach my $index (sort {$a <=> $b} keys %jobsHash) {
      print JOBS "$jobsHash{$index}\n";
   }
   close JOBS;
}

sub getNjobs {
   # don't even try it if we don't have the lock
   return unless haveLock();
   my $num = shift;
   return unless (-e $jobsfilename);
   if (! open (JOBS, "<", $jobsfilename) ) {
      warn "Can't open $jobsfilename\n";
      releaseLock();
      exit;
   }
   my @jobs;
   while (<JOBS>) {
      s/#.+//; # get rid of comments
      next unless (/\S/); # make sure something is here
      chomp;
      push @jobs, $_;
   } # while JOBS
   if ($num >= @jobs) {
      unlink $jobsfilename;
      system "touch $jobsfilename";
      return @jobs;
   }
   my @retval;
   for (1..$num) {
      my $job = shift @jobs;
      push @retval, $job;
   }
   # write the rest of the jobs back into the file
   if (! open (JOBS, ">", $jobsfilename) ) {
      warn "Can't open $jobsfilename\n";
      releaseLock();
      exit;
   }
   print JOBS join ("\n", @jobs), "\n";
   close JOBS;
   return @retval;
}

sub getMachineRequirements {
   # don't even try it if we don't have the lock
   return unless haveLock();
   my %allowed;
   if (! -e $allowedFile) {
      return 0;
   }
   if (! open (ALLOWED, "<", $allowedFile) ) {
      warn "Can't open $allowedFile\n";
      releaseLock();
      exit;
   }   
   while (<ALLOWED>) {
      s/#.+//; # get rid of comments
      if (/(\w+)\s+(\w+)/) {
         $allowed{$1} = $2;
      } #
   } # while allowed
   return $allowed{$host} || 0;
}

sub psRunningJobs {
   # don't even try it if we don't have the lock
   return unless haveLock();
   return unless (-e $pidsfilename);
   if (! open (PIDS, "<", $pidsfilename) ) {
      warn "Can't open $pidsfilename\n";
      releaseLock();
      exit;
   }
   my %pids;
   while (<PIDS>) {
      chomp;
      s/#.+//; # get rid of comments
      if (/(\d+)/) {
         $pids{$1} = 0;
      }
   } # while PIDS
   close PIDS;
   if (! open (PS, "ps -eaf|") ) {
      warn "Can't open ps\n";
      releaseLock();
      exit;
   }
   while (<PS>) {
      chomp;
      if (/^\w+\s*(\d+)/) {
         my $pid = $1;
         if (exists $pids{$pid}) {
            $pids{$pid} = $_;
         } # if this is on our list
      } # if some digits
   } # while PS
   foreach my $pid ( sort {$a <=> $b} keys %pids) {
      if ($pids{$pid}) {
         print "found $pid: $pids{$pid}\n";
      } else {
         print "  MISSING $pid\n";
      }
   } # foreach $pid
}

sub getRunningJobs {
   # don't even try it if we don't have the lock
   return unless haveLock();
   return unless (-e $pidsfilename);
   if (! open (PIDS, "<", $pidsfilename) ) {
      warn "Can't open $pidsfilename\n";
      releaseLock();
      exit;
   }
   my %pids;
   while (<PIDS>) {
      chomp;
      s/#.+//; # get rid of comments
      if (/(\d+)/) {
         $pids{$1} = $_;
      }
   } # while PIDS
   close PIDS;
   return unless keys %pids;
   
   my @runningPids;
   @runningPids = keys %pids;
   return @runningPids;
   
   if (! open (PS, "ps -e|") ) {
      warn "Can't open ps\n";
      releaseLock();
      exit;
   }
   while (<PS>) {
      if (/^\s*(\d+)/) {
         my $pid = $1;
         if ($pids{$pid}) {
            push @runningPids, $_;
         } # if this is on our list
      } # if some digits
   } # while PS
   # should we rewrite this file?
   if (scalar @runningPids != scalar keys %pids) {
      # looks like a job finished.  I'll rewrite the list
      # Are there any left
      if (! @runningPids) {
         unlink $pidsfilename;
         return;
      }
      if (! open (PIDS, ">", $pidsfilename) ) {
         warn "Can't open $pidsfilename\n";
         releaseLock();
         exit;
      }
      foreach my $pid (@runningPids) {
         print PIDS "$pids{$pid}\n";
      }
      #print PIDS join("\n", sort @runningPids),"\n";
      close PIDS;      
   } # mismatch
   return @runningPids;
}

sub haveLock {
   return "true" if ($lock);
   return;
}

sub getLock {
   if ($lock = new File::NFSLock {file      => $lockfile,
                                  lock_type => LOCK_EX|LOCK_NB,
                                  blocking_timeout   => 10,      # 10 sec
                                  stale_lock_timeout => 10 * 60, # 10 min
                               }) {
      return "true";
   } else {
      $lock = 0;
      return;
   }
}

sub releaseLock {
   if (! haveLock()) {
      warn "releaseLock: Never had lock.\n";
      return;
   }
   ### unlock it
   $lock->unlock();
   return "true";
}


1;
