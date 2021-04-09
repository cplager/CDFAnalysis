package Candidate;

use strict;
use warnings;

my $_noEntryMode = "";
my @_allnames = qw (entry run event lep1_ID lep1_name lep2_ID lep2_name Zmass);
my @_names = qw (lep1_ID lep1_name lep2_ID lep2_name Zmass);
my @_equiv = (); #qw (lep1_ID lep2_ID);
my @_swap;
#push @_swap, ["lep1_", "lep2_"];

my $_numNames = @_names;
my %accuracy;
$accuracy{'Zmass'} = 0.01;

use overload 
    (
     ## '<=>' => \&compare,
     ## 'cmp' => \&compare,
     ## '+'   => \&add,
     ## '-'   => \&subtract,
     '""'  => \&stringify,
     );

sub new {
   my $class = shift;
   my $self = {};

   bless ($self, $class);

   my $line = shift;
   $self->_init($line);
   return $self;
}

sub _init {
   my $self = shift;
   my $line = shift;
   if (ref $line eq "Candidate") {
      # do a copy constructor
	  foreach my $name (@_names) {
		 $self->{$name} = $line->{$name};
	  }
	  $self->{'entry'} = $line->{'entry'};
	  $self->{'run'} = $line->{'run'};
	  $self->{'event'} = $line->{'event'};
   } else {
      # initialize from string
	  $line =~ s/\#.+$//; # get rid of comments
      $line =~ s/^\s+//; # rid of leading spaces
      $self->{'entry'} = 1;
	  my @parts = split /\s+/, $line;
	  if (@parts < 2) {
		 # we don't have enough
		 return;
	  }
      my $loop = 0;
      foreach my $name (@_allnames) {
         #print "seeting $name to $parts[$loop]\n";
         $self->{$name} = $parts[$loop++];
      } # foreach name
      $self->{'entry'} = 1 if ($_noEntryMode);
   } # else if
} # sub init

sub isOK {
   my $self = shift;
   if ($_noEntryMode) {
      if (!$self->run()   || ($self->run   !~ /^\d+$/) ||
          !$self->event() || ($self->event !~ /^\d+$/) ) {
         printf("Problem: %d, %d : %s\n", $self->run, 
                $self->event, $self);
         return;
      }
   } else {
      if (!$self->entry() || ($self->entry !~ /^\d+$/) ||
          !$self->run()   || ($self->run   !~ /^\d+$/) ||
          !$self->event() || ($self->event !~ /^\d+$/) ) {
         printf("Problem: %d : %d, %d : %s\n", $self->entry, $self->run, 
                $self->event, $self);
         return;
      }
   }
   return "true";
}

sub swapParts {
   my $self = shift;
   my $first = shift;
   my $second = shift;
   if ('ARRAY' eq ref $first) {
	  $second = $first->[1];
	  $first  = $first->[0];
   }
   foreach my $name (@_names) {
	  if ($name =~ /$first/) {
		 # we got one
		 (my $other = $name) =~ s/$first/$second/;
		 my $temp = $self->{$other};
		 $self->{$other} = $self->{$name};
		 $self->{$name}  = $temp;
	  } # if name matches
   } # foreach name
}

sub entry {
   my $self = shift;
   return $self->{'entry'};
}

sub run {
   my $self = shift;
   return $self->{'run'};
}

sub event {
   my $self = shift;
   return $self->{'event'};
}

sub key {
   my $self = shift;
   if ($_noEntryMode) {
      return sprintf ("%6d, %9d", 
                      $self->run, $self->event);
   } else {
      return sprintf ("%7d:  %6d, %9d", 
                      $self->entry, $self->run, $self->event);
   }
}

sub shortKey {
   my $self = shift;
   my $retval = $self->key.": ";
   foreach my $equiv (@_equiv) {
	  $retval .= sprintf (" %3s",$self->{$equiv});
   } # foreach equiv
   return $retval;
}

##  sub set {
##     my $self = shift;
##     my $attr = shift || undef;
##     my $value = shift || undef;
##     if (! defined $attr) {
##  	  warn "No attr set in 'set'";
##  	  return;
##     }
##     $attr = "attr_$attr";
##     if (! defined $value) {
##  	  delete $self->{$attr};
##  	  return;
##     }
##     $self->{$attr} = $value;
##  }
##  
##  sub AUTOLOAD {
##     use vars qw($AUTOLOAD);
##     my $self = shift;
##     my $attr = $AUTOLOAD;
##     $attr =~ s/.*:://;
##     return unless $attr =~ /[^A-Z]/;  # skip DESTROY and all-cap methods
##     #print "Autoload called with $attr : @_\n";
##     my $full = "attr_$attr";
##     if ($self->{$full}) {
##  	  return $self->{$full};
##     }
##     die "Didn't find variable '$attr'\n";
##     return;
##  } 

sub evaluate {
   my $self = shift;
   my $line = shift;
   my $retval;
   $line =~ s|\b_|\$self\->|g;
   print "evaluating: $line\n";
   
   $line = "\$retval = $line";
   eval $line;
   if ($@) {
   	  print "didn't work: $@\n";
   } else {
   	  print "retval $retval\n";
   }
}

sub stringify {
   my $self = shift;
   my $retval;
   if ($_noEntryMode) {
      $retval = sprintf("%6d  %8d -", $self->run, $self->event);
   } else {
      $retval = sprintf("%7d : %6d  %8d -", 
                        $self->entry, $self->run, $self->event);
   }
   foreach my $name (@_names) {
	  #print "$name : $self->{$name}\n";
	  #$retval .= "  $name: ".$self->{$name};
	  $retval .= "  ".$self->{$name};
   }
   return $retval;
}

sub compare {
   my $self = shift;
   my $other = shift;
   # loop over all variables
   my $differences;
   foreach my $name (@_names) {
	  my $acc = $accuracy{$name};
	  if ($acc) {
		 # here, we need to do a numerical comparison
		 my $val = $self->{$name} - $other->{$name};
		 if (abs ($val) > $acc) {
			# out of tolarance
			# make $val pretty
			$val /= $acc;
			$val = int $val;
			$val *= $acc;
			$differences .= "$name ($val: " . $self->{$name} . 
              " <=> " . $other->{$name} . ")  ";
		 } # if out of tolarance
	  } else {
		 if ($self->{$name} ne $other->{$name}) {
			# no joy
			$differences .= "$name (" . $self->{$name} . ", " . 
              $other->{$name} . ")  ";
		 }
	  } # else acc
   }
   # if we're still here, we're good to go
   return $differences;
}

sub printARef {
   my $aref = shift;
   my $count = 0;
   foreach my $item (@$aref) {
	  print "  ",++$count, $item, "\n";
   } # foreach $item
}

# returns true if the keys as well as the lepN_IDs are the same
sub areEquivalent {
   my $self = shift;
   my $otherOrig = shift;
   # make a copy so I can mess nilly-willy with the 'other' object
   my $other = Candidate->new($otherOrig);
   # check the keys
   if ($self->key ne $other->key) {
	  # don't bother continuing
	  return;
   }
   # o.k.  how about lepton IDs
   my $matched = "true";
   foreach my $equiv (@_equiv) {
	  if ($self->{$equiv} ne $other->{$equiv}) {
		 # not this one
		 $matched = "";
		 last;
	  } # if equiv
   } # foreach $equiv
   # if we're matched, return true
   return "true" if ($matched);

   # if we're still here, we should swap parts and try again
   # I should make this prettier
   return $matched if (! @_swap);
   $other->swapParts($_swap[0]);
   $matched = "true";
   foreach my $equiv (@_equiv) {
	  if ($self->{$equiv} ne $other->{$equiv}) {
		 # not this one
		 $matched = "";
		 last;
	  } # if equiv
   } # foreach $equiv
   return $matched;
}

# checks to see which elements are equivalent
sub checkEquivalents {
   my $oneRef = shift || {};
   my $twoRef = shift || {};
   my (%oneHash, %twoHash);
   # loop over first array
   foreach my $oneItem (@$oneRef) {
	  foreach my $twoItem (@$twoRef) {
		 if (areEquivalent ($oneItem, $twoItem) ) {
			# mark them
			$oneHash {$oneItem} = $twoItem;
			$twoHash {$twoItem} = $oneItem;
			last;
		 }
	  } # foreach twoItem
   } # foreach oneItem
   my ($firstOnly, $secondOnly, $bad);
   my $numGood = 0;
   # start by looping over the first list
   foreach my $oneItem (@$oneRef) {
	  my $twoItem = $oneHash{$oneItem};
	  if (! $twoItem) {
		 # no match, dude
		 $firstOnly .= "$oneItem\n";
		 next;
	  } 
	  my $diff = areDifferent ($oneItem, $twoItem);
	  if (! $diff) {
		 ++$numGood;
		 next;
	  }
	  # o.k., so they don't agree:
	  $bad .= $oneItem->shortKey." : ".$diff."\n";
   } # foreach oneitem
   # now lets loop over the second list to pick up the secondOnlys
   foreach my $twoItem (@$twoRef) {
	  my $oneItem = $twoHash{$twoItem};
	  if (! $oneItem) {
		 # no match, dude
		 $secondOnly .= "$twoItem\n";
	  } 
   }   
   return  ($firstOnly, $secondOnly, $numGood, $bad);
}

sub areDifferent {
   my $self = shift;
   my $otherOrig = shift;
   # make a copy so I can mess nilly-willy with the 'other' object
   my $other = Candidate->new($otherOrig);
   # first check the entry, run, and event numbers
   if ( ($self->entry != $other->entry) ||
	    ($self->run   != $other->run  ) ||
	    ($self->event != $other->event) ) {
	  return "Different entry, run, or event numbers";
   }
   my $comp1 = compare($self, $other);;
   if ( ! $comp1 ) {
	  return;
   } else {
	  # I should make this prettier
      return $comp1 if (! @_swap);
	  $other->swapParts($_swap[0]);
	  # check again
	  my $comp2 = compare($self, $other); 
	  if (! $comp2) {
		 return;
	  }
	  my @parts1 = split (/\s+/, $comp1);
	  my @parts2 = split (/\s+/, $comp2);
	  if (@parts1 <= @parts2) {
		 return $comp1;
	  } else {
		 return $comp2;
	  }
   }
}

sub setAccuracy {
   my $name = shift;
   my $value = shift;
   $accuracy{$name} = $value;
}

sub setEquivalents {
   (my $line = shift) =~ s/\s+$//;
   @_equiv = split (/\s+/, $line);
}

sub addSwap {
   my $first = shift;
   my $second = shift;
   push @_swap, [$first, $second];
}

sub setNames {
   my $line = shift;
   $line =~ s/##--//;
   $line =~ s/^\s*//;
   $line =~ s/\s*$//;
   @_allnames = split /\s+/, $line;
   @_names = ();
   foreach my $name (@_allnames) {
      if ($name ne "entry" && $name ne "run" and $name ne "event") {
         push @_names, $name;
      }
   }
   $_numNames = @_names;
}

sub setNoEntryMode {
   $_noEntryMode = shift || "true";
}

1;
