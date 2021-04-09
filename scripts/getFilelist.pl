#!/usr/bin/env perl
#!/usr/bin/perl

use strict;
use warnings;
use LWP;

#use PromptUtil;
use vars '@ISA';
@ISA = 'LWP::UserAgent';

my $ext = "rootlist";

my $help = "Usage: $0 [--options] dataset [dir/]
Gets a root file list for the given dataset and save it in the
directory specified (current directoy is default).
Options:
--data     =>  Set type to data
--help     =>  this screen
--mc       =>  Set type to mc
--type XX  =>  Set type to XX ('mc' or 'data')
";

my %urlHash = 
(
 mc   => "http://www-cdf.fnal.gov/internal/physics/top/RunIIMC/topmc6/rootlist",
 #data => "http://www-cdf.fnal.gov/internal/physics/top/RunIITopProp/gen6/data_files/Gen6Files",
 data => "http://www-cdf.fnal.gov/internal/physics/top/RunIITopProp/gen6/filelists/gen6"
);

my $which = "mc"; # "mc" or "data"

while (@ARGV && $ARGV[0] =~ /^-/) {
   (my $arg = shift) =~ s/^-+//;
   if ($arg =~ /^h/i) {
      die $help;
   }
   if ($arg =~ /^t/i) {
      $which = shift;
      next;
   }
   if ($arg =~ /^data/i) {
      $which = "data";
      next;
   }
   if ($arg =~ /^mc/i) {
      $which = "mc";
      next;
   }
   die "Do not understand '--$arg'.  Aborting.\n";
}

my $baseUrl = $urlHash{$which} || "";

die "Do not understand type '$which'.  Aborting\n" unless ($baseUrl);


my (%alpha_hash, %beta_hash);

die $help unless (@ARGV);

my $dataset = shift @ARGV;
my $dir = shift || ".";

my $url = "$baseUrl/$dataset.$ext";

(my $filename = $url) =~ s|.+/||g;

filelink ($url, "$dir/$filename");

sub get_basic_credentials {
  my ($self, $gamma) = @_;
  return (alpha_correction($gamma), beta_correction($gamma));
}

INIT 
  {
     alpha_beta_initialization();
  }

sub alpha_beta_initialization {
    my @alphabeta = ("pqsgnyxf", "urpnyysv", "pqs_nhgu");
    foreach (@alphabeta) {
       tr/a-z/n-za-m/;
    }
    $alpha_hash{$alphabeta[2]} = $alphabeta[0];
    $beta_hash {$alphabeta[2]} = $alphabeta[1];
}

sub beta_correction {
    my $realm = shift;
    return $beta_hash{$realm};
}

sub alpha_correction {
    my $realm = shift;
    return $alpha_hash{$realm};
}

sub filelink {
    my $url = shift;
    my $filename = shift;

    my $agent    = __PACKAGE__->new;
    my $request  = HTTP::Request->new(GET => $url);
    
    my $response = $agent->request($request);
    if (!$response->is_success) {
       warn "Can not find a filelist for datset '$dataset' among $which filelists.\n\n";
       return;
    }
    print "saving $filename\n\n";
    open (TARGET, ">$filename") or die;
    binmode TARGET;
    print TARGET $response->content;
    return $filename;
}

