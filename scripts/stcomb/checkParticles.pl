#!/usr/bin/env perl

use warnings;
use strict;

my $version = "checkParticles.pl v1.13 cplager 060421";

# Load GenObject.pm.  Assume it is either in the current directory or
# the same directory as the script.
(my $dir = $0) =~ s|[^/]+$||;
my $line = sprintf ("use lib '%s'; use GenObject;", $dir);
my $retval = eval $line;
die "Unable to load GenObject:\n$@\n" if ($@);

my @particles;
my @ignore;
my @ignore_unlessAoA;
my %firstHoHoA  = ();
my %secondHoHoA = ();
my %metaCounter = ();
my %dropifHoA   = ();
my $key;
my $href;
my ($debug, $useEntry, $notUnique, $nomissing);
my ($minrun, $maxrun) = (0,0);
(my $thisscript = $0) =~ s|.+/||g;


while ($ARGV[0] && $ARGV[0] =~ /^-+/) {
   my $arg = shift @ARGV;
   $arg =~ s/^-+//;
   # help
   if ($arg =~ /^h/i) {
      help();
   } 
   # debug
   if ($arg =~ /^de/i) {
      $debug = "true";
      next;
   }      
   # ignore
   if ($arg =~ /^ig/i) {
      my $value = shift;
      push @ignore, $value;
      next;
   }       
   # unless
   if ($arg =~ /^unless/i) {
      my $ig  = shift;
      my $unl = shift;
      # push an anonymous array of 2 elements on the @ignore_unlessAoA list
      push @ignore_unlessAoA,  [$ig, $unl];
      next;
   }        
   # dropif
   if ($arg =~ /^drop/i) {
      my $name = shift;
      my $bool = shift;
      next unless $bool;
      push @{$dropifHoA{$name}}, $bool;
      next;
   }      
   # entry
   if ($arg =~ /^e/i) {
      print "Using 'entry' in key.\n";
      $useEntry = "true";
      next;
   } 
   # minrun
   if ($arg =~ /^min/i) {
      $minrun = shift;
      print "min $minrun\n";
      next;
   }       
   # maxrun
   if ($arg =~ /^max/i) {
      $maxrun = shift;
      next;
   }       
   # not unique
   if ($arg =~ /^not/i) {
      $notUnique = "true";
      next;
   }       
   # no missing
   if ($arg =~ /^nom/i) {
      $nomissing = "true";
      next;
   }       
   # nodie
   if ($arg =~ /^nod/i) {
      no warnings;
      $GenObject::dieOnError = "";
      next;
   }       
   # if we're still here, I don't understand the argument.  Fess up now
   die "I don't understand '$arg'.  Aborting.\n";
}

help() unless (@ARGV >= 2);

my $first = shift;
my $second = shift;



print "Reading $first file\n";
loadFromFile ($first,  \%firstHoHoA);
print "Reading $second file\n";
loadFromFile ($second, \%secondHoHoA);
print "Files loaded.\n";

my @firstOnly;
my @both;
foreach my $key (sort keys %firstHoHoA) {
   if ($secondHoHoA{$key}) {
      push @both, $key
   } else {
      # push this on the 'unique' firstOnly list unless 'notUnique' is
      # set
      push @firstOnly, $key unless $notUnique;
   }
} # foreach key

# Don't bother creating the 'unique' secondOnly list if 'notUnique' is
# set
my @secondOnly;
if (! $notUnique) {
   foreach my $key (sort keys %secondHoHoA) {
      if (!$firstHoHoA{$key}) {
         push @secondOnly, $key
      }
   } # foreach key
} # if ! $notUnique;

my %probCounterHoH;
my %particleCounterHash;
my %missingParticleCounterHash;
foreach my $particle (@particles) {
   print "\n\n ## $particle ##\n";
   $particleCounterHash{$particle} = 0;
   $missingParticleCounterHash{$particle} = 0;
   #print "particle $particle\n";
   my $matched_problems;
   my $oneNotTwo_problems = "";
   my $twoNotOne_problems = "";
   foreach my $key (@both) {
      my $firstAref = $firstHoHoA{$key}->{$particle};
      my $secondAref = $secondHoHoA{$key}->{$particle};
      # O.k.  if neither reference exists, then go on to the next event
      if (! $firstAref && ! $secondAref) {
         # no objects on either side
         next;
      }           
      # Now, if one or the other array reference doesn't exist, we
      # need to be clever.
      my (@temp1, @temp2, @unmatched1, $key_prob_text);
      if ($firstAref && $secondAref) {
         # This is what usually happens
         # copy the arrays so we can mess with them
         @temp1 = @$firstAref;
         @temp2 = @$secondAref;         
      } elsif (! $firstAref) {
         # copy all of them to @temp2 which will become @unmatched2
         @temp1 = ();
         @temp2 = @$secondAref;
      } elsif (! $secondAref) {
         @temp1 = ();
         @temp2 = ();
         @unmatched1 = @$firstAref;
      }
      my (@match1, @match2);
      # loop over the first array
      for my $loop1 (0 .. @temp1 - 1) {
         my $matched = "";
         my $obj1 = $temp1[$loop1];
         for my $loop2 (0 .. @temp2 - 1) {
            my $obj2 = $temp2[$loop2];
            if ($obj1->isTheSameAs($obj2)) {
               # We've got a match.  Push these onto the match arrays
               # and remove the second one from the list
               push @match1, $obj1;
               push @match2, $obj2;
               splice @temp2, $loop2, 1;
               #print "$obj1 and\n$obj2 are the same\n\n";
               $matched = "true";
               last;
            } # if the same
         } # for loop2
         # If we didn't match this object, we want to know about it
         push @unmatched1, $obj1 unless ($matched);
      } # for loop1
      # Anything that is still in @temp2 is unmatched.
      my @unmatched2 = @temp2;

      #########################################
      ## Differences Between Matched Objects ##
      #########################################
      $particleCounterHash{$particle} += scalar @match1;
      my $upper = @match1 - 1;
      for my $loop (0..$upper) {
         my @problems = $match1[$loop]->compareWith($match2[$loop]);
         next unless @problems;
         # put it on the global list
         $key_prob_text .= "$match1[$loop] || @problems\n";
         # keep track of what type of problem it is
         foreach my $problem (@problems) {
            (my $variable = $problem) =~ s/\s.+//;
            ++$probCounterHoH{$particle}->{$variable};
         } # foreach $problem
      } # for loop
      if ($key_prob_text) {
         $matched_problems .= "$key\n$key_prob_text\n";
      }

      if (! $nomissing) {
         #####################
         ## Missing Objects ##
         #####################
         $particleCounterHash{$particle} += scalar @unmatched1;
         $particleCounterHash{$particle} += scalar @unmatched2;
         $missingParticleCounterHash{$particle} += scalar @unmatched1;
         $missingParticleCounterHash{$particle} += scalar @unmatched2;
         
         if (@unmatched1) {
            $oneNotTwo_problems .= "$key\n";
            foreach my $obj (@unmatched1) {
               $oneNotTwo_problems .= $obj."\n";
            }
         } # if unmatched1
         
         if (@unmatched2) {
            $twoNotOne_problems .= "$key\n";
            foreach my $obj (@unmatched2) {
               $twoNotOne_problems .= $obj."\n";
            }
         } # if unmatched2      
      } # # no missing
   } # foreach key

   if (! $nomissing) {
      ####################
      ## Missing Events ##
      ####################
      foreach $key (@firstOnly) {
         my $aref = $firstHoHoA{$key}->{$particle};
         next unless $aref;
         $particleCounterHash{$particle} +=  scalar @$aref;
         $missingParticleCounterHash{$particle} += scalar @$aref;
         $oneNotTwo_problems .= "$key\n";
         foreach my $obj (@$aref) {
            $oneNotTwo_problems .= $obj."\n";
         }                      # foreach obj
      }

      foreach $key (@secondOnly) {
         my $aref = $secondHoHoA{$key}->{$particle};
         next unless $aref;
         $particleCounterHash{$particle} +=  scalar @$aref;
         $missingParticleCounterHash{$particle} += scalar @$aref;
         $twoNotOne_problems .= "$key\n";
         foreach my $obj (@$aref) {
            $twoNotOne_problems .= $obj."\n";
         }                      # foreach obj
      }
   } # no missing

   if ($oneNotTwo_problems || $twoNotOne_problems) {
      print "Missing $particle"."s:\n";
      if ($oneNotTwo_problems) {
         print "In $first, but not $second:\n$oneNotTwo_problems\n\n";
      }
      if ($twoNotOne_problems) {
         print "In $second, but not $first:\n$twoNotOne_problems\n\n";
      }
   }
   print "Variable Match Problems:\n$matched_problems\n" if $matched_problems;
} # foreach particle

print "Summary:\n";
foreach my $particle (@particles) {
   print "Number of $particle"."s: $particleCounterHash{$particle}\n";
   my $href = $probCounterHoH{$particle};
   my $totMisProb = 0;
   my $sumProb = "";
   my @vars = GenObject::listOfCompareVariables($particle);
   foreach my $key (@vars) {
      my $value = $href->{$key};
      next unless $value;
      $totMisProb += $value;
      $sumProb .= "    - '$key' Mismatches: $value\n";
   } # foreach key
   my $totProb = $totMisProb + $missingParticleCounterHash{$particle};
   print "  Total Problems: $totProb\n    Total Missing: $missingParticleCounterHash{$particle}\n";
   print "    Total Mismatches: $totMisProb\n$sumProb\n";
}

if ($debug) {
   foreach my $key (sort keys %metaCounter) {
      my $half = $metaCounter{$key} / 2;
      print "$key $metaCounter{$key} $half\n";
   }
}


#############################
## ####################### ##
## ## ################# ## ##
## ## ## Subroutines ## ## ##
## ## ################# ## ##
## ####################### ##
#############################

sub loadFromFile {
   my $filename = shift;
   my $hohoaRef = shift;
   my $key;
   my $href = {};
   open (FILE, $filename) or die "Can't open $filename.  Aborting.\n";
   LINE: while (<FILE>) {      
      foreach my $value (@ignore) {
         next LINE if (/$value/);
      }
      foreach my $aref (@ignore_unlessAoA) {
         my $ig  = $aref->[0];
         my $unl = $aref->[1];
         next LINE if (/$ig/ && ! /$unl/);
      }
      if (/#-\s+particles\s*:\s*(.+)/) {
         (my $partString = $1) =~ s/[\s\;]+$//;
         my @newparticles = split /\s+/, $partString;
         # add any new particles to the list
         foreach my $newpart (@newparticles) {
            my $found = "";
            foreach my $particle (@particles) {
               if ($particle eq $newpart) {
                  $found = "true";
                  last;
               } # if matched
            } # foreach
            if (! $found) {
               push @particles, $newpart;
            }
         } # foreach newparticle
         next;
      }
      if (/#-\s+init\s+(\w+)\s*:\s*(.+)/) {
         GenObject::setInit ($1, $2);
         next;
      }
      if (/#-\s+equal\s+(\w+)\s*:\s*(.+)/) {
         GenObject::setEqualString ($1, $2);
         next;
      }
      if (/#-\s+comp\s+(\w+)\s*:\s*(.+)/) {
         GenObject::setCompareVariables ($1, $2);
         next;
      }
      if (/#-\s+prec\s+(\w+)\s*:\s*(.+)/) {
         GenObject::setPrecisionFromString ($1, $2);
         next;
      }
      # remove all other comments
      s/#.+//g;
      # don't bother with empty lines
      next unless /\S/;
      if (/run\s+(\d+)\s+event\s+(\d+)/) {
         my $run = $1;
         my $event = $2;
         my $entry = -1;
         # Is this run below our minimum?
         if ($minrun && ($minrun > $run)) {
            $key = "";
            next LINE;
         }
         # Is this run below our minimum?
         if ($maxrun && ($maxrun < $run)) {
            $key = "";
            next LINE;
         }
         if (/entry\s+(\d+)/) {
            $entry = $1;
         }
         # first finish of what we already have
         if ($useEntry) {
            $key = sprintf ("%6d %9d %8d", $run, $event, $entry);
         } else {
            $key = sprintf ("%6d %9d", $run, $event);
         }
         next;
      }
      next unless $key;
      if (/(\w+)\s*:\s*(.+)/) {
         my $type = $1;
         my $rest = $2;
         my @inits = split /\;/, $rest;
         $metaCounter{$type} += scalar @inits;
         ## Here's the deal:
         ##
         ## IF running over multiple files that have the SAME run and
         ## event numbers, THEN clearing out the array (like below)
         ## will lose information.  IF I DON'T clear out the array,
         ## then I will add the objects on top of the old event.
         ##
         ## If you are running on MC that has duplicate run and event
         ## numbers, but are in a consistent order, you can use the
         ## '-entry' flag to use entry in the key and avoid this problem.
         ## e.g., run 211111 event 123 entry 23434
         $hohoaRef->{$key}->{$type} = []; # inits and clears array
         my $dropifAref = $dropifHoA{$type};
         foreach my $init (@inits) {
            my $object = GenObject->new("$type | $init");
            next unless $object;
            # do we want to drop this object
            my $keepParticle = "true";
            foreach my $bool (@$dropifAref) {
               if ($object->evaluate($bool)) {
                  print "'$bool' is true\n";
                  $keepParticle = "";
                  last;
               } # if bool is true
            }
            push @{$hohoaRef->{$key}->{$type}}, $object if ($keepParticle);
         } # foreach init
      } # if particle
   } # while first

   if ($debug) {
      ## For debugging
      print "particles @particles\n";
      print "keys ", scalar keys %$hohoaRef,"\n";
      foreach my $particle (@particles) {
         my $num = 0;
         foreach my $key (keys %$hohoaRef) {
            my $aref = $hohoaRef->{$key}->{$particle};
            if ($aref) {
               $num += scalar @$aref;
            }
         } # foreach key
         print "count $particle: $num\n";
         print "meta  $metaCounter{$particle}\n";
      } # foreach particle
   } # if debug
   close FILE;
}

sub help {
   no warnings;
   print "Usage: $thisscript [-args] file1 file2
--debug        -> Print out debug information
--dropif x \"y\" -> Drop particle 'x' if 'y' is true 
                  (e.g. -dropif electron \"_et < 20\" drops all electrons
                  with 'et' less than 20; can be used multiple times) 
--entry        -> Use 'entry' in addition to 'run' and 'event' in the key
--help         -> This screen
--ignore XX    -> Ignore lines containing 'XX' (can be used multiple times)
--maxrun XX    -> Ignore runs above 'XX'
--minrun XX    -> Ignore runs below 'XX'
--nodie        -> Will keep running if there are errors parcing the text files
                  (Not recommended - Use at your own risk.)
--nomissing    -> Will not print information about missing particles
--notunique    -> Ignores events that do not show up in both files
--unless XX YY -> Ignore lines containing 'XX' UNLESS it also contains 'YY'
                  (can be used multiple times)
For more information, visit the script's homepage:
http://www-cdf.fnal.gov/~cplager/internal/Analysis/Tools/ParticleByParticle/particle.html
$GenObject::version\n$version\n\n";
   exit;
}
