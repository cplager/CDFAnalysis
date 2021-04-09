package ErrorItem;

use strict;
use warnings;

# whether to die or not when parcing errors are found
our $dieOnError = "true";
our $version    = "ErrorItem     v1.01 cplager 060605";
# line number of current file
my $_lineNumber = 0;
my $_shortString = "true";
my $_globalRequire = "";
my @errorItemList;
my %errorItemHash;

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

   my $status = $self->_init(@_);
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
   if (@_ < 2) {
      warn "ErrorItem Initialization Error: Must have at least two items\n";
      return;
   }
   $self->{level} = shift @_;
   # set using global require if needed
   if ($_globalRequire) {
      $self->{requires} = $_globalRequire;
   }
   # named quantities
   while (@_ && $_[0] =~ /^::(\w+)=(\w+)::$/) {
      $self->{$1} = $2;
      shift @_;
   }
   my $num = $self->{num} = scalar @_;
   if (! $num) {
      warn "ErrorItem Initialization Error: Must have at least one match line\n";
      return;
   }
   $self->{where} = 0;
   $self->{linenum}  = 0;
   foreach my $line (@_) {
      chomp (my $string = $line);
      my $limit = 0;
      my @notlist = ();
      if ($string =~ m|::!!(.+)!!::|) {
         $string = $`;
         @notlist = split /,,,/, $1;
      }
      if ($string =~ m|::(\d+)::$|) {
         $limit = $1;
         $string = $`;
      }
      # print "string '$string'\n";
      push @{$self->{aref}}, $string;
      if (@notlist) {
         my $num = scalar @{$self->{aref}} - 1;
         push @{$self->{notArefAoA}->[$num]}, @notlist;
      }
      push @{$self->{limAref}}, $limit;
   }
   # put this on the hash if it is named
   if ($self->{name}) {
      $errorItemHash{ $self->{name} } = $self;
   }
   return "ok";
}

sub stringify {
   my $self = shift;
   my $retval;
   if ($self->isFound) {
      my $aref = $self->{foundAref};
      my $numfound = scalar @$aref;
      my $s = "s";
      $s = "" if (1 == $numfound);
      $retval .= $self->{level}." $numfound problem$s\n";
      my %countProblemHash;
      foreach my $string (@$aref) {
         ++$countProblemHash{$string};
      }
      foreach my $key (sort keys %countProblemHash) {
         $retval .= "$countProblemHash{$key} times: $key\n";         
      }
      return $retval;
   }
   if ($_shortString) {
      my $aref = $self->{aref};
      $retval .= $self->{level}." NOT found\n";      
      $retval .= join ("\n    ", @$aref);
      return $retval;
   }
   $retval = $self->{level}.", ";
   my $num = $self->{num};
   $retval .= "$num, ".$self->{where}.":\n";
   for my $loop (0..$num-1) {
      my $limit = $self->{limAref}->[$loop];
      my $line  = $self->{aref}->[$loop];
      $retval .= $line;
      if ($limit > 0) {
         $retval .= " :: $limit ::";
      }
      $retval .= "\n";
   } # for loop
   return $retval;
}

sub resetItem {
   my $self = shift;
   $self->{where} = 0;
   $self->{linenum} = 0;
   @{$self->{linesAref}} = ();
}

sub resetAllItem {
   my $self = shift;
   $self->resetItem();
   $self->{foundAref} = undef;
} 

sub where {
   my $self = shift;
   return $self->{where};
}

sub isFound {
   my $self = shift;
   return (defined $self->{foundAref});
}

sub checkLine {
   my $self = shift;
   my $line = shift;
   if (my $requires = $self->{requires}) {
      return unless conditionSatisfied ($requires);
   }
   # have we already gone past the limit
   my $where    = $self->{where};
   my $linenum  = $self->{linenum};
   my $limit    = $self->{limAref}->[$where];
   if ($limit && ($linenum + $limit < $_lineNumber)) {
      # over the limit
      $self->resetItem();
      return;
   } # if over the limit
   # does this line match?
   my $match = $self->{aref}->[$where];
   if ($line =~ /$match/) {
      # we've got a match
      # should we keep it?
      my $aref = $self->{notArefAoA}->[$self->{where}];
      if ($aref) {
         # we need to make sure we don't match these
         foreach my $not (@$aref) {
            if ($line =~ /$not/) {
               return;
            }
         }
      }
      $self->{linenum} = $_lineNumber;
      $self->{where} += 1;
      push @{$self->{linesAref}}, $line;
      if ($self->{num} == $self->{where}) {
         push @{$self->{foundAref}}, join ("\n", @{$self->{linesAref}});
         $self->resetItem();
      }
   } # if line matches
}

sub shouldWarn {
   my $self = shift;
   my $level = $self->{level};
   if ($level =~ /^err/i || $level =~ /^warn/i) {
      return "true";
   }
   return;
}

sub shouldNotWarn {
   my $self = shift;
   my $level = $self->{level};
   if ($level =~ /^anti/i) {
      return "true";
   }
   return;
}

######################
## Static Functions ##
######################

sub conditionSatisfied {
   my $string = shift;
   my ($name, $number);
   if ($string =~ /^(\w+)_(\d+)$/) {
      $name = $1;
      $number = $2;
   } else {
      # sorry
      return;
   }
   my $item = $errorItemHash{$name};
   if (! $item) {
      return;
   }
   return ($item->where == $number)
}

sub checkLogFile {
   my $logfile = shift;
   resetAllItems("everything");
   open SOURCE, $logfile or die "Can't open $logfile for input.  Aborting\n";
   while (<SOURCE>) {
      chomp;
      next unless /\S/; # don't check blank lines
      checkAllItemsForLine ($_);
   } # while SOURCE
   return returnAllFoundItems();
}

sub returnAllFoundItems {
   my @retval;
   foreach my $item (@errorItemList) {
      push (@retval, $item) if ($item->isFound && $item->shouldWarn);
      push (@retval, $item) if (! $item->isFound && $item->shouldNotWarn);
   } # foreach item
   return @retval;
}

sub checkAllItemsForLine {
   my $line = shift;
   ++$_lineNumber;
   foreach my $item (@errorItemList) {
      $item->checkLine($line);
   } # foreach item
}

sub resetAllItems {
   my $everything = shift;
   foreach my $item (@errorItemList) {
      if ($everything) {
         $item->resetAllItem();
      } else {
         $item->resetItem();
      }
   } # foreach item
}

sub readErrorItemListFromFile {
   my $filename = shift;
   if (! open (SOURCE, $filename)) {
      warn "Couldn't open $filename to read error conditions.  Aboring.\n";
      die "\n";
   } # if open failed
   my $level = "error";
   my @lines = ();
   while (<SOURCE>) {
      chomp;
      s/$//; # windows-unix issues
      # change variables with '#+ var1=value1'
      # require=, level=
      if (/^#\+\s*([\w]+)=(\S*)$/) {
         my $varname = $1;
         my $value = $2 || "";
         if ($varname =~ /require/i) {
            $_globalRequire = $value;
            next;
         }
         if ($varname =~ /level/i) {
            $level = $value;
            next;
         }
         next;
      }
      # comment line?
      if (/^#/) {
         next;
      }
      # is this a blank line?
      if (! /\S/) {
         if (@lines) {
            my $item = ErrorItem->new ($level, @lines);
            push @errorItemList, $item;
            @lines = ();
         } # if something is defined
         next;
      }
      # if we're still here then we should add it to lines
      #print "adding '$_':\n";
      push @lines, $_;
   } # while SOURCE
   # finish the last
   if (@lines) {
      my $item = ErrorItem->new ($level, @lines);
      push @errorItemList, $item;
   }
}

sub printErrorItemList {
   my @list = @_;
   if (! @list) {
      @list = @errorItemList;
   }
   #print "size of list ", scalar @list, "\n";
   my $count = 0;
   foreach my $item (@list) {
      printf "%2d) %s\n", ++$count, $item;
   }
}

1;
