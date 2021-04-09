#!/usr/local/bin/perl -w

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

print <<"EOH";
Content-type: text/html

<HTML>
<HEAD>
<TITLE>Mail to </TITLE>
EOH
	;
foreach (@parts) {
	print "$_ <br>\n";
}
