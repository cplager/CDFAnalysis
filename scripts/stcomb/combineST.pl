#!/usr/bin/env perl

use warnings;
use strict;

use constant {
   kParticle => "particle",
};

my $version = "combineST.pl v1.00 cplager 071214";

# Load GenObject.pm.  Assume it is either in the current directory or
# the same directory as the script.
(my $dir = $0) =~ s|[^/]+$||;
my $line = sprintf ("use lib '%s'; use GenObject;", $dir);
my $retval = eval $line;
die "Unable to load GenObject:\n$@\n" if ($@);

# Option parcing
my $output = "tchan.txt";
my $problem = "tchan_problems.txt";
my ($only, $warn);
while (@ARGV && $ARGV[0] =~ /^-/) {
   (my $arg = shift) =~ s/^-+//;
   if ($arg =~ /^output/i) {
      $output = shift || die;
      next;
   }
   if ($arg =~ /^problem/i) {
      $problem = shift || die;
      next;
   }
   if ($arg =~ /^only/i) {
      $only = "true";
      next;
   }
   if ($arg =~ /^warn/i) {
      $warn = "true";
      next;
   }

   die "I don't understand '-$arg'.  Aborting.\n";
}

my (@LFfiles, @MEfiles, @NNfiles);
foreach my $file (@ARGV) {
   if ($file =~ /LF/) {
      push @LFfiles, $file;
   } elsif ($file =~ /ME/) {
      push @MEfiles, $file;
   } elsif ($file =~ /NN/) {
      push @NNfiles, $file;
   } else {
      die "huh?: $file\n";
   }
}

# We eventually want to add the alpgen sample type as well (NN doesn't
# have it yet).  For now, this is the best we can do.
my $equalString = "abs(1_leppt-2_leppt)<0.02"; # && 1_njets==2_njets";#.
#  "&& 1_ntags==2_ntags";

# LF doesn't do tags correctly yet, so:
$equalString = "abs(1_leppt-2_leppt)<0.02";# && 1_njets==2_njets";
GenObject::setKey (kParticle, 
                   "%d  %9d  %2d  %d  %3d  %d".
                   "  %9.6e  %9.6e  %6.2f  ".
                   "%.4f  %.4f  %7.4f", 
                   "_run", "_event", "_det", "_ntags", "_alp", "_njets", 
                   "_weight", "_misweight", "_leppt",
                   "_lf", "_me", "_nn");
my @lfFormat = ( "%d  %9d  det: %2d  tags: %d  jets:%d".
                 "  weight: %9.6e  leppt: %6.2f  lf: %7.4f", 
                 "_run", "_event", "_det", "_ntags", "_njets", 
                 "_totweight", "_leppt", "_lf");
my @meFormat = ( "%d  %9d  det: %2d  tags: %d  alp: %d  njets: %d".
                 "  weight: %9.6e  leppt: %6.2f  me: %7.4f", 
                 "_run", "_event", "_det", "_ntags", "_alp", "_njets", 
                 "_weight * _misweight", "_leppt", "_me");
my @nnFormat = ( "%d  %9d  tags: %d  jets: %d  leppt: %6.2f nn:%7.4f",  
                 "_run", "_event", "_ntags", "_njets", "_leppt", "_nn");

print "\n\n\n\n";
print "output: $output\n";
print "equal:  $equalString\n";
print "LF:     @LFfiles\n";
print "ME:     @MEfiles\n";
print "NN:     @NNfiles\n";

exit if ($only);

# run evt lep ntag alp njet weight misweight lepPt LF ME NN
GenObject::setEqualString (kParticle, $equalString);

# open (PROBLEMS, ">", $problem) or die "Can't open problem file '$problem'\n";

my %particlesHoHoA;

###################
## Read ME files ##
###################

# ME's format
my $meLoaded = 0;
GenObject::setInit (kParticle, 
                    "run event det njets ntags alp ".
                    "weight misweight leppt me");
foreach my $file (@MEfiles) {
   open (ME, "<", $file) or die "Can't open ME file $file\n";
   while (<ME>) {
      chomp;
      s/#.+//;  # get rid of comments
      s/\s+$//; # trailing spaces
      s/^\s+//; # leading spaces
      my @parts = split /\s+/, $_;
      next unless (@parts);
      if (20 != @parts) {
         warn "$_ doesn't make sense";
         next;
      }
      my @numbers;
      while (@parts) {
         shift @parts;
         push @numbers, shift @parts;
      }
      $numbers[1] %= 1000;
      $numbers[1] *= 1000;
      $numbers[1] = 0;
      my $object = GenObject->new (kParticle . " | @numbers");
      push @{$particlesHoHoA{$object->run}->{$object->event}}, $object;
      ++$meLoaded;
   } # while ME
}

print "ME loaded:    $meLoaded\n";

###################
## Read NN files ##
###################
my $nnRematched = 0;
my ($nnLoaded, $nnUsed);
foreach my $file (@NNfiles) {
   open (NN, "<", $file) or die "Can't open NN file $file\n";
   my ($jetbin, $tagbin);
   if ($file =~ /_(\d)jet(\d)tag/) {
      $jetbin = $1;
      $tagbin = $2;
   } else {
      die $file;
   }

   # NN's format
   GenObject::setInit (kParticle, 
                       "run event peter fred leppt njets ntags same one nn");
   while (<NN>) {
      s/#.+//;  # get rid of comments
      s/\s+$//; # trailing spaces
      s/^\s+//; # leading spaces
      my @parts = split /\s+/, $_;
      next unless (@parts);
      #push @parts, $jetbin, $tagbin;
      $parts[1] %= 1000;
      $parts[1] *= 1000;
      $parts[1] = 0; # what I think of Tom's programming prowess
      my $object = GenObject->new (kParticle . " | @parts");
      ++$nnLoaded;
      my $aref = $particlesHoHoA{$object->run}->{$object->event} || 0;
      if (! $aref) {
         # print PROBLEMS "NN: Didn't find $object\n";
         next;
      }
      my $found = 0;
      foreach my $me (@$aref) {
         if ($me->isTheSameAs ($object)) {
            if ($me->has("nn")) {
               ++$nnRematched;
               if ($warn) {
                  warn "NN: Already matched:\n   ME: ", $me->key(@nnFormat),
                    "\n   NN: ", $object->key(@nnFormat),"\n";
               } # if warn
            } # if overlap
            $found = 1;
            $me->merge ($object);
            last;
         } # if the same
      } # foreach
      if (! $found) {
         #print PROBLEMS "NN: Found key but didn't find $object\n";
         next;
      }
      ++$nnUsed;
   } # while NN
} # foreach $file

printf("NN loaded:    $nnLoaded\nNN used:      $nnUsed (%.1f%%)\n", 
       (100. * $nnUsed / $nnLoaded));
print "NN rematched: $nnRematched\n";

###################
## Read LF files ##
###################

# LF's format
GenObject::setInit (kParticle, 
                    "run event det njets ntags totweight leppt lf");
my $lfRematched = 0;
my ($lfLoaded, $lfUsed);
foreach my $file (@LFfiles) {
   my ($jetbin);
   if ($file =~ /_(\d)j/) {
      $jetbin = $1;
   } else {
      die $file;
   }
   open (LF, "<", $file) or die "Can't open LF file $file\n";
   while (<LF>) {
      s/#.+//;  # get rid of comments
      s/\s+$//; # trailing spaces
      s/^\s+//; # leading spaces
      my @parts = split /\s+/, $_;
      next unless (@parts >= 8);
      # make things ints
      for my $loop (0..5) {
         $parts[$loop] = int ($parts[$loop] + 0.01);
      }
      $parts[1] %= 1000;
      $parts[1] *= 1000;
      $parts[1] = 0; # What I think of Tom's Fortran prowess
      $parts[3] = $jetbin;
      my $object = GenObject->new (kParticle . " | @parts");
      ++$lfLoaded;
      my $aref = $particlesHoHoA{$object->run}->{$object->event} || 0;
      if (! $aref && $warn) {
         print  "LF: Didn't find $object\n";
         next;
      }
      my $found = 0;
      foreach my $me (@$aref) {
         if ($me->isTheSameAs ($object)) {
            if ($me->has("lf")) {
               ++$lfRematched;
               if ($warn) {
                  warn "LF: Already matched:\n   ME: ", $me->key(@lfFormat),
                    "\n   LF: ", $object->key(@lfFormat),"\n";
               } # if warn
            } # if overlap
            $found = 1;
            $me->merge ($object);
            last;
         } # if the same
      } # foreach
      if (! $found && $warn) {
         print  "LF: Found key but didn't find $object\n";
         next;
      }
      ++$lfUsed;
   } # while LF
} # foreach file

printf("LF loaded:    $lfLoaded\nLF used:      $lfUsed (%.1f%%)\n", 
       (100. * $lfUsed / $lfLoaded));
print "LF rematched: $lfRematched\n";


open (OUTPUT, ">", $output) or die "Can't open output file '$output'\n";
# ME's format
GenObject::setInit (kParticle, 
                    "run event det njets ntags alp ".
                    "weight misweight leppt me");
my $count = 0;
my $printed = 0;
foreach my $run (sort keys %particlesHoHoA) {
   my $href = $particlesHoHoA{$run};
   foreach my $event (sort keys %$href) {
      my $aref = $href->{$event};
      ## if (0 == ++$count % 100) {
      ##    print ".";
      ## }
      foreach my $object (@$aref) {
         if (! $object->has("nn")) {
            # print PROBLEMS "ME: Didn't find NN $object\n";
            next;
         } # if no NN
         if (! $object->has("lf")) {
            # print PROBLEMS "ME: Didn't find LF $object\n";
            next;
         } # if no LF
         print OUTPUT $object->key, "\n";
         ++$printed;
      } # foreach object
   } # foreach event
} # foreach run

printf("Printed:      $printed (%.1f%%)\n", 
       (100. * $printed / $meLoaded));
