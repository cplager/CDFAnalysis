#!/usr/bin/perl

 #####################################################
 ## This is a perl script that will find the run(s) ##
 ## that you are looking for and make a list of     ##
 ## them so you can click away...                   ##
 #####################################################

require "config.pl";
# get the parameters
$query = $ENV{'QUERY_STRING'};
@parts = split '&', $query;
for $index (0..$#parts) {
  $ok = "true";
  $parts[$index] =~ s/\+/ /g;
  while ($ok) {
    if ($parts[$index] =~ /\%([\dA-F]{2})/) {
      $num = hex($1);
      $char = chr($num);
      $parts[$index] = $`.$char.$';
    } else {
      $ok = "";
    }
  }
}

$endRun = 0;
$doNewLines = "";
foreach (@parts) {
  if (/newline=on/i) {
    $doNewlines = "true";
  }
  if (/runSelect=/i) {
    $which = $';
    next;
  }
  if (/startRun=(\d+)/i) {
    $startRun = $1;
    next;
  }
  if (/startMonth=(\d+)/i) {
    $startMonth = $1;
    next;
  }
  if (/startDay=(\d+)/i) {
    $startDay = $1;
    next;
  }
  if (/startYear=(\d+)/i) {
    $startYear = $1;
    next;
  }
  if (/endRun=(\d+)/i) {
    $endRun = $1;
    next;
  }
  if (/endMonth=(\d+)/i) {
    $endMonth = $1;
    next;
  }
  if (/endDay=(\d+)/i) {
    $endDay = $1;
    next;
  }
  if (/endYear=(\d+)/i) {
    $endYear = $1;
    next;
  }
}

$part = join "<br>\n", @parts;
print <<"EOH";
Content-type: text/html

<HTML>
<HEAD>
<TITLE>Reading Comments</TITLE>
EOH
	;
print "<h1>";
if ($which =~ /run/i) {
  # do we need to fake endRun?
  if ( $endRun < $startRun) {
    print "Reading comments for runs from run $startRun\n";
    $endRun = 899999;		# something big
  } else {
    print "Reading comments for runs from run $startRun to run $endRun\n";
  }
} elsif ($which =~ /date/i) {
  # do we need to fake endDate (is startDate > endDate
  if (! compDate ($startMonth, $startDay, $startYear,
		  $endMonth,   $endDay,   $endYear))
    {
      print "Reading comments from date $startMonth/$startDay/$startYear\n";
      $endYear = 2999;
    } else {
      print "Reading comments from date $startMonth/$startDay/$startYear to $endMonth/$endDay/$endYear\n";
    }
} elsif ($which =~ /last100/i) {
  
  print "Reading comments for the last 100 runs\n";
} else {

  print "Reading comments in today's runs\n";
}
print "</h1>\n";
print << "EOH";
</HEAD>
<BODY>
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

open( INDEX, "$archive_index" );

while( <INDEX> ){

  chomp;

  my ($run, $date, $file) = split /\t/;

  $files{ $run } = $file;
}

@dataDirFiles = glob "$data_dir/*.notes";

foreach $file (@dataDirFiles){

  $file =~ s/\.notes//g;
  if( $file =~ /(\d{6})/ ){

    $files{ $1 } = $file;
  }
}

if ($which =~ /run/i) {

  foreach $run (keys %files) {

    if (($run >= $startRun) && ($run <= $endRun)) {

      push @runsToDisplay, $run;
    }
  }
}
elsif ($which =~ /date/i) {

  foreach $run (keys %files) {

    $name = $files{ $run };

    if ($name =~ /(\d{6})\-(\d{2})\-(\d{2})\-(\d{4})/) {
      $month = $2;
      $day = $3;
      $year = $4;

      if ( compDate ($startMonth, $startDay, $startYear,
		     $month,      $day,      $year) &&
	   compDate ($month,      $day,      $year,
		     $endMonth, $endDay, $endYear)) {

	push @runsToDisplay, $run;
      }
    }
  }
}
elsif ( $which =~ /last100/i ){

  @runs = (sort keys %files);
  $lastRunIndex = @runs - 1;

  $nRuns = 0;
  while( $nRuns < 100 ){

    push @runsToDisplay, @runs[$lastRunIndex--];
    $nRuns++;
  }
}
else{

  #today

  @dateParts = localtime(time);
  $year = $dateParts[5] + 1900;
  $month = $dateParts[4] + 1;
  $day = $dateParts[3];
  $time_prefix = sprintf("%02d-%02d-%04d", $month, $day, $year);

  foreach $run (keys %files) {

    $name = $files{ $run };

    if ($name =~ /$time_prefix$/) {

      push @runsToDisplay, $run;
    }
  }
}

# great  Now we got a list of files that we want
# lets search them

foreach $run (sort @runsToDisplay) {

  $notesFile = $files{ $run }.".".$notes_ext;
  $datFile = $files{ $run }.".".$db_ext;

  $runDate = $events = $livetime = $sequence = $starttime = $endtime = "";
  @lines = `cat $datFile`;
  foreach (@lines) {
    if (m|(\d+/\d+/\d+)| && ! $runDate) {
      $runDate = $1;
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
      # Only include luminosities from data-taking runs in the total
      if ($sequence =~ m/data/i) {
	$totalLum += $intLum unless ($intLum == $kMagicLumi);
      }
    }
    if (/eventnumber\s+(\d+)/i) {
      $events = $1;
    }
    if ( /energy_kev\s+(\d+\.\d+)/i ) {
      $energy = $1;
    }
  }				# foreach lines
  
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
  print "<A HREF=\"$view_run_url?run=$files{$run}\">$run</A>";
  print "<a href=\"$view_pass1_url?run=$run\">.</a>";
  print "</td><td>\n";
  print "$runDate";
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
  print "$energy";
  print "</td><td ALIGN=\"right\">\n";
  print "$events";
  print "</td><td ALIGN=\"right\">\n";
  print $livetimeDis;
  print "</td><td ALIGN=\"right\">\n";
  print $intLumDis;
  print "</td></tr>\n";
  print "<tr> <td colspan=\"7\"><ul>\n";
  if (-e $notesFile) {
    open (SOURCE, $notesFile) or warn "Can't open $notesFile\n";
    @lines = @comments = ();
    $endComment = "";
    while (<SOURCE>) {
      chomp;
      push @lines, $_;
    }
    
    if ($doNewlines) {
      $line = join "\n", @lines;
    } else {
      $line = join " ", @lines;
    }
    @parts = split /$record_sep/, $line;
    foreach (@parts) {
      s/$field_sep/ /g;
      if (m|^CMNT|i) {
	$comment = $';
				# lets make the name and date red
	if ($comment =~ m|(\d{2}/\d{2}/\d{4})|) {
	  $before = $`;
	  $date = $1;
	  $rest =$';
	  #warn "before: $before\ndate: $date\nrest: $rest\n";
	  if ($doNewlines) {
	    $comment = "<font color=\"brown\">$before$date:</font><br>$rest";
	  } else {
	    $comment = "<font color=\"brown\">$before$date:</font>$rest";
	  }
	  #warn "comment $comment\n\n\n";
	}
	if ($doNewlines) {
	  $comment =~ s/\n/<br>/g;
	  $comment =~ s/<br>\s*$//g; # get rid of trailing <br>s
	}
	push @comments, $comment;
      }
      if (/^XTRA endComment\s/) {
				#print "$' <br>\n";
	$comment = $';
	if ($comment =~ /\w/) {
	  $endComment = "<FONT COLOR=\"darkorchid\">End of run comment:</FONT> $comment";
	}
      }
    }				# foreach @parts
    push @comments, $endComment if ($endComment);
  }				# if file exists
  foreach (@comments) {
    $line = $_;
    print " <li><font size=\"-1\"> $line </font size>\n";
  }				# foreach @comments
  print "</td><td></td> </tr> </ul>\n";
  print "<TR><TD></TD></TR>\n";
  print "<TR><TD></TD></TR>\n";
}				# foreach run

$niceLum = sprintf "%.2f", $totalLum;
print "</TABLE>\n<HR>\n";
print << "EOH";
<P ALIGN="center"> <FONT COLOR="blue">Total accumulated luminosity:</FONT> &nbsp; &nbsp; &nbsp; &nbsp;
<FONT FACE="ScriptC" SIZE="+2">L</FONT> = <FONT SIZE="+2" COLOR="magenta"><B>$niceLum
nb</B><SUP>-1</SUP></B></FONT></P>
<p>This luminosity is the sum of the above runs.  If any of the runs are not accurate, the total will also be off.
</BODY>
</HTML>
EOH
	;

exit;

sub compDate {
  # return one if L <= R
  my ($Lmonth, $Lday, $Lyear, $Rmonth, $Rday, $Ryear) = @_;
  # check year first
  if ($Lyear > $Ryear) {
    # L > R
    return 0;
  }
  if ($Lyear < $Ryear) {
    # L < R
    return 1;
  }
  # check month next
  if ($Lmonth > $Rmonth) {
    # L > R
    return 0;
  }
  if ($Lmonth < $Rmonth) {
    # L < R
    return 1;
  }
  if ($Lday > $Rday) {
    # L > R
    return 0;
  }
  return 1;
}
