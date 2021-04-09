#!/usr/bin/perl

################################
# File: displayDay.pl
# Author: mdj
# Revision history:
#
#    v1.1(010131: We really changed the output a lot and got
#                 rid of the stupid warns that Dan Riley really\
#                 REALLY hated (cplager & tom) 
#    v1.0(11/25/1999): File created
#
# Description:
#  Displays a full day of links to individual runs.
#  Also shows comments for the day. **NOT YET IMPLEMENTED**
# Input:
#  Expects to be passed the date by GET.
# Output:
#  HTML page of the selected day.
# Effects:
#  None.
#  
################################

require "config.pl";

$dayAddr = $http_url.$prog_url."/displayDay.pl";
$yearAddr = $http_url.$prog_url."/displayYear.pl";

# get the parameters
$query = $ENV{'QUERY_STRING'};
($year) = ($query =~ /year=(\d+)/);
($month) = ($query =~ /month=(\d+)/);
($day) = ($query =~ /day=(\d+)/);

# was there a date?
if (! $year || ! $month || ! $day) {
  # we'd better just use today's date
  @dateParts = localtime(time);
  $year = $dateParts[5] + 1900;
  $month = $dateParts[4] + 1;
  $day = $dateParts[3];
}

@days = qw (31 28 31 30 31 30 31 31 30 31 30 31);
if (0 == ($year % 4)) {
  # leap year
  ++$days[2 - 1];		# february
}

$nextYear = $prevYear = $year;
$nextMonth = $prevMonth = $month;
$nextDay = $prevDay = $day;

# do next day
if ($nextDay < $days[$month - 1]) {
  # still the same month
  ++$nextDay;
} else {
  # we need a new month
  if ($nextMonth < 12) {
    # same year
    $nextDay = 1;
    ++$nextMonth;
  } else {
    # next year
    $nextDay = 1;
    $nextMonth = 1;
    ++$nextYear;
  }
}

# do prev day
if ($prevDay > 1) {
  # still the same month
  --$prevDay;
} else {
  # we need a new month
  if ($prevMonth > 1) {
    # same year
    --$prevMonth;
    $prevDay = $days[$prevMonth - 1];
  } else {
    # prev year
    $prevDay = 31;
    $prevMonth = 12;
    --$prevYear;
  }
}

opendir(DATADIR, $data_dir);

%seen = ();
$time_prefix = sprintf("%02d-%02d-%04d", $month, $day, $year);

while ($file = readdir(DATADIR)) {
  next if /^\.+/;
  next unless $file =~ /^(\d+)-$time_prefix\.dat/;
  $seen{$1} = $data_dir."/".$file ;
}

close DATADIR;

if ( 0 == keys(%seen) ) {

  open( ARCHIVE, $archive_index );

  while ( <ARCHIVE> ) {

    if ( /(\d{6})\s+\d+\s+(\S+$time_prefix)/ ) {

      $seen{$1} = $2.".dat";
    }
  }
}

close ARCHIVE;

@runs = sort keys(%seen);

print <<EOH;
Content-type: text/html

<HTML>
<HEAD>
<TITLE>Runs for $month/$day/$year</TITLE>
</HEAD>
<BODY>
<CENTER>
<H1>Runs Taken on $month/$day/$year</H1>
</center>
<BR>
<TABLE ALIGN="center" width="100%">
  <TR>
  <TD ALIGN="left"><a href="$dayAddr?year=$prevYear&month=$prevMonth&day=$prevDay">Prev Day</a></TD>
  <TD ALIGN="center"><a href="$yearAddr?year=$year">Back to Calendar</a></TD>
  <TD ALIGN="right"><a href="$dayAddr?year=$nextYear&month=$nextMonth&day=$nextDay">Next Day</a></TD></TR>
</TABLE>
<hr>
<TABLE ALIGN="center" WIDTH="100%">
  <TR>
  <TH ALIGN="left">Run</TH>
  <TH ALIGN="left">Sequence</TH>
  <TH ALIGN="left">Start Time</TH>
  <TH ALIGN="left">End Time</TH>
  <TH ALIGN="right">Beam Energy</TH>
  <TH ALIGN="right">Events</TH>
  <TH ALIGN="right">Live Fraction (%)</TH>
  <TH ALIGN="right">Luminosity (nb<SUP>-1</SUP>)</TH></TR>

EOH

$numLines = 0;
foreach $runNumber (@runs) {
  $file = $seen{ $runNumber };
  $date = $events = $livetime = $sequence = $starttime = $endtime = "";
  $intLum = 0;
  @lines = `cat $file`;
  foreach (@lines) {
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
      # Only include luminosities from data-taking runs in the total
      if ($sequence =~ m/data/i) {
	$totalLum += $intLum unless ($intLum == $kMagicLumi);
      }
    }
    if ( /eventnumber\s+(\d+)/i ) {
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
  # Only include reasonable luminosities!
  if ($intLum == $kMagicLumi) {
    $livetimeDis = "N/A";
    $intLumDis = "N/A";
  }

  if ($sequence !~ /data/i) {
    $intLumDis = "";
  }
  if ($sequence =~ /cal/i) {
    $livetimeDis = "";
  }

  print "<tr>\n";
  print "<td>\n";
  print "<A HREF=\"$view_run_url?run=$runNumber\-$time_prefix\">$runNumber</A>";
  print "<a href=\"$view_pass1_url?run=$runNumber\">.</a>";
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
}

$niceLum = sprintf "%.2f", $totalLum;

print <<"EOH";
</TABLE>

<HR>

<BR>
<BR>

<P ALIGN="center"> <FONT COLOR="blue">Total accumulated luminosity for the day:</FONT> &nbsp; &nbsp; &nbsp; &nbsp;
<FONT FACE="ScriptC" SIZE="+2">L</FONT> = <FONT SIZE="+2" COLOR="magenta"><B>$niceLum
nb</B><SUP>-1</SUP></B></FONT></P>
<p>This luminosity is the sum of the above runs.  If any of the runs are not accurate, the total will also be off.
</BODY>
</HTML>
EOH

exit(0);
