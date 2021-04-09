#!/usr/bin/perl

################################
# File: displayRun.pl
# Author: mdj
# Revision history:
#
#    v1.7(02/06/2001): Added "send comments to expert",
#                      "current run" and "this day"
#    v1.6(10/31/2000): Interlocked before/during run info
#                      Removed ability to add data/trip lines after run
#                        is finished
#    v1.5(09/30/2000): Added submit button for beginRun info
#                      Format changes
#                      Changed run end handling
#    v1.4(09/10/2000): Still more format changes
#    v1.3(08/27/2000): More format changes
#    v1.2(07/26/2000): Format revison, printing
#    v1.1(04/13/2000): Changed to ASCII format databases
#    v1.0(11/28/1999): File created
#
# Description:
#  Shows the data contained in a given run. 
#  Builds links to allow users to annotate the data.
# Input:
#  Expects to be passed the run number by GET.
#  Can be passed the shifter initials, to allow for quick 
#    setting of comment tags
# Output:
#  HTML page of the selected run.
# Effects:
#  Can dispatch requests to add comments.
# Notes:
#  [^\0]+ is used instead of .+ to allow for imbedded newlines.
#  The data table code is pretty hairy self-generating stuff...
#  DBM has been eliminated due to endianness problems between
#    Solaris and Alpha processes
#  
################################

require "config.pl";

$query = $ENV{'QUERY_STRING'};
&do_noSuchRun() if ($query =~ /noSuchRun/);
($run, $month, $day, $year) = ($query =~ /run.+(\d{6})-(\d+)-(\d+)-(\d+)/);
($last_shifter) = ($query =~ /shifter=(\w+)/);
($printable) = ($query =~ /printable/);
($firstrun) = ($query =~ /firstrun/);
($nextrun) = ($query =~ /next/);

# fix single-digit bug
$month = '0' . $month if $month =~ /^\d$/;

# open files 
$time_prefix = "$month-$day-$year";
$totalName = "$run-$time_prefix";
if( -e $data_dir."/".$totalName.".".$notes_ext ){

  $notesName = $data_dir."/".$totalName.".".$notes_ext;
  $dbName = $data_dir."/".$totalName.".".$db_ext;
}
else{

  open( ARCHIVE, $archive_index );

  while( <ARCHIVE> ){

    if( /(\S+$totalName)/ ){

      $notesName = $1.".".$notes_ext;
      $dbName = $1.".".$db_ext;
    }
  }
}


%runData = ();
# convert ASCII format database into associative array
# not as efficient as DBM, which uses a fixed size memory cache...
open(DB,"<$dbName");
while(<DB>) {
    ($name,$val) = split("\t", $_, 2);
    $runData{$name} = $val;
}

warn "Something funny's going on... $run != $runData{\"runNumber\"}" if $run != $runData{"runNumber"};

# print output
# transform shifter list - doesn't do anything yet.

$temp = $runData{"shifterlist"};
# chop($temp); # kill \0
$temp =~ s/(\w+),{0,1}\s*/" " . &get_name($1) . ","/eg;
chop($temp);

# process bonus fields in notes file
open(NOTES, "<$notesName");
flock(NOTES,1); # shared read lock
$/ = $record_sep;

while(<NOTES>) {
    chop;
    next unless /^XTRA$field_sep([^\0]+)/;
    ($key, $val) = split($field_sep, $1);
    $runData{$key} = $val;
}

# global routine to turn data into correct formats
# for example: the beam energy comes from CESR as keV and needs to be
#   displayed in GeV
&cook_data();

# handle redirects and waits
&do_goNext() if $nextrun;


# set up printable capability - magic here
if ($printable) {
    pipe(PRD, PWR);
    if (fork) {
	close(PWR);
	while(<PRD>) {
	    print;
	}
	exit;
    } else {
	open(STDOUT, ">&PWR");
	pipe(PRD2, PWR2);
	if (fork) {
	    close(PWR2);
	    open(STDIN, "<&PRD2");
	    exec "./printable.pl";
	} else {
	    close(PRD2);
	    open(STDOUT, ">&PWR2");
	}
    }
}

# print it out
print <<"EOH";
Content-type: text/html
Pragma: no-cache

<HTML>
<HEAD>
<TITLE> Run $run</TITLE>
</HEAD>
<BODY>
<TABLE WIDTH=100% BORDER=0>
  <TR>
  <TD VALIGN="top"><FONT SIZE=+3>Cleo III Run Sheet</FONT><BR><BR></TD>
  <TD VALIGN="top" COLSPAN=2>
  <TABLE BORDER=1 WIDTH=100%>
    <TR>
    <TD>
    <TABLE WIDTH=100%>
      <TR>
      <TD COLSPAN=2 WIDTH=33% ALIGN=LEFT>Beam Energy: $runData{"energy_GeV"} GeV</TD>
      <TD COLSPAN=2 WIDTH=33% ALIGN=CENTER>Luminosity: $runData{"integLum_invnb"} 1/nb</TD>
      <TD COLSPAN=2 WIDTH=33% ALIGN=RIGHT>B Field: $runData{"BField_kG"} kG </TD>
      </TR>

      <TR>
      <TD COLSPAN=2 WIDTH=33% ALIGN=LEFT>SEN 1 NMR: $runData{"SEN1NMR"}</TD>
      <TD COLSPAN=2 WIDTH=33% ALIGN=CENTER>SEN 2 NMR: $runData{"SEN2NMR"} </TD>
      <TD COLSPAN=2 WIDTH=33% ALIGN=RIGHT>CESR Fill: $runData{"CESRFill"}</TD>
      </TR>
    </TABLE>
    </TD>
    </TR>
  </TABLE>
  </TD>

  <TD VALIGN="top" ALIGN=CENTER ROWSPAN="6">
  <TABLE BORDER=0>
<!--INPUT--><TR><TD></TD></TR>
<!--INPUT--><TR><TD><A HREF="$view_run_url?run=$runData{'previousRun'}">Previous</A></TD></TR>
<!--INPUT--><TR><TD><A HREF="$ENV{'SCRIPT_NAME'}?$ENV{'QUERY_STRING'}&printable">Printable</A></TD></TR>
<!--INPUT--><TR><TD><A HREF="$view_current_url">Current Run</A></TD></TR>
<!--INPUT--><TR><TD><A HREF="$view_pass1_url?run=$run">Pass1 Plots</A></TD></TR>
<!--INPUT--><TR><TD><A HREF="$home_url">Elog Home</A></TD></TR>
<!--INPUT--><TR><TD><A HREF="$view_day_url?year=$year&month=$month&day=$day">This day</A></TD></TR>
<!--INPUT--><TR><TD><A HREF="$view_run_url?run=$totalName&next">Next</A></TD></TR>
  </TABLE>
  </TD>
  </TR>

  <TR>
  <TD COLSPAN="4"></TD>
  </TR>

  <TR>
  <TD>Run # $runData{"runNumber"}</TD>
  <TD>Start: $runData{"runStartTime"}</TD>
  <TD>End: $runData{"runEndTime"}</TD>
  <TD></TD>
  </TR>

  <TR>
  <TD> Date: $time_prefix </TD>
  <TD> Experimenters: $temp </TD>
  <TD> Total events: $runData{"eventnumber"}</TD>
  <TD></TD>
  </TR>

  <TR>
  <TD>Run type: $runData{'runtype'}</TD>
  <TD>Sequence: $runData{'runsequence'}</TD>
  <TD>Last auto-update: $runData{'lastUpdate'}</TD>
  <TD></TD>
  </TR>

  <TR>
  <TD>Live time: $runData{'liveTime_percent'} \%</TD>
  <TD></TD>
  <TD></TD>
  <TD></TD>
  </TR>
</TABLE>

<HR>
<FONT SIZE=+1><B>Before Start Run:</B></FONT>
EOH

print <<"EOH" if $firstrun;
<FORM METHOD=POST ACTION="$data_actionUrl/$totalName/firstrun">
<!-- INPUT
<BR>
Run not yet started.
<BR>
 INPUT -->
HV on? <INPUT TYPE="CHECKBOX" NAME="HVon" VALUE="yes"> (start DR HV sheet)<BR>
SV biased? <INPUT TYPE="CHECKBOX" NAME="SVon" VALUE="yes"><BR>
Destination Disk (see Storage Mgr GUI): <INPUT TYPE=TEXT SIZE=10 NAME="destDisk">
<BR>
<INPUT TYPE="HIDDEN" NAME="runShStarted" VALUE=1>
<INPUT TYPE="RESET" VALUE="Clear"><INPUT TYPE="SUBMIT" VALUE="Submit">
</FORM>
<BR>
EOH

print <<"EOH" unless $firstrun;
<TABLE WIDTH=100% BORDER=0>
<TR>
<TD>HV on? $runData{'HVon'}</TD>
<TD>SV biased? $runData{'SVon'}</TD>
<TD>Destination Disk: $runData{'destDisk'}</TD>
</TR>
</TABLE>
<BR>
EOH

    unless ($firstrun) {
	print <<"EOH";
<HR>
<FONT SIZE=+1><B>During Run:</B></FONT>
<BR><BR>
<FORM METHOD=POST ACTION="$data_actionUrl/$totalName">
<TABLE WIDTH=100% BORDER=1>
<TR>
<TD>
<TABLE WIDTH=100% BORDER=0>
<TR>
<TH>Time</TH>
<TH COLSPAN=2>Events</TH>
<TH COLSPAN=2>Trig Rate</TH>
<TH COLSPAN=2>Deadtime<BR>(DFC)</TH>
<TH>L_inst<BR>x10^30</TH>
<TH COLSPAN=3>I-/I+<BR>(mA)</TH>
<TH>Hybrids<BR>on (Si3)</TH>
<TH COLSPAN=5>
<TABLE WIDTH=100%>
<TR><TH COLSPAN=5 ALIGN=CENTER>Systems checked:</TH></TR>
<TR><TD ALIGN=CENTER>HV</TD><TD>/</TD><TD ALIGN=CENTER>Pass1</TD><TD>/</TD><TD ALIGN=CENTER>Event</TD></TR>
</TABLE>
</TH>
EOH

    $data_templ = <<'EOT';
<TR>
<TD ALIGN=CENTER>$entryTime</TD>
<TD ALIGN=RIGHT>$evts</TD><TD ALIGN=LEFT>K</TD>
<TD ALIGN=RIGHT>$trigRate</TD><TD ALIGN=LEFT>Hz</TD>
<TD ALIGN=RIGHT>$deadtime</TD><TD ALIGN=LEFT>%</TD>
<TD ALIGN=CENTER>$Linst</TD>
<TD ALIGN=RIGHT>$i_minus</TD><TD ALIGN=CENTER>/</TD><TD ALIGN=LEFT>$i_plus</TD>
<TD ALIGN=CENTER>$si3on</TD>
<TD ALIGN=CENTER>$hv_chk</TD><TD>/</TD>
<TD ALIGN=CENTER>$pass1_chk</TD><TD>/</TD>
<TD ALIGN=CENTER>$evtdisp_chk</TD>
EOT

@vars = ();
while ($data_templ =~ /(\$\w+)/g) {
    push(@vars,$1);
}

seek(NOTES,0,0);

$/ = $record_sep;
$extras = 0;
while (<NOTES>) {
    chop;
    next unless /^HEAD$field_sep([^\0]+)/;
    $title = ($1 =~ s/\n/<BR>/g);
    $extras++;
    print "<TH>$title</TH>\n";
    $data_templ = $data_templ . "<TD ALIGN=RIGHT>\$extra$extras</TD>\n";
    push(@vars, "\$extra$extras");
}
print "</TR>\n";

# change if structure changes
$total_cols = 17+$extras;

$varList = join(",",@vars);

$rowCommands = <<"EOT";
($varList) = split("\$field_sep", \$1);
print <<"EOOL";
$data_templ</TR>
EOOL
EOT

seek(NOTES,0,0); # back to the beginning...

while(<NOTES>) {
    chop;
    next unless /^DATA$field_sep([^\0]+)/;
    eval $rowCommands;
}

# action URL moved up a bit
print <<"EOH" unless $runOver;
<TR>
<INPUT TYPE="HIDDEN" NAME="lastShifter" VALUE="$last_shifter">
<TD ALIGN=CENTER> </TD>
<TD ALIGN=RIGHT><INPUT TYPE="TEXT" SIZE=3 NAME="evnts"></TD><TD ALIGN=LEFT>K</TD>
<TD ALIGN=RIGHT><INPUT TYPE="TEXT" SIZE=2 NAME="trig"></TD><TD ALIGN=LEFT>Hz</TD>
<TD ALIGN=RIGHT><INPUT TYPE="TEXT" SIZE=5 NAME="deadtime"></TD><TD ALIGN=LEFT>%</TD>
<TD ALIGN=CENTER><INPUT TYPE="TEXT" SIZE=5 NAME="instL"></TD>
<TD ALIGN=RIGHT><INPUT TYPE="TEXT" SIZE=3 NAME="Ineg"></TD><TD ALIGN=CENTER>/</TD><TD ALIGN=LEFT><INPUT TYPE="TEXT" SIZE=3 NAME="Ipos"></TD>
<TD ALIGN=CENTER><INPUT TYPE="TEXT" SIZE=3 NAME="si3on"></TD>
<TD ALIGN=CENTER><INPUT TYPE="CHECKBOX" NAME="hv_chk" VALUE="yes" ></TD>
<TD>/</TD>
<TD ALIGN=CENTER><INPUT TYPE="CHECKBOX" NAME="pass1_chk" VALUE="yes"></TD>
<TD>/</TD>
<TD ALIGN=CENTER><INPUT TYPE="CHECKBOX" NAME="evtdisp_chk" VALUE="yes"></TD>
</TR>
<TR>
<TD ALIGN=RIGHT COLSPAN=$total_cols>
<INPUT TYPE="RESET" VALUE="Clear"><INPUT TYPE="SUBMIT" VALUE="Enter">
</TD>
</TR>
EOH

print <<"EOH";
</TABLE>
</TD>
</TR>
</TABLE>
</FORM>
<BR>
EOH
}

print <<"EOH";
<HR>
<FONT SIZE=+1><B>HV trips:</B></FONT>
<TABLE WIDTH=100%>
<TR>
<TH WIDTH=10%>Device</TH><TH WIDTH=10%> </TH><TH ALIGN=LEFT>Channels</TH>
</TR>
EOH

# do the file dance some more
seek(NOTES,0,0);

while (<NOTES>) {
    chop;
    next unless /^TRIP$field_sep([^\0]+)/;
    ($trip_src, $trip_time, $trip_comment) = split($field_sep,$1);
    $trip_time =~ s/(\d+):(\d+):(\d+)(.*)/\1:\2:\3/;
    print <<"EOOL";
<TR>
<TD ALIGN=CENTER>$trip_src</TD>
<TD ALIGN=CENTER>$trip_time</TD>
<TD ALIGN=LEFT COLSPAN=2>$trip_comment</TD>
</TR>
EOOL
}

print <<"EOH" unless $runOver;
<FORM METHOD=POST ACTION="$trip_actionUrl/$totalName">
<TR>
<INPUT TYPE="HIDDEN" NAME="lastShifter" VALUE="$last_shifter">
<TD ALIGN=CENTER><INPUT TYPE="TEXT" NAME="tripDev" SIZE=8></TD>
<TD ALIGN=CENTER></TD>
<TD ALIGN=LEFT><INPUT TYPE="TEXT" NAME="tripCom" SIZE=40></TD>
</TR>
<TR>
<TD COLSPAN=3 ALIGN=RIGHT>
<INPUT TYPE="RESET" VALUE="Clear"><INPUT TYPE=SUBMIT VALUE="Enter">
</TD>
</TR>
</FORM>
EOH

print <<"EOH";
</TABLE>
EOH

print <<"EOH" unless $runOver;
<HR>
<FONT SIZE=+1><B>After End Run:</B></FONT><BR><BR>
<FORM METHOD=POST ACTION="$endRun_actionUrl/$totalName">
<INPUT TYPE="HIDDEN" NAME="lastShifter" VALUE="$last_shifter">
<INPUT TYPE="HIDDEN" NAME="runShFinished" VALUE="1">
Run ends because: 
<INPUT TYPE="RADIO" NAME="endRun" VALUE="end of fill" CHECKED>End of fill
<INPUT TYPE="RADIO" NAME="endRun" VALUE="beam dump">Beam loss or dump
<INPUT TYPE="RADIO" NAME="endRun" VALUE="DAQ/crate trouble">DAQ/crate trouble
<INPUT TYPE="RADIO" NAME="endRun" VALUE="Other">Other
<BR>Comments: <INPUT TYPE="TEXT" NAME="endComment" SIZE=30>
<BR><BR>
Event display checked: 
<INPUT TYPE="CHECKBOX" NAME="evtDisplay" VALUE="yes"><BR>
Pass 1 looks okay? 
<INPUT TYPE="CHECKBOX" NAME="pass1ok" VALUE="yes"><BR>
Pass 1 histograms printed? 
<INPUT TYPE="CHECKBOX" NAME="pass1print" VALUE="yes"><BR>
<BR>
<INPUT TYPE="RESET" VALUE="Clear"><INPUT TYPE=SUBMIT VALUE="Submit"></TD></TR>
</FORM>
EOH

$runData{'endComment'} = "Comment: $runData{'endComment'}" if $runData{'endComment'};
print <<"EOH" if ($runData{'runShFinished'} && $runData{'runEnded'});
<HR>
<FONT SIZE=+1><B>After End Run:</B></FONT><BR><BR>
<INPUT TYPE="HIDDEN" NAME="runShFinished" VALUE="1">
Run ends because of $runData{'endRun'}<BR>
$runData{'endComment'}
<BR>
Event display checked: $runData{'evtDisplay'}
<BR><BR>
Pass 1 looks okay? $runData{'pass1ok'}
<BR><BR>
Pass 1 histograms printed? $runData{'pass1print'}
EOH

print <<"EOH";
<BR><BR>
<TABLE WIDTH=100%>
<TR>
<TD ALIGN=LEFT>
<!--INPUT--><A HREF="$view_run_url?run=$runData{'previousRun'}">Previous run</A>
<!-- INPUT
Previous: $runData{'previousRun'}
 INPUT -->
</TD>
<TD ALIGN=RIGHT>
<!--INPUT--><A HREF="$view_run_url?run=$totalName&next">Next run</A>
<!-- INPUT
Next: $runData{'nextRun'}
 INPUT -->
</TD>
</TR>
</TABLE>
<HR>
<FONT SIZE=+1><B>Comments:</B></FONT><BR>
<TABLE WIDTH=100%>
<TR>
<TH>Initials</TH>
<TH>Comment</TH>
EOH

print <<"EOH";
<FORM METHOD=POST ACTION="$comment_actionUrl/$totalName">
<TR>
<INPUT TYPE="HIDDEN" NAME="lastShifter" VALUE="$last_shifter">
<TD VALIGN=TOP ALIGN=CENTER>
<SELECT NAME="shifterDefaults">
EOH

# grok the shifter list for options
$shifters = $runData{"shifterlist"};
$shifters .= ", $last_shifter" unless ($shifters =~ /$last_shifter,?/);
@shiftList = split(/,\s+/,$shifters);
foreach (@shiftList) {
    print "<OPTION";
    print " SELECTED" if (defined $last_shifter) && /$last_shifter/o;
    print ">$_\n";
}

print <<"EOH";
</SELECT>
<BR>
<INPUT TYPE="TEXT" SIZE=6 NAME="initials"></TD>
<TD ALIGN=CENTER>
<TABLE>
<TR><TD COLSPAN=2><TEXTAREA ROWS=6 COLS=70 NAME="commentBody"></TEXTAREA></TD><TD></TD></TR>
<TR><TD></TD>
<TD ALIGN=RIGHT>
<INPUT TYPE="RESET" VALUE="Clear"><INPUT TYPE=SUBMIT VALUE="Enter"></TD><TD></TD></TR>
</TABLE>
</TD>
</TR>
</FORM>
EOH

seek(NOTES,0,0);

while(<NOTES>) {
    chop;
    next unless /^CMNT$field_sep([^\0]+)/;
    ($initials, $time, $com) = split($field_sep, $1);
    $name_str = &get_name($initials);
    $com =~ s/\n/<BR>/g;
    $time =~ s/(\d+):(\d+):(\d+)(.*)/\1:\2:\3/ unless $runData{'runShFinished'};
    print <<"EOOL";
<TR>
<TD ALIGN=CENTER>$name_str</TD>
<TD ALIGN=LEFT>$time</TD>
</TR>
<TR>
<TD></TD>
<TD>$com</TD>
</TR>
EOOL
}


# For e-mail form at bottom b/c we don't know what the heck is going on anywhere else
$shifters =~ s/\s//g;
$shifters =~ s/,/ /g;

# Form that allows sending all comments to a detector exprt
print <<"EOH";
</TABLE>

<BR>
<TABLE WIDTH=100%>
<TR>
<!--INPUT--><TD ALIGN=LEFT><A HREF="$view_run_url?run=$runData{'previousRun'}">Previous run</A></TD>
<!--INPUT--><TD ALIGN=RIGHT><A HREF="$view_run_url?run=$totalName&next">Next run</A></TD>
</TR>
</TABLE>

<HR>
<FORM ACTION="http://lnson3.lns.cornell.edu/rbin/runsheet/sendComment.pl" METHOD="GET">
<!--INPUT--><P><FONT SIZE="+1" COLOR="green">Mail all comments already entered on this page to a detector expert:</FONT> &nbsp;
<SELECT NAME="mailTo" SIZE="1">
  <OPTION>RunMgmt <OPTION>Shifters <OPTION>MU <OPTION>CC <OPTION>RICH 
  <OPTION>DR <OPTION>Si <OPTION>DAQ <OPTION>Trigger <OPTION>HV <OPTION>Gas 
  <OPTION>Cooling <OPTION> DRCA <OPTION>Energy <OPTION>Kreinick <OPTION> Scan
  <OPTION> Calibration
</SELECT>
<INPUT TYPE="submit" VALUE="Send">
<INPUT TYPE="hidden" NAME="run" VALUE="$totalName">
<INPUT TYPE="hidden" NAME="shifterList" VALUE="$shifters">
<!--INPUT--><P>This will <I>automatically</I> generate an entry in the runsheet that mail was sent to the selected expert(s).
</FORM>

</BODY>
</HTML>
EOH

flock(NOTES,8); # release lock
close(NOTES);

exit(0);

# eventually, use to automatically look up shifter intials
sub get_name {
    local($initials) = @_;
    return $initials;
}

sub cook_data {
    $firstrun = 1 unless ($runData{'runShStarted'} && $runData{'runStarted'});
    $runOver = ($runData{'runShFinished'} && $runData{'runEnded'});
    $runData{'previousRun'} = 'noSuchRun' unless $runData{'previousRun'};
    $runData{'nextRun'} = 'noSuchRun' unless $runData{'nextRun'};
    $runData{'energy_GeV'} = $runData{'energy_keV'};
    $runData{'integLum_invnb'} = 'unknown' unless $runData{'integLum_invnb'};
    $runData{'liveTime_percent'} = 'pending' unless $runData{'runEnded'};
    $runData{'eventnumber'} = int($runData{'eventnumber'});
    $runData{'eventnumber'} = 'pending' if ($runData{'runStarted'} && !$runData{'runEnded'});
    $runData{'eventnumber'} = 'waiting' unless $runData{'runStarted'};
    $runData{'runStartTime'} = &format_time_short($runData{'runStartTime'});
    $runData{'runStartTime'} = 'waiting' unless $runData{'runStarted'};
    $runData{'runEndTime'} = &format_time_short($runData{'runEndTime'});
    $runData{'runEndTime'} = 'running' unless $runData{'runEnded'};
    $runData{'runEndTime'} = 'waiting' unless $runData{'runStarted'};
    if ($runData{'runEnded'}) {
	$runData{'lastUpdate'} = &format_time_long($runData{'lastUpdate'});
    } else {
	$runData{'lastUpdate'} = &format_time_short($runData{'lastUpdate'});
    }
}

sub format_time_short {
    local($timeIn) = @_;
    local($tz, $date, $time, $secs) = ($timeIn =~ /^([^:]+):\s+(\d+\/\d+\/\d+)\s+(\d+:\d+:\d+).(\d+)/);
    return "$time";
}

sub format_time_long {
    local($timeIn) = @_;
    local($tz, $date, $time, $secs) = ($timeIn =~ /^([^:]+):\s+(\d+\/\d+\/\d+)\s+(\d+:\d+:\d+).(\d+)/);
    return "$time $tz, $date";
} 

sub do_noSuchRun {
    print <<"EOH";
Content-type: text/html
Pragma: no-cache

<HTML>
<HEAD><TITLE>No such run!</TITLE></HEAD>
<BODY>
The run you have requested does not exist.<BR>
Please return to the previous page.
</BODY>
</HTML>
EOH
    exit(0);


}

sub do_waiting {
    # print out a waiting page, and set a 30 second refresh
    $url = "$view_run_url?run=$totalName&next";
    print "Pragma: no-cache\n";
    print "Refresh: 30; URL=$url\n\n";
    print "Waiting for the next run to begin - this page will reload until the next run starts.\n\n";
    print "$runData{'runEnded'}\n\n";
    print "$runData{'nextRun'}\n";
    exit(0);
}

sub do_goNext {
    # handle redirects - send the user to holding if the run is over
    if ($runData{'runEnded'} && ($runData{'nextRun'} =~ /noSuchRun/)) {
	&do_waiting();
    }
    $url = "$view_run_url?run=$runData{'nextRun'}";
    # redirect the browser to the page pointed to in nextRun
    print "Status: 302 Redirect\n";
    print "Location: $url\n";
    print "Pragma: no-cache\n\n";

    exit(0);
}
