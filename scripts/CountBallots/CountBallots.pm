package CountBallots;



# array of all votes
my @allVotes;
my %uniqueHash;

use overload 
    (
     ##  '<=>' => \&compare,
     ##  'cmp' => \&compare,
     '""'  => \&stringify,
     );

use strict;
use warnings;


sub new
{
    # Peel off class instance as first input argument
    my $class = shift;

    # Member data will be maintained in a sc-called "anonymous hash"
    my $self = {};
    # Connect this hash to the object
    bless($self, $class);
    
    # Pass rest of input arguments to separate init routine
    my $line = "@_";
    if (!$self->_init(@_)) {
        warn "CountBallots '$line' not valid\n";
        return;
    }

    return $self;
}

sub _init
{
    # Peel off class instance as first input argument
    my $self = shift;
    # votes are in the format of name:id:1st:2nd:3rd
    my $string = shift;
    my @words = split(':', $string);
    $self->{name} = shift @words;
    $self->{id} = shift @words;
    my @clean;
    foreach my $word (@words) {
        if (($word =~ /\S/) && ($word !~ /none/i)) {
            push @clean, $word;
            $uniqueHash{$word} = $word;
        }
    }
    @{$self->{array}} = @clean;    
    push @allVotes, $self;
    return 1;
}

sub countVotes {
    my @notList = @_;
    push @notList, "none";
    my %hash = ();
    foreach my $vote (@allVotes) {
        my $top = $vote->topVote(@notList);
        # don't count it as a vote if 'none' is the person
        ++$hash{$top} if ($top ne "none");
    }    
    my %retval;
    foreach my $key (keys %hash) {
        my $value = $hash{$key};
        push @{$retval{$value}}, $key;
    }
    return %retval;
}

sub id {
    my $self = shift;
    return $self->{id};
}

sub topVote {
    my $self = shift;
    my @notList = @_;
    my @array = $self->array;
    foreach my $vote (@array) {
        my $problem = "";
        foreach my $not (@notList) {
            if ($vote eq $not) {
                $problem = "true";
                last;
            }
        }
        if (! $problem) {
            return $vote;
        }
    }
    return "none";
}

sub totalVotes {
    return scalar @allVotes;
}

# prints out the results
# returns the best percentage and the list of who's worst
sub printNiceResults {
    my %results = @_;
    my $topPercent = 0.;
    my @worst;
    foreach my $count (sort {$b <=> $a} keys %results) {
        my $percent = $count * 100. / totalVotes();
        if (! $topPercent) {
            $topPercent = $percent;
        }
        my $aref = $results{$count};
        printf "%3d (%.1f%%) %s\n", $count, $percent, join(" ", @$aref);
        @worst = @$aref;
    }
    return ($topPercent, @worst);
}

# either returns the whole array (no arguments)
# or returns an element ($index passed in)
sub array {
    my $self = shift;
    my $index = shift;
     my $bla = 0;
     if ((defined $index) && $index =~ /^-?\d+/) {
        return $self->{array}->[$index];
    } else {
        return @{$self->{array}};
    }
}

# finds the overall winner by iteration
sub findWinner {
    my @worst = ();
    my $percent = 0.;
    my $numCandidates = keys %uniqueHash;
    print "Candidates: $numCandidates\n";
    my $count = 0;
    while (@worst < $numCandidates && $percent <= 50.0) {
        print "\n\n";
        print "Round ", ++$count, "\n";
        print "Not being counted: @worst\n" if (@worst);
        my @lowest;
        my %results = countVotes(@worst);
        ($percent, @lowest) = printNiceResults(%results);
        push @worst, @lowest;
    }    
}

# makes a string version of a ballot
sub stringify {
    my $self = shift;
    my $retval = $self->id.": ".join(" ", $self->array);
}

# prints all votes
sub printAllVotes {
    my $count = 0;
    foreach my $vote (@allVotes) {
        printf "%3d) $vote\n", ++$count;
    }
}

# All packages end with a 1
1;
