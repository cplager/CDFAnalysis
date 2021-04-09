package GenObject;

use strict;
use warnings;
use POSIX qw(ceil floor);

our $version = "GenObject.pm      v1.2 cplager 071217";

# Rules for ruling two particles the same
my %_equivHash;
# variables names to initialize particles (hash of array)
my %_initHoA;
# equation to compare for equality (e.g. are these two electrons the same?)
my %_equalHash;
# variables used to check properties of the electrons
my %_compareHoA;
# precisions to compare particles (hash of hash)
my %_precisionHoH;
# information for 'key' for a particle
my %_keyHoA;
# whether to die or not when parcing errors are found
our $dieOnError = "true";

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

   my $line = "@_";
   my $status = $self->_init($line);
   if ($status) {
      return $self;
   }
   # if we're still here, things went badly
   if ($dieOnError) {
      die "\n";
   } else {
      warn "***Danger***\n";
   }
   return;
}

sub _init {
   my $self = shift;
   my $line = shift;

   # Is this a Standard or Alternative initialization?
   if ($line =~ m/\s*(\w+)\s*\|\s*(.+)/) {
      #############################
      ## Standard Initialization ##
      ## 'type | 1 2 3'          ##
      #############################
      # name | 1 2 3
      my $name = lc $1;
      my $rest = $2;
      $self->{name} = $self->{attr_name} = $name;
      # does this one have an init hash
      my $aref = $_initHoA{$name};
      if (! $aref) {
         warn "Don't understand type '$name'.  Aborting\n$line\n";
         return;
      }
      my $elements = @$aref;
      $rest =~ s/\s+$//;
      my @parts = split /\s+/, $rest;
      if (@parts != $elements) {
         my $num = @parts;
         warn "Particle Initialization Error:\n'$name' particles are supposed".
           " to have $elements elements.  You have provided $num.\n$line\n";
         return;
      }
      for my $loop (0..$elements-1) {
         my $tag = "attr_".$aref->[$loop];
         my $value = $parts[$loop];
         #print "$tag = $value\n";
         $self->{$tag} = $value;
      }
      return "ok";
   } # if standard initialization

   ################################
   ## Alternative Initialization ##
   ## 'name:type a:1; b:2; c:3'  ##
   ################################
   my @parts = split /;/, $line;
   my $num = @parts;
   foreach my $part (@parts) {
      $part =~ s/\s*$//;
      if ($part =~ /\s*(\w+)\s*:\s*(\S.*)\s*$/) {
    	 my $attr = "attr_$1";
    	 my $value = $2;
    	 $self->{$attr} = $value;
      } # if $part
   } # foreach part
   # Make sure this object has a name
   if (! $self->{attr_name}) {
      # no good
      return;
   } # if no name
   # 'name' variable is special
   $self->{name} = $self->{attr_name};
   return "ok";
} # sub init

sub set {
   my $self = shift;
   my $attr = shift || undef;
   my $value = shift || undef;
   if (! defined $attr) {
	  warn "No attr set in 'set'";
	  return;
   }
   $attr = "attr_$attr";
   if (! defined $value) {
	  delete $self->{$attr};
	  return;
   }
   $self->{$attr} = $value;
}

sub has {
   my $self = shift;
   my $attr = shift;
   return unless $attr;
   $attr = "attr_$attr";
   return defined $self->{$attr};
}

sub AUTOLOAD {
   use vars qw($AUTOLOAD);
   my $self = shift;
   my $attr = $AUTOLOAD;
   $attr =~ s/.*:://;
   return unless $attr =~ /[^A-Z]/;  # skip DESTROY and all-cap methods
   #print "Autoload called with $attr : @_\n";
   my $full = "attr_$attr";
   if (defined $self->{$full}) {
	  return $self->{$full};
   }
   die "Didn't find variable '$attr'\n";
   return;
} 

sub key {
   my $self = shift;
   my @formatArray = @_;
   if (! @formatArray) {
      # warn "name ", $self->name;
      my $aref = $_keyHoA{$self->name};
      return unless $aref;
      @formatArray = @$aref;
   }
   my $format = shift @formatArray;
   my @values;
   foreach my $part (@formatArray) {
      push @values, $self->evaluate($part);
   }
   my $retval = sprintf ($format, @values);
   return $retval;
}

sub evaluate {
   my $self = shift;
   my $line = shift;
   my $retval;
   $line =~ s|\b_|\$self\->|g;
   #print "evaluating: $line\n";
   
   $line = "\$retval = $line";
   eval $line;
   if ($@) {
   	  print "Evaluate: '$line' didn't work: $@\n";
      die if $dieOnError;
      return;
   }
   #print "retval $retval\n";
   return $retval;
}

sub stringify {
   my $self = shift;
   my $name = $self->name;
   my $aref = $_initHoA{$name};
   if (! $aref) {
      warn "No equiv reference for $name\n";
      return;
   }
   #return unless $aref;
   my $retval = "$name ";
   my $first = "true";
   foreach my $key (@$aref ) {
      if (! $first) {
         $retval .= ", ";
      } else {
         $first = "";
      }
	  $retval .= $key.": ".$self->$key;
   }
   return $retval;
}

sub compareWith {
   my $self = shift;
   my $other = shift;
   # are they the same type?
   if ($self->name ne $other->name) {
      # don't bother
      warn "comparing different types doesn't work\n";
      return;
   }
   my $name = $self->name;
   # do I have a compare string for these
   my $aref = $_compareHoA{$name};
   if (!$aref) {
      warn "comparing without seting up compareHash doesn't work\n";
      return;
   }
   my @problems = ();
   foreach my $variable (@$aref) {
      my $attr = "attr_$variable";
      my $thisVar = $self->{$attr};
      my $thatVar = $other->{$attr};
      if (! defined $thisVar || ! defined $thatVar) {
         warn "variable $variable not defined\n";
         next;
      }
      my $precision = $_precisionHoH{$name}->{$variable} || "";
      if ($precision) {
         my $diff = $thisVar - $thatVar;
         if (abs ($diff) > $precision) {
            $diff = roundToPrecision ($diff, $precision);
            push @problems, "$variable ($diff: $thisVar, $thatVar)";
         }
      } else {
         if ($thisVar ne $thatVar) {
            push @problems, "$variable ($thisVar, $thatVar)";
         }
      } # else if precision
   } # foreach aref
   return @problems;
}

sub roundToPrecision {
   my $value = shift;
   my $precision = shift;
   my $digits = -1 * floor(log($precision)/log(10));
   if ($digits <= 0) {
      return $value;
   }
   my $format = "\%.$digits"."f";
   return sprintf ($format, $value);
}

sub isTheSameAs {
   my $self = shift;
   my $other = shift;
   # are they the same type?
   if ($self->name ne $other->name) {
      # don't bother
      warn "comparing different types doesn't work\n";
      return;
   }
   my $name = $self->name;
   # do I have a compare string for these
   my $equalString = $_equalHash{$name};
   if (! $equalString) {
      warn "No compare string for $name\n";
      return;
   }
   my $retval;
   $equalString = "\$retval = $equalString";
   $equalString =~ s|1_|\$self->|g;
   $equalString =~ s|2_|\$other->|g;
   eval $equalString;
   if ($@) {
      print "didn't work: $@\n";
   }
   return $retval;
}

sub merge {
   my $self  = shift;
   my $other = shift;
   # We want to go through and add NEW variables only
   foreach my $key (keys %$other) {
      # is this already defined
      next if (defined $self->{$key});
      $self->{$key} = $other->{$key};
   }
}

sub setEquivalent {
   my $type  = lc shift;
   my $equiv = shift;
   if (! $equiv) {
      warn "GenObject::setEquivalent: You must provide 'type' and 'equiv'\n";
      die "\n" if $dieOnError;
      return;
   }
   $_equivHash{$type} = $equiv;
}

sub setEqualString {
   my $type  = shift;
   my $hash = shift;
   if (! $hash) {
      warn "GenObject::setEqualString: You must provide 'type' and 'compare'\n";
      die "\n" if $dieOnError;
      return;
   }
   $_equalHash{$type} = $hash;
}

sub setPrecisionFromString {
   my $type = shift;
   my $string = shift;
   $string =~ s/^\s+//;
   $string =~ s/\s+$//;
   my @parts = split /\s+/, $string;
   setPrecision($type, @parts);
}

sub setPrecision {
   my $type  = shift;
   my %hash = @_;
   if (! %hash) {
      warn "GenObject::setPrecision: You must provide 'type' and 'list'\n";
      die "\n" if $dieOnError;
      return;
   }
   my $href;
   foreach my $key (keys %hash) {
      $href->{$key} = $hash{$key};
   }
   $_precisionHoH{$type} = $href;
}

sub setCompareVariables {
   my $type  = shift;
   my @list = @_;
   if (! @list) {
      warn "GenObject::setCompareVariables: You must provide 'type' and 'list'\n";
      die "\n" if $dieOnError;
      return;
   }
   # I want to allow for either one string with space separated
   # variable names or for an array of names.
   my $aref;
   foreach my $line (@list) {
      # leading and trailing spaces
      $line =~ s/^\s+//;
      $line =~ s/\s+$//;
      my @parts = split /\s+/, $line;
      push @$aref, @parts;
   }
   $_compareHoA{$type} = $aref;
}

sub listOfCompareVariables {
   my $particle = shift;
   my $aref = $_compareHoA{$particle};
   if (! $aref) {
      warn "GenObject::listOfCompareVariables: No particle '$particle'.\n";
      die "\n" if $dieOnError;
      return;
   }
   return @$aref;
}

sub setInit {
   my $type  = shift;
   my @list = @_;
   if (! @list) {
      warn "GenObject::setInit: You must provide 'type' and 'list'\n";
      die "\n" if $dieOnError;
      return;
   }
   # I want to allow for either one string with space separated
   # variable names or for an array of names.
   my $aref;
   foreach my $line (@list) {
      # leading and trailing spaces
      $line =~ s/^\s+//;
      $line =~ s/\s+$//;
      my @parts = split /\s+/, $line;
      push @$aref, @parts;
   }
   $_initHoA{$type} = $aref;
   # While we're here, lets setup the precision hash if its not already there
   if (! $_precisionHoH{$type}) {
      $_precisionHoH{$type} = {};
   }
}

sub setKey {
   my $type = shift;
   my @elements = @_;
   $_keyHoA{$type} = [@elements];
}

1;
