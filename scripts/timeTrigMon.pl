#!/usr/bin/env perl

use strict;
use warnings;
use LWP;
use Time::Local;
#use Time::localtime;

my @months = qw (jan feb mar apr may jun jul aug sep oct nov dec);
my $monitor = "TrigMon";
my $computer = "server";
my $first = 0;
my $last = "";

print "Monitor Run   Event    Sec    Date\n";
while (1) {

   my @lines = split /\n/, getPage();
   
   foreach my $line (@lines) {
      chomp $line;
      next unless ($line =~ /$monitor/i && $line =~ /$computer/i);
      # don't bother if this matches the last line
      my @parts = split m|</td>|, $line;
      foreach (@parts) {
         s/<[^>]*>//g;
         s/^\s+//;
         s/\s+$//;
      }
      my @output = (@parts)[0, 3, 4];
      my $events = $parts[4];
      my $date   = $parts[7];
      my $epoch = 0;
      if ($date =~ /\w+\s+(\w+)\s+(\d+)\s+(\d+):(\d+):(\d+)\s+(\d+)/) {
         my $month = $1;
         my $day = $2;
         my $hour = $3;
         my $minute = $4;
         my $seconds = $5;
         my $year = $6;
         #print "m $month d $day $hour:$minute:$seconds y $year\n";
         my $monthnum = 0;
         foreach my $string (@months) {
            ++$monthnum;
            if ($month =~ m|$string|i) {
               last;
            }
         }
         $epoch = timelocal ($seconds, $minute, $hour, $day, $monthnum - 1, 
                             $year - 1900);
         ##  my $tm = localtime($epoch);
         ##  printf("Dateline: %02d:%02d:%02d-%04d/%02d/%02d\n",
         ##         $tm->hour, $tm->min, $tm->sec, $tm->year+1900,
         ##         $tm->mon+1, $tm->mday);
         if (! $first) {
            $first = $epoch;
         }
         $epoch -= $first;
      }
      #print "epoch $epoch\n";
      my $out = sprintf ("@output %6d    $parts[7]", $epoch);
      if ($last eq $out) {
         next;
      }
      #print "last $last out $out\n";
      $last = $out;
      print "$out\n";
      next;
   } # foreach line
   sleep 30;
} # while 1


sub getPage {
   my $url = "http://www-cdfonline.fnal.gov/consumer/consumer_status.html";

   my $agent   = LWP::UserAgent->new;
   my $request = HTTP::Request->new(GET => $url);
   
   my $response = $agent->request($request);
   if (!$response->is_success) {
      warn "$url: ", $response->message,"\n";
      return "";
   }
   return $response->content;
}
