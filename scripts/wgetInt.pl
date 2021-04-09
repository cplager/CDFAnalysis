#!/usr/bin/env perl
#!/usr/bin/perl

use strict;
use warnings;
use LWP;

#use PromptUtil;
use vars '@ISA';
@ISA = 'LWP::UserAgent';

my $ext = "rootlist";

my $help = "Usage: $0 http://web.address
Gets a file
Options:
--help      =>  this screen
--output XX =>  Saves output as 'XX'
";

my $output;
while (@ARGV && $ARGV[0] =~ /^-/) {
   (my $arg = shift) =~ s/^-+//;
   if ($arg =~ /^h/i) {
      die $help;
   }
   if ($arg =~ /^out/i) {
      $output = shift;
      next;
   }
   die "Do not understand '--$arg'.  Aborting.\n";
}

my $url = shift || "";

die "$help\n" unless ($url);


my (%alpha_hash, %beta_hash);


(my $filename = $url) =~ s|.+/||g;
$filename = $filename || "index_.html";
$output = $filename unless $output;

filelink ($url, $output);

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
       warn "Can not find url '$url'.\n\n";
       return;
    }
    print "saving $filename\n\n";
    open (TARGET, ">$filename") or die;
    binmode TARGET;
    print TARGET $response->content;
    return $filename;
}

