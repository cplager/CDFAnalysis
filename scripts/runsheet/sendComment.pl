#!/usr/bin/perl

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

foreach (@parts) {
	#print "$_ <br>\n";
	if (/run=([\d\-]+)/i) {
		$fileTag = $1;
	}
	if (/mailto=(\w+)/i) {
		$expert = $1;
	}
	if (/shifterlist=([\w\s]+)/i) {
		$shifters = $1;
	}
}

if ($fileTag =~ /^(\d+)/) {
	$runNumber = $1;
}

# find expert list
$expertGroup = "";
foreach (@expert_groups) {
	if (/$expert/i) {
		$expertGroup = $_;
		last;
	}
}

if (! $expertGroup) {
	warn "No expert group: $expert : @expert_groups";
	$url = "$view_run_url?run=$fileTag";
	print "Status: 302 Redirect\n";
	print "Pragma: no-cache\n";
	print "Location: $url\n\n";
	exit;
}

# get shifters email addresses
open (SHIFTERS, "names.list");
while (<SHIFTERS>) {
	@parts = split /\s+/;
	$email{$parts[2]} = $parts[1];
}

@shifterList = split /\s+/, $shifters;
$addEmail = "";

foreach (@shifterList) {
	if ($email{$_}) {
		$addEmail .= $email{$_}." ";
	}
}

if ($expert =~ /shifter/i) {
	$salutation = "Dear shifters,\n\nSomebody";
} else {
	$salutation = "Dear $expert expert(s),\n\n The shifters ($shifters)";
}
$body = "$salutation thought you would be interested in the run $runNumber ($http_url$view_run_url?run=$fileTag).  
Here are the comments for this run: \n\n";

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

open (NOTES, "<$notesFile") or print "Can't open $notesFile\n";
flock(NOTES,1); # shared read lock
while (<NOTES>) {
	chomp;
	push @lines, $_;
}

$line = join "\n", @lines;

@parts = split /$record_sep/, $line;

foreach (@parts) {
	if (/^CMNT$field_sep/i) {
		($name_str, $time, $com) = split /$field_sep/, $';
		$body .= "$name_str    $time\n$com\n\n";
	} else {
		next;
	}
}

$body .= "--------------------------------------------------\n".
	"This message has been sent from the Cleo III Elog\n";

$html = $body;
$html =~ s/\n/<br>\n/g;
flock(NOTES,8); # release lock
close(NOTES);

#print $html;
$mailCmd = "/bin/mail @$expertGroup $addEmail << EOM
Subject: Run $runNumber
$body

EOM
";

`$mailCmd`;

# add comment to notes file
$time_time = `date +%H:%M:%S`;
($time_tz) = (`date` =~ /([A-Z]{3})/);
$time_date = `date +%m/%d/%Y`;
chop($time_time);
chop($time_date);
$curTime = "$time_time $time_tz $time_date";


$user = "Elog";
if ($expert =~ /shifter/i) {
	$name = "Shifters";
} else {
	$name = "$expert experts(s)";
}
#$addEmail =~ s///;
if (@$expertGroup) {
	$toWhom = "<i>@$expertGroup</i> and the shifters: <i>$addEmail</i>";
} else {
	$toWhom = "<i>$addEmail</i>";
}
#$addEmail =~ s/\@[\w\.]+\.lns\.edu//g;
$toWhom =~ s/\@\S+//g;		
$comment = "Sent $name elog comments:  $toWhom";
$out_str = <<"EOC";
CMNT$field_sep$user$field_sep$curTime$field_sep$comment
$record_sep
EOC
	;
chop($out_str); # kill bad \n

open (NOTES, ">>$notesFile");
flock(NOTES,2); # exclusive lock
#seek(NOTES,0,2); # go to end (just in case)
#$| = 1; # no buffering
print NOTES $out_str;
flock(NOTES,8); # release lock
close(NOTES);



$url = "$view_run_url?run=$fileTag";
print "Status: 302 Redirect\n";
print "Pragma: no-cache\n";
print "Location: $url\n\n";

#print <<"EOH";
#Content-type: text/html
#
#<HTML>
#<HEAD>
#<TITLE>Mail to </TITLE>
#$out_str
#EOH
#	;
