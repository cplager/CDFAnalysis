#!/usr/bin/perl

use warnings;
use strict;

die "Usage: $0 file1.time file2.time\n" unless (@ARGV >= 2);

my $onefile = shift @ARGV;
my $twofile = shift @ARGV;

open (FIRSTFILE, $onefile) or die;
open (SECONDFILE, $twofile) or die;

my (@first, @second);

my $last = -1;
my $line = 0;
while (<FIRSTFILE>) {
    ++$line;
    chomp;
    s/^\s*//; # remove leading spaces
    s/\#.+//; # remove comments
    my $value = -1;
    if (/([\d\.\-]+)/) {
        $value = $1;
    } else {
        next;
    }
    if ($value <= $last) {
        warn "In line $line, $value <= $last\n";
    } else {
        push @first, $value;
    }
} # while firstfile

$last = -1;
$line = 0;
while (<SECONDFILE>) {
    ++$line;
    chomp;
    s/^\s*//; # remove leading spaces
    s/\#.+//; # remove comments
    my $value = -1;
    if (/([\d\.\-]+)/) {
        $value = $1;
    } else {
        next;
    }
    if ($value <= $last) {
        warn "In line $line, $value <= $last\n";
    } else {
        push @second, $value;
    }
} # while secondfile

my $size1 = @first;
my $size2 = @second;
my %outputHash;
my @results;
my $current = 0;
for my $loop (0..$size1 - 1) {
    my $diff;
    ($diff, $current) = findClosest ($first[$loop], $current);
    my $output = 
        sprintf("%4d) %4d %3d\n", $loop + 1, $diff, $loop - $current);
    print $output;
    push @results, $diff;
    $outputHash{$loop} = $output;
}

# check for "special values"
print "\nspecial:\n";
my $size = @results;
my @special;
foreach my $loop (0..$size - 1) {
    if ($loop < 0) {
        $loop = 0;
    }
    if ($loop >= $size) {
        $loop = $size - 1;
    }
    my $previous = $loop - 1;
    if ($previous < 0) {
        $previous = 0;
    }
    my $before = abs ($results[$previous]);
    my $current = abs($results[$loop]);
    next if (($current == $before) && ($loop != $previous));
    while ($current == $before) {
        --$previous;
        if ($previous < 0) {
            $before = $current + 1000; # something small
        } else {
            $before = abs($results[$previous]);
        }
    } # while we're at a plateau
    next unless ($before > $current);
    
    my $next = $loop + 1;
    if ($next >= $size) {
        $next = $size - 1;
    }
    my $after = abs($results[$next]);
    while ($current == $after) {
        ++$next;
        if ($next >= $size) {
            $after = $current + 1000; # something big
        } else {
            $after = abs($results[$next]);
        }
    } # while we're at a plateau
    if ($current < $after) {
        #push @special, "c $current b $before a $after $outputHash{$loop}";
        push @special, $outputHash{$loop};
    }
}

foreach $_ (@special) {
    print;
}

#################
## Subroutines ##
#################

sub findClosest {
    my $value = shift;
    my $current = shift;
    my ($before, $on, $after) = (0, 10, 0);
    ($before, $on, $after) = getCurrentDiffs ($value, $current);
    #print "v $value c $current b $before o $on a $after\n";
    # loop while you don't have the best difference
    my $counter = 0;
    while ( (abs($before) < abs($on)) || 
            (abs($after)  < abs($on)) ) {
        if ($on > 0) {
            # second > value => second index too high
            --$current;
        } else {
            # second < value <= second index too low
            ++$current;
        }
        ($before, $on, $after) = getCurrentDiffs ($value, $current);
        #print "v $value c $current b $before o $on a $after\n";
        die "too many iterations\n" if (++$counter > 10);
    } # not yet best difference
    return ($on, $current);
}

sub getCurrentDiffs {
    my $value = shift;
    my $current = shift;
    if ($current < 0) {
        $current = 0;
    }
    if ($current >= $size2) {
        $current = $size2 - 1;
    }
    my $previous = $current - 1;
    if ($previous < 0) {
        $previous = 0;
    }
    my $next = $current + 1;
    if ($next >= $size2) {
        $next = $size2 - 1;
    }
    return ($second[$previous] - $value, 
            $second[$current] - $value,  
            $second[$next] - $value);
}
