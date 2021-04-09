#!/usr/bin/perl

################################
# File: writeComment.pl
################################

require "config.pl";
require "subroutines.pl";

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

$which = "all";
$search = "";
$commentOnly = $readingOnly = "";
foreach (@parts) {
	if (/both=/i) {
		$both = $';
	}
	if (/search=/i) {
		$search = $';
		next;
	}
	if (/detector=(\w+)/) {
		$which = $1;
		next;
	}
	if (/select=/i) {
		$select = $';
		next;
	}
	if (/startMonth=(\d+)/i) {
		$startMonth = $1;
		next;
	}
	if (/startDay=(\d+)/i) {
		$startDay = $1;
		next;
	}
	if (/startYear=(\d+)/i) {
		$startYear = $1;
		next;
	}
	if (/endMonth=(\d+)/i) {
		$endMonth = $1;
		next;
	}
	if (/endDay=(\d+)/i) {
		$endDay = $1;
		next;
	}
	if (/endYear=(\d+)/i) {
		$endYear = $1;
		next;
	}
}

if ($select =~ /beginning/i) {
	$beginning = "true";
	$startMonth = $endMonth = $firstMonth;
	$startDay = $endDay = $firstDay;
	$startYear = $endYear = $firstYear;
	--$endYear;
	$select = "date";
}

if ($both =~ /both/i) {
	$commentOnly = $readingOnly = "";
} elsif ($both =~ /comment/i) {
	$commentOnly = "true";
	$readingOnly = "";
} elsif ($both =~ /reading/i) {
	$commentOnly = "";
	$readingOnly = "true";
}



if ($search) {
	$display = "Searching for /$search/i:";
} else { 
	$display = "Comments:";
}

# move around defaulted fields
($today, $short) = getDate();
$filename = "$data_dir/$short";

print << "EOH";
Content-type: text/html

<HTML>
<HEAD>
<TITLE>Reading Hardware log</TITLE>
</head>
 <br> 
<h4>$display</h4>
<TABLE>
EOH
	;

if ($select =~ /date/i) {
	@files = generateDateList ($startMonth, $startDay, $startYear,
							   $endMonth,   $endDay, $endYear,
							   $date);
	if (! compDate ($startMonth, $startDay, $startYear, 
					$endMonth, $endDay, $endYear) ) {
		# enddate is before startdate
		$date =~ m|(\d{2})/(\d{2})/(\d{4})|;
		$endMonth = $1;
		$endDay = $2;
		$endYear = $3;
	}
} else {
    @files = ($short);
}


foreach (@files) {
	open (NOTES, "<$data_dir/$_");
	#print "File: $_<br>\n";
	while (<NOTES>) {
		if (/^(\w+)#/) {
			$string = $';
			$type = $1;
		} else {
			next;
		}
		($detector, $who, $date) = split /#/, $string;
		# check date
		if ($select =~ /date/i) {
			if ($date =~ m|(\d{2})/(\d{2})/(\d{4})|) {
				$month = $1;
				$day = $2;
				$year = $3;
			} else {
				# somethings wrong with this date
				next;
			}
			# is this date > enddate
			if (! compDate ($month, $day, $year,
							$endMonth, $endDay, $endYear) ) {
				# this is after the end date
				#print "$month/$day/$year <> ".
				#"$endDay/$endMonth/$endYear<br>\n";
				next;
			}
			# is this startdate > date 
			if (! compDate ($startMonth, $startDay, $startYear,
							$month, $day, $year) ) {
				# this is after the end date
				#print "$month/$day/$year <> ".
				#"$startDay/$startMonth/$startYear<br>\n";
				next;
			}
		}
		# check detector
		# if we don't want all and this one isn't what we want
		# skip it
		if (($which !~ /all/i) && ($which !~ /^$detector$/i)) {
			# this ain't what we want
			next;
		}
		if ($type =~ /comment/i) {
			# this line is a comment
			next if ($readingOnly);
			if ($search) {
				$string =~ /#([^#]+)$/;
				$before = $`;
				$comment = $1;
				if ($comment =~ /$search/i) {
					# it's here
					$comment =~ s|($search)|<font color=\"red\">$1</font>|ig;
					$string = "$before#$comment";
				} else {
					# it's not here
					next;
				}
			}
			displayComment($string);
		} elsif ($type =~ /reading/i) {
			# this is a reading
			next if ($commentOnly);
			next if ($search); # don't search readings
			displayWholeReading ($string);
		}
	}
	close (NOTES);
} # foreach files
print "</TABLE></html>\n";

sub generateDateList {
	my ($startMonth, $startDay, $startYear, 
		$endMonth, $endDay, $endYear, $date) = @_;
	# is startdate > enddate
	if (! compDate ($startMonth, $startDay, $startYear, 
					$endMonth, $endDay, $endYear) ) {
		# enddate is before startdate
		$date =~ m|(\d{2})/(\d{2})/(\d{4})|;
		$endMonth = $1;
		$endDay = $2;
		$endYear = $3;
	}
	my $year, $month;
	my $start, $stop;
	my @retval = ();
	for ($year = $startYear; $year <= $endYear; ++$year) {
		# if this is the first year, start at startMonth,
		# otherwise start at January (1)
		if ($year == $startYear) {
			$start = $startMonth;
		} else {
			$start = 1;
		}
		# if this is the last year, end at endMonth,
		# otherwise end at December (12)
		if ($year == $endYear) {
			$stop = $endMonth;
		} else {
			$stop = 12;
		}
		for ($month = $start; $month <= $stop; ++$month) {
			push @retval, sprintf ("%02d_%04d.dat", $month, $year);
		} # for $month
	} # for year
	return @retval;
}
