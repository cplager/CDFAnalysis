#!/usr/bin/perl

 #####################################################
 ## This is a perl script that will find the run(s) ##
 ## that you are looking for and make a list of     ##
 ## them so you can click away...                   ##
 #####################################################

require "config.pl";
$query = $ENV{'QUERY_STRING'};
@parts = split '&', $query;
$string = $parts[0];
@pieces = split '=' $string;
$rType = $pieces[1]; 
$rType = 3;

print "Content-type: text/html\n\n";
print << "EOH";


print "<html>";
print "<head>";
print "<title>Searching for $query</title>";
print << "EOH";
EOH;

print "</HEAD>";
print "<BODY>";
EOH;
   

$file = "/cdat/lnsro4/disk1/hennessy/cleo3/runmanager/runlist/runlist2.dat";
open (SOURCE, $file) or warn "Can't open $file\n";
       
	while (<SOURCE>) {@pp = split " ", $_ ;  if($pp[5] = ~ "Global|DataTaking"){ $sum = $sum + $pp[3]} ; }
     print $sum ;
for $index (0..$#parts-1) {if($parts[$index]=~"runSelect=thrSonL"){print "HI ";}  ;}
foreach (@parts) { print  "$1" ; }
print "</body>\n";
print "</html>\n";
exit(0);



     
