#!/usr/bin/perl

#####################################################
## This is a perl script that will find the run(s) ##
## that you are looking for and make a list of     ##
## them so you can click away...                   ##
#####################################################

require "config.pl";
# get the parameters
$query = $ENV{'QUERY_STRING'};
if ($query =~ /\&submit/i) {
  $query = $`;
}
if ($query=~ /number=(.+)/i) {
  $numPattern = $1;
}
# put '?'s back in
$numPattern =~ s/\%3F/?/g;
# get rid of everything that is not a number, * or ?
$numPattern =~ s/[^\d\*\?]//g;
if ($numPattern !~ /^\d/) {
  $numPattern = "1$numPattern";
}

# Find runs that match run-number pattern
@files = glob ("$data_dir/$numPattern*.dat");
foreach $file (@files) {

  if ( $file =~ /(\d{6})-\d\d-\d\d-\d{4}\.dat/ ) {

    $fileName{ $1 } = $file;
  }
}

# Now look through the archive, but first turn $numPattern into a regexp.
# We need to save $numPattern for display later.

$regPattern = $numPattern;
$regPattern =~ s/\?/\\d/g;
$regPattern =~ s/\*/\\d\*/g;

open( ARCHIVE, $archive_index );

while ( <ARCHIVE> ) {

  if ( /^($regPattern)\s+\d+\s+(\S+)/ ) {

    $fileName{$1} = $2.".dat";
  }
}

@runs = sort keys %fileName;
$numRuns = @runs;

# Print head of table
print <<"EOH";
Content-type: text/html

<HTML>
<HEAD>
<TITLE>Searching for Runs for $numPattern</TITLE>
<h1>Searching for runs $numPattern</h1>
</HEAD>
<BODY>

<H3>(Found $numRuns runs)</H3>

<CENTER>
</center>
<br><hr>
<TABLE ALIGN="center" WIDTH="100%">
  <TR>
  <TH ALIGN="left">Run</TH>
  <TH ALIGN="left">Date</TH>
  <TH ALIGN="left">Sequence</TH>
  <TH ALIGN="left">Start Time</TH>
  <TH ALIGN="left">End Time</TH>
  <TH ALIGN="right">Beam Energy</TH>
  <TH ALIGN="right">Events</TH>
  <TH ALIGN="right">Live Fraction (%)</TH>
  <TH ALIGN="right">Luminosity (nb<SUP>-1</SUP>)</TH></TR>

EOH
;

$numLines = 0;
foreach $runNumber (@runs) {
  $file = $fileName{$runNumber};
  $date = $events = $sequence = $starttime = $endtime = "";
  $livetime = $intLum = 0;
  @lines = `cat $file`;

  foreach (@lines) {
    if (m|(\d+/\d+/\d+)| && ! $date) {
      $date = $1;
    }
    if (/runStartTime.+\s+(\d+\:\d+\:\d+)/i) {
      $starttime = $1;
    }
    if (/runendTime.+\s+(\d+\:\d+\:\d+)/i) {
      $endtime = $1;
    }
    if (/runsequence\s+/i) {
      $sequence = $';		# get everything after the match
      if ($sequence =~ m|([^/]*)$|) {
	$sequence = $1;
      }
    }
    if (/livetime_percent\s+/i) {
      $livetime = $';
    }
    if (/integLum_invnb\s+/i) {
      $intLum = $';
      $totalLum += $intLum;
    }
    if (/eventnumber\s+(\d+)/i) {
      $events = $1;
    }
    if ( /energy_kev\s+(\d+\.\d+)/i ) {
      $energy = $1;
    }
  }

  # Fix that will only use
  #   the event number if we've already seen
  #   a run end-time, else it prints "N/A"
  if ($endtime eq "") {
    $events = "N/A";
    $livetimeDis = "N/A";
    $intLumDis = "N/A";
  } else {
    $livetimeDis = sprintf "%.2f", $livetime;
    $intLumDis = sprintf "%.2f", $intLum;
  }
  if ($sequence !~ /data/i) {
    $intLumDis = "";
  }
  if ($sequence =~ /cal/i) {
    $livetimeDis = "";
  }

  print "<tr>\n";
  print "<td>\n";
  print "<A HREF=\"$view_run_url?run=$file\">$runNumber</A>";
  print "</td><td>\n";
  print "$date";
  print "</td><td>\n";
  if ($sequence =~ /cal/i) {
    print "<font color = \"green\">$sequence</font>";
  } else {
    print "$sequence";
  }
  print "</td><td>\n";
  print "$starttime";
  print "</td><td>\n";
  print "$endtime";
  print "</td><td ALIGN=\"right\">\n";
  print $energy;
  print "</td><td ALIGN=\"right\">\n";
  print "$events";
  print "</td><td ALIGN=\"right\">\n";
  print $livetimeDis;
  print "</td><td ALIGN=\"right\">\n";
  print $intLumDis;
  print "</td></tr>\n";
  if (++$numLines % 4 == 0) {
    print "<TR><TD></TD></TR>\n";
  }
}				# foreach runNumber

print "</TABLE>\n<HR>\n";
if (! $numLines) {
  print "<h1>Sorry, no results found.</h1><br>\n";
}

print <<"EOH";
</body>
</html>
EOH
;
