#!/usr/local/bin/perl 

require "config.pl";
require "subroutines.pl";

print <<"EOH";
Content-type: text/html

<HTML>
<HEAD>
EOH
	;

# get the parameters
$query = $ENV{'QUERY_STRING'};
$query =~ s/\s+/_/g;
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
	#print "<b>Part:</b> $_<br>\n";
	if (/^([\w\.]+)=/) {
		$variables{$1} = $';
		#print "<b>var</b> $1 : $variables{$1}<br>\n";
	}
}

$detector = $variables{detector};
$fields   = $variables{fields};
$title    = $variables{title};
$detector =~ tr/A-Z/a-z/;
$detector = ucfirst $detector;
$title = ucfirst $title;
$niceTitle = $title;
$niceTitle =~ s/_/ /g;
@names = split '-', $fields;

foreach (@names) {
	if (/^([\w\.]+):/) {
		$name = $1;
		$unit = $';
	} elsif (/^([\w\.]+)/) {
		$name = $1;
		$unit = "";
	} else {
		# this doesn't match, skip this one
		next;
	}
	$units{$name} = $unit;
	push @ordered, $name;
}


$string = "";
foreach (@ordered) {
	#print "thing $_<br>variable $variables{$_}<br>\n";
	if ($string) {
		$string .= $newline;
	}
	$string .= "$_$field_sep$variables{$_}$field_sep$units{$_}";
}
print << "EOH";
You have just entered the following reading into the Hardware log:
<br><br><br>
<TABLE>
EOH
	;

($date, $shortName) = getDate();
$filename = "$data_dir/$shortName";
$reading = "$detector#$title#$date#$string";

displayWholeReading ($reading);
#displayReading($string);

print "</TABLE>\n";
@list = keys %variables;
foreach (@parts) {
#	print "$_ <br>\n";
}

open(NOTES,">>$filename");
flock(NOTES,2); # exclusive lock
print NOTES "Reading#$reading\n";
flock(NOTES,8); # release lock
close(NOTES);
