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
foreach (@parts) {
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
  if (/string=/i) {
    $string = $';
    next;
  }
}

$part = join "<br>\n", @parts;
print <<"EOH";
Content-type: text/html

<HTML>
<HEAD>
<TITLE>Searching for $string</TITLE>
EOH
;
print "<h1>";
if ($which =~ /run/i) {
  # do we need to fake endRun?
  if ( $endRun < $startRun) {
    print "Searching for runs from run $startRun for string \"$string\"\n";
    $endRun = 899999;		# something big
  } else {
    print "Searching for runs from run $startRun to run $endRun for string \"$string\"\n";
  }
} elsif ($which =~ /date/i) {
  # do we need to fake endDate (is startDate > endDate
  if (! compDate ($startMonth, $startDay, $startYear,
		  $endMonth,   $endDay,   $endYear)) {
    print "Searching from date $startMonth/$startDay/$startYear for string \"$string\"\n";
    $endYear = 2999;
  } else {
    print "Searching from date $startMonth/$startDay/$startYear to $endMonth/$endDay/$endYear for string \"$string\"\n";
  }
} else {
  print "Searching for string \"$string\" in today's runs\n";
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

      push @runsToSearch, $run;
    }
  }				# foreach @files
} elsif ($which =~ /date/i) {

  foreach $run (keys %files) {

    $name = $files{ $run };

    if ($name =~ /(\d{6})\-(\d{2})\-(\d{2})\-(\d{4})/) {
      $run = $1;
      $month = $2;
      $day = $3;
      $year = $4;

      if ( compDate ($startMonth, $startDay, $startYear,
		     $month,      $day,      $year) &&
	   compDate ($month,      $day,      $year,
		     $endMonth, $endDay, $endYear)) {

	push @runsToSearch, $run;
      }
    }
  }
} else {

  #today

  @dateParts = localtime(time);
  $year = $dateParts[5] + 1900;
  $month = $dateParts[4] + 1;
  $day = $dateParts[3];
  $time_prefix = sprintf("%02d-%02d-%04d", $month, $day, $year);

  foreach $run (keys %files) {

    $name = $files{ $run };

    if ($name =~ /$time_prefix$/) {

      push @runsToSearch, $run;
    }
  }				# foreach @files
}

# great  Now we got a list of files that we want
# lets search them

$numMatchRuns = 0;
foreach $run (sort @runsToSearch) {

  $notesFile = $files{ $run }.".".$notes_ext;
  $datFile = $files{ $run }.".".$db_ext;

  @comments = @parts = @lines = ();
  $found = $endComment = "";

  if (-e $notesFile) {

    open (SOURCE, $notesFile) or warn "Can't open $notesFile\n";

    while (<SOURCE>) {
      chomp;
      push @lines, $_;
    }
    $line = join " ", @lines;
    @parts = split /$record_sep/, $line;
    foreach (@parts) {
      s/$field_sep/ /g;
      if (m|^CMNT|i) {
	$comment = $';
	if ($comment =~ m|(\d{2}/\d{2}/\d{4})|) {
	  $before = $`;
	  $date = $1;
	  $rest =$';
	  $comment = "<font color=\"brown\">$before$date:</font>$rest";
	  push @comments, $comment;
	}
      }				# if CMNT
      if (/^XTRA endComment\s/) {
	$comment = $';
	if ($comment =~ /\w/) {
	  $endComment = "<FONT COLOR=\"darkorchid\">End of run comment:</FONT> $comment";
	}
      }				# if endComment
    }				# foreach parts
    push @comments, $endComment if ($endComment);
  } else {			# does this file exist, else ...
    print "wandering $notesFile<br>\n";
  }
  foreach (@comments) {
    if (m|\</font\>|i) {
      $check = $';
    } else {
      $check = $_;
    }
    if ($check =~ /$string/i) {
      $found = "true";
      last;
    }
  }

  if ($found) {

    ++$numMatchRuns;
    $runDate = $events = $livetime = $sequence = 
      $starttime = $endtime = "";

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
	if (/data/i) {
	  $totalLum += $intLum unless ($intLum == $kMagicLumi);
	}
      }
      if (/eventnumber\s+(\d+)/i) {
	$events = $1;
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
    print "$events";
    print "</td><td ALIGN=\"right\">\n";
    print $livetimeDis;
    print "</td><td ALIGN=\"right\">\n";
    print $intLumDis;
    print "</td></tr>\n";
    print "<tr> <td colspan=\"7\"><ul>\n";
    foreach (@comments) {
      if (m|\</font\>|i) {
	$before = $`.$&;
	$check = $';
      } else {
	$check = $_;
	$before = "";
      }
      if ($check =~ /$string/i) {
	$check =~ s/($string)/\<font color=\"red\"\>$1\<\/font\>/ig;
	print " <li><font size=\"-1\"> $before$check </font>\n";
      }
    }				# foreach @comments
    print "</td><td></td> </tr> </ul>\n";
    print "<TR><TD></TD></TR>\n";
    print "<TR><TD></TD></TR>\n";
  }				# found
}				# foreach run

print "</TABLE>\n<HR>\n";
print "<H3>(Found $numMatchRuns runs that matched)</H3>\n";
print "</body>\n";

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
