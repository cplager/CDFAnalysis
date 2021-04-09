#!/usr/bin/perl

################################
# File: doEndRun.pl
# Author: mdj
# Revision history:
#    v1.3(10/31/2000): Updated redirector code
#    v1.2(09/30/2000): Format changes, linking in beginRun.pl
#    v1.1(08/27/2000): Format changes, link to previous run
#    v1.0(07/26/2000): File created
#
# Description:
#  Adds the end-of-run data to the notes file
#  Sends the browser back to displayRun for the next run
# Input:
#  Expects to be passed the run number as extra path.
#  The rest by POST:
#   lastShifter hidden field
#   end-run checkboxen and data
# Output:
#  redirect to displayRun, next run
# Effects:
#  Adds data to file.
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

%boolargs = ('pass1print', 'no', 'pass1ok', 'no', 'evtDisplay', 'no');

foreach (@argsIn) {
    ($var, $val) = split('=');
    $_ = $val;
    s/\+/ /g;
    s/\%([\dA-Fa-f]{2})/pack("C",hex($1))/eg;
    s/\x0D\x0A/\n/g;
    $args{$var} = $_;
}

# transform boolean arguments
foreach (keys %boolargs) {
    $args{$_} = $boolargs{$_} unless defined $args{$_};
}

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

$lastShifter = $args{'lastShifter'};
$args{'lastShifter'} = undef;

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
