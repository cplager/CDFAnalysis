#!/usr/bin/perl

################################
# File: addComment.pl
# Author: mdj
# Revision history:
#
#    v1.0(12/19/1999): File created
#
# Description:
#  Adds a comment to the .notes file
#  Sends the browser back to displayRun
# Input:
#  Expects to be passed the run number as extra path.
#  The rest by POST:
#   lastShifter hidden field
#   shifterDefaults popup
#   initials fill-in
#   commentBody field
# Output:
#  redirect to displayRun
# Effects:
#  Adds comment to file.
# Notes:
#
################################

require "config.pl";

($fileTag) = ($ENV{'PATH_INFO'} =~ /(\d{6}-\d+-\d+-\d+)/);

# transform POST arguments
$postArgs = '';

while(<>) {
    $postArgs .= $_;
}

@argsIn = split('&', $postArgs);
%args = ();

foreach (@argsIn) {
    ($var, $val) = split('=');
    $_ = $val;
    s/\+/ /g;
    s/\%([\dA-Fa-f]{2})/pack("C",hex($1))/eg;
    s/\x0D\x0A/\n/g;
    $args{$var} = $_;
}

# move around defaulted fields

$args{'initials'} = $args{'shifterDefaults'} unless $args{'initials'};
$args{'lastShifter'} = $args{'initials'};
$time_time = `date +%H:%M:%S`;
($time_tz) = (`date` =~ /([A-Z]{3})/);
$time_date = `date +%m/%d/%Y`;
chop($time_time);
chop($time_date);
$curTime = "$time_time $time_tz $time_date";

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

$out_str = <<"EOC";
CMNT$field_sep$args{'initials'}$field_sep$curTime$field_sep$args{'commentBody'}
$record_sep
EOC

chop($out_str); # kill bad \n
print NOTES $out_str;
flock(NOTES,8); # release lock
close(NOTES);

# now output redirect headers

$url = "$view_run_url?run=$fileTag";
print "Status: 302 Redirect\n";
print "Pragma: no-cache\n";
print "Location: $url\n\n";

exit(0);
