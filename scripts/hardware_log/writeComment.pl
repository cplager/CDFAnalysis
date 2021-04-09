#!/usr/bin/perl

################################
# File: writeComment.pl
################################

require "config.pl";
require "subroutines.pl";

$elogDir = "/nfs/cleoonl/web/cleo3elog/data";
$elogFile = "$elogDir/113684-01-01-2001.notes";
#($runNumber) = ($ENV{'PATH_INFO'} =~ /([\d-]+)/);

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
$who = $args{'initials'};
($date, $shortName) = getDate();
$filename = "$data_dir/$shortName";
$comment = $args{'commentBody'};
$doElog = $args{'elog'};

if ($doElog) {
	$elogComment = $comment;
	$comment .= "\n<font color=\"green\">This comment saved in Elog.</font>";
}

if ($who) {
	# we are going to save this to a file
	$string = formatComment ($args{'Detector'}, $who, 
							 $date, $comment);
	print << "EOH";
Content-type: text/html

<HTML>
<HEAD>
<TITLE>Saving a comment</TITLE>
</head>
 <br> 
<TABLE>
You have just saved the following comment:
EOH
	;

	displayComment($string);

	print "</TABLE></html>\n";
	#save comments
	open(NOTES,">>$filename");
	flock(NOTES,2); # exclusive lock
	print NOTES "Comment#$string\n";
	flock(NOTES,8); # release lock
	close(NOTES);

	# Do we want to save this in Elog as well?
	if ($doElog) {
		# here we go...
		# find current run
		chdir $elogDir;
		open (LISTING, "ls -lt |");
		
		while (<LISTING>) {
			if (/(\d{6}-\d{2}-\d{2}-\d{4})\.dat/) {
				$elogFile = "$1.notes";
				if ($elogFile =~ /(\d{6})/) {
					$runNumber = $1;
				} else {
					$runNumber = $elogFile;
				}
				last;
			}
		}
		# Elog values
		$RS = "\x01"; # record separator
		$FS = "\x02";  # field separator
		$comment = "CMNT$FS$who$FS$date$FS<font color=\"green\">$args{Detector} detector</font>\n$elogComment\n$RS";
		open (NOTES, ">>$elogFile");
		flock (NOTES, 2); # exclusive lock
		#seek(NOTES,0,2); # go to end (just in case)
		#$| = 1; # no buffering
		print NOTES $comment;
		flock (NOTES, 8); # release lock
		close (NOTES);
		print "<br>This comment was saved to Elog to run $runNumber.<br>\n";
	}

} else {
	# user needs to give us a name
	print << "EOH";
Content-type: text/html

<HTML>
<HEAD>
<TITLE>Saving a comment.</TITLE>
</head>
	<b>You need to enter a name to save a comment.  Please hit <i>Back</i> to continue.</b><br>
EOH
	;
}

