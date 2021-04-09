#!/usr/local/bin/perl

require "config.pl";

$run = 0;
$query = $ENV{'QUERY_STRING'};
$query =~ s/\+/ /g;

$pass1 = "online";

if ($query =~ /pass1=([\w\s]+)/) {
	$which = $1;
}

if ($query =~ /run=(\d+)/) {
	$run = $1;
}


if ($which =~ /online pass1/i) {
	$pass1 = "online";
} elsif ($which =~ /offline pass1/i) {
	$pass1 = "offline";
} elsif ($which =~ /mini pass2/i) {
	$pass1 = "mini";
}

if ("online" eq $pass1) {
	$shortname = "Pass1Run$run\.pdf";
	$filename = "$pass1_dir/$shortname";
	$url = "$pass1_url/$shortname";
	
	if (-e $filename) {
		print "Status: 302 Redirect\n";
		print "Pragma: no-cache\n";
		print "Location: $url\n\n";
	} else {
		print <<"EOH";
Content-type: text/html

<HTML>
<HEAD>
<TITLE>Pass1 plots for run $run</TITLE>
</head>
<h1>Sorry, there are no online plots available for run $run.</h1>
Please visit the <a href="http://www.lns.cornell.edu/~cleo31/private/Pass1Plots/?M=D">Online Pass1 directory</a> for a complete list of runs with .pdf file.
<br>
</html>
EOH
	;
	} # filename doesn't exist
	exit;
}

if ("offline" eq $pass1) {
	if ($run < 100000) {
		$run = 100000;
	}
	# look at all the offline directories
	foreach (@offlineDirs) {
		$path = "$offlinePass1_dirname/$_";
		$globVar = "$path/*$run*ps*";
		@files = glob "$globVar";
		if (@files) {
			$file = shift @files;
			if ($file =~ m|([^/]+)$|) {
				$file = $1;
			}
			$url = "$offlinePass1_url/$_/$file";
			print "Status: 302 Redirect\n";
			print "Pragma: no-cache\n";
			print "Location: $url\n\n";
			exit;
		}
	}
	# couldn't find file
		print <<"EOH";
Content-type: text/html

<HTML>
<HEAD>
<TITLE>Pass1 plots for run $run</TITLE>
</head>
<h1>Sorry, there are no offline plots available for run $run.</h1>
Please visit the <a href="$view_offlinePass1List_url">Offline Pass1 list</a>.
<br>
</html>
EOH
	;

	exit;
} # offline

		print <<"EOH";
Content-type: text/html

<HTML>
<HEAD>
<TITLE>Pass1 plots for run $run</TITLE>
</head>
<h1>Sorry, there are no mini pass2  plots yet available.</h1>
</html>
EOH
	;
