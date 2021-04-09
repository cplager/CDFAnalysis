#!/usr/local/bin/perl -w
require "config.pl";

foreach (@offlineDirs) {
	$path = "$offlinePass1_dirname/$_";
	@files = glob "$path/*ps*";
	foreach (@files) {
		if (/(\d{6})/) {
			$found{$1} = $1;
		}
	} # foreach @files
} # foreach offlineDirs

print <<"EOH";
Content-type: text/html

<HTML>
<HEAD>
<TITLE>Pass1 plots for run $run</TITLE>
</head>
<h1>Available Offline Pass1 files:</h1>
<br>
EOH
	;

foreach (reverse sort {$a <=> $b} keys %found) {
	print "<A HREF=\"$view_pass1_url?pass1=Offline+Pass1&run=$_\">$_</a><br>\n";
}

print "</html>\n";
