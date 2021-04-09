#!/usr/bin/perl

use strict;
use warnings;

my @blist = ();
my %hash;
my $step = 0.01;
my $deltaEpsilon = 0.43;

while (<>) {
    if (/b = ([\d\.]+)/) {
        push @blist, $1;
        next;
    }
    if (/([\d\.]+)\s*:\s*([\d\.]+)/) {
        push @{$hash{$1}}, $2 / $deltaEpsilon;
    }
}


my $bsize = @blist;
my $min = $blist[0];
my $max = $blist[ $#blist ];

if ($max - $min == ($bsize - 1) * $step) {
    print "All bins are accounted for\n";
} else {
    print "** WARNING ** Missing Bins\n";
}

foreach my $key (keys %hash) {
    my $size = @{$hash{$key}};
    die "$key size $size doesn't match blist size $bsize\n" 
        unless ($size == $bsize);
}

print "b: @blist\n\n";

foreach my $key (sort {$a <=> $b} keys %hash) {
    print "$key : ", join (" ", @{$hash{$key}}), "\n\n";
}

$min -= $step / 2;
$max += $step / 2;
my $niceSize = "size: $bsize, $min, $max\n";
print "$niceSize\n";

foreach my $key (sort {$a <=> $b} keys %hash) {
    open(TARGET, ">limits_$key.txt") or die;
    print TARGET "$bsize\n$min\n$max\n0\n"; # 0 is underflow bin
    print TARGET join ("\n", @{$hash{$key}}), "\n0\n"; # 0 is overflow bin
}
