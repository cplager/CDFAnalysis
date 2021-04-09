#!/usr/local/bin/perl -w


# get the parameters
$query = $ENV{'QUERY_STRING'};
@parts = split '&', $query;
foreach $part (@parts) {
	$not_done = "true";
	$part =~ s/\+/ /g;
	while ($not_done) {		
		if ($part =~ /\%([\dA-F]{2})/) {
			$num = hex($1);
			$char = chr($num);
			$part = $`.$char.$';
		} else {
			$not_done = "";
		}
	}
    $part =~ s/\;.*//;
}

print "Content-type: text/html\n\n";

print "<HTML>";

foreach $part (@parts) {
    print "$part<br>\n";
}

print "</html>\n";

