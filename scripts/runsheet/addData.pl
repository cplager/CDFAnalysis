#!/usr/bin/perl

################################
# File: addData.pl
# Author: mdj
# Revision history:
#    v1.3(09/30/2000): split before/during run
#    v1.2(08/27/2000): Changed/added fields
#    v1.1(07/26/2000): Updated to receive new fields
#    v1.0(12/20/1999): File created
#
# Description:
#  Adds a data line to the .notes file
#  Sends the browser back to displayRun
# Input:
#  Expects to be passed the run number as extra path.
#  The rest by POST:
#   lastShifter hidden field
#   data fields
#   pre-run checkboxen
# Output:
#  redirect to displayRun (without first view flag)
# Effects:
#  Adds data line to file.
# Notes:
#  
################################

require "config.pl";

($fileTag, $firstrun) = ($ENV{'PATH_INFO'} =~ /(\d{6}-\d+-\d+-\d+)(.*)/);

($currentRun = readlink("$data_dir/$current_name.$notes_ext")) || warn "$!\n";
($currentRun) = ($currentRun =~ /$data_dir\/(.*)\.notes/);

# transform POST arguments
$postArgs = '';

while(<>) {
    $postArgs .= $_;
}

@argsIn = split('&', $postArgs);
%args = ();

%boolargs = ('hv_chk', 'no', 'pass1_chk', 'no', 'evtdisp_chk', 'no');
%boolargs = ('SVon', 'no', 'HVon', 'no') if $firstrun;

foreach (@argsIn) {
    ($var, $val) = split('=');
    $_ = $val;
    s/\+/ /g;
    s/\%([\dA-Fa-f]{2})/pack("C",hex($1))/eg;
    s/\x0D\x0A/\n/g;
    $args{$var} = $_;
}

unless ($fileTag == $currentRun) {
    print <<"EOH";
Content-type: text/html

<HTML>
<HEAD><TITLE>Entry not allowed - run has ended</TITLE></HEAD>
<BODY>
<H1>The run has ended.</H1>
'During Run' data cannot be added after a run has been ended.<BR>
Go to the <A HREF="$view_run_url?run=$runNumber&next">next run</A>
</BODY>
</HTML>     
EOH
    exit(0);
}

# transform boolean arguments
foreach (keys %boolargs) {
    $args{$_} = $boolargs{$_} unless defined $args{$_};
}

# transform extra fields
@extras = grep(/^extra\d+/,keys %args);
$extra_str = '';
foreach (@extras) {
    $extra_str = "$extra_str$field_sep$args{$_}";
    delete $args{$_};
}

$args{'time'} = `date +%H:%M`;

$notesFile = "";
opendir(DATADIR, $data_dir);
while( $file = readdir(DATADIR) ) {

    next if /^\.+/;
    next unless $file =~ /^$fileTag\.$notes_ext/;
    $notesFile = $data_dir."/".$file ;
}
close DATADIR;

if( $notesFile eq "" ){

  open( ARCHIVE, $archive_index );
  while( <ARCHIVE> ){

    if( /\d{6}\s+\d+\s+(\S+$fileTag)/ ){

      $notesFile = $1.".".$notes_ext;
    }
  }
}

#the file doesn't exist make one

if( $notesFile eq "" ){

  $notesFile = $data_dir."/".$fileTag.".".$notes_ext;
}

# write to file

open(NOTES,">>$notesFile");
flock(NOTES,2); # exclusive lock
seek(NOTES,0,2); # go to end (just in case)
$| = 1; # no buffering

unless ($firstrun) {
    @data_items = ('time', 'evnts', 'trig', 'deadtime', 'instL', 'Ineg', 'Ipos', 'si3on', 'hv_chk', 'pass1_chk', 'evtdisp_chk');

    $out_str = "DATA$field_sep";
    foreach (@data_items) {
	$out_str .= "$args{$_}$field_sep";
	delete $args{$_};
    }
    $out_str .= "$extra_str$record_sep";
    
    print NOTES $out_str;
}

$lastShifter = $args{'lastShifter'};
delete $args{'lastShifter'};

# write the rest
foreach (keys %args) {
    print NOTES "XTRA$field_sep$_$field_sep$args{$_}$record_sep";
}

flock(NOTES,8); # release lock
close(NOTES);

# now output redirect headers
$url = "$view_run_url?run=$fileTag";
print "Status: 302 Redirect\n";
print "Pragma: no-cache\n";
print "Location: $url\n\n";
exit(0);
