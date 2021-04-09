#!/usr/bin/env perl

use warnings;
use strict;

my $outputname = shift || "";

my $host = $ENV{HOST} || `hostname`;
my $dir  = $ENV{PWD};
if (! $outputname) {
   ($outputname = $dir) =~ s|/$||;
   $outputname =~ s|.+/||g;
}
if ($outputname !~ /\./) {
   $outputname .= ".rootlist";
}

# give it an extention if it needs it
if ($host !~ /\./) {
   $host .= ".fnal.gov";
}
$host .= "/"; # rootd wants two //s

my $prefix = "root://";

# dcap://fcdfrdc3/pnfs/diskpool/top/TopNtuple_Gen6/Originals/bhel0i/203819_206989/Lepton/topnt_boj_bhel0i-203819-206989-25Jan06-172251_100-Lepton.root

# is this a diskpool directory
if ($dir =~ m|^/pnfs/|) {
   $host = "fcdfrdc3.fnal.gov";
   $prefix = "dcap://";
}

# I'll need to check if I should change this to 'dcap://'

my @files = glob ("*.root");

if (! @files) {
   print "No root files here.  Aborting.\n";
   exit;
}

open (TARGET, ">$outputname") or die "Can't open $outputname for output\n";

foreach my $file (@files) {
   print TARGET "$prefix$host$dir/$file\n";
}

close TARGET;
print "Wrote file list $outputname\n";
