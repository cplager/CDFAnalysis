###############
# subroutines #
###############

sub displayComment {
	my ($string) = @_;
	my ($detector, $who, $time, $comment) = split /#/, $string;
	$comment =~ s/$numSign/#/g; # put #'s back in
	$comment =~ s|$newline|<br>|g; # put in \n's
	print << "EOH";
<TR>
<TD ALIGN=CENTER><font color="red">$detector</font></TD>
<TD ALIGN=LEFT> $who <font color="darkgreen">$time :</font></TD>
</TR>
<TR>
<TD></TD>
<TD>$comment<BR><BR></TD>
</TR>
EOH
	;
}

sub displayWholeReading {
	my ($string) = @_;
	#my $len = length ($string);
	#print "whole ($len) $string<br>\n";
	my @parts = split /#/, $string;
	my $detector = shift @parts;
	my $title = shift @parts;
	$title =~ s/_/ /g;
	my $date = shift @parts;
	$string = join "#", @parts;
	print "<TR><TD ALIGN=CENTER><FONT COLOR=\"RED\">$detector</FONT></TD>".
		"<TD>$title  <FONT COLOR=\"DARKGREEN\">$date:</FONT></TD></TR>\n";
	print "<TR><TD></TD><TD>\n";
	displayReading($string);
	print "</TD><TR></TR><TR></TR><TR></TR><br>\n";
}


sub displayReading {
	my ($string) = @_;
	#my $len = length ($string);
	#print "string ($len) $string<br>\n";
	my @parts = split /$newline/, $string;
	my $first = "true";
	my @title = ();
	my @values = ();
	foreach (@parts) {
		if ($first) {
			$first = "";
		} else {
			#print "&nbsp &nbsp &nbsp";
		}
		my ($name, $var, $units) = split /$field_sep/;
		$name =~ s/_/ /g;
		if ($units =~ /\w/) {
			push @title, "$name ($units)";
		} else {
			push @title, $name;
		}
		push @values, $var;
	} # loop over every entry
	print "<TABLE BORDER=\"1\" CELLPADDING=\"3\">\n<TR>\n";
	foreach (@title) {
		print "<TD ALIGN=\"CENTER\">$_</TD>";
	} # foreach title
	print "\n</TR><TR>\n";
	foreach (@values) {
		print "<TD ALIGN=\"CENTER\">$_</TD>";
	} # foreach values
	print "\n</TR></TABLE>\n";
}

sub formatComment {
	my ($detector, $who, $time, $comment) = @_;
	$who =~ s/#//g;
	$comment =~ s|\n|$newline|g;
	$comment =~ s/#/$numSign/g;
	return "$detector#$who#$time#$comment";
}

sub compDate {
	# return one if L <= R
	my ($Lmonth, $Lday, $Lyear, $Rmonth, $Rday, $Ryear) = @_;
	# check year first
	if ($Lyear > $Ryear) {
		# L > R
		return 0;
	}
	if ($Lyear < $Ryear) {
		# L < R
		return 1;
	}
	# check month next
	if ($Lmonth > $Rmonth) {
		# L > R
		return 0;
	}
	if ($Lmonth < $Rmonth) {
		# L < R
		return 1;
	}
	if ($Lday > $Rday) {
		# L > R
		return 0;
	}
	return 1;
}

sub getDate() {
	my $time_time = `date +%H:%M:%S`;
	my ($time_tz) = (`date` =~ /([A-Z]{3})/);
	my $time_date = `date +%m/%d/%Y`;
	chop($time_time);
	chop($time_date);
	$date = "$time_time $time_tz $time_date";
	my $filename;
	if ($time_date =~ m|(\d{2})/\d{2}/(\d{4})|) {
		$filename = "$1_$2.dat";
	} else {
		$filename = "default.dat";
	}
	return ($date, $filename);
}


sub compDate {
	# return 1 if L <= R else 0
	my ($Lmonth, $Lday, $Lyear, $Rmonth, $Rday, $Ryear) = @_;
	# check year first
	if ($Lyear > $Ryear) {
		# L > R
		return 0;
	}
	if ($Lyear < $Ryear) {
		# L < R
		return 1;
	}
	# check month next
	if ($Lmonth > $Rmonth) {
		# L > R
		return 0;
	}
	if ($Lmonth < $Rmonth) {
		# L < R
		return 1;
	}
	if ($Lday > $Rday) {
		# L > R
		return 0;
	}
	return 1;
}

1
