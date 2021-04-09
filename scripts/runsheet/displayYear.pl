#!/usr/bin/perl

################################
# File: displayYear.pl
# Author: mdj
# Revision history:
# 
#    v1.0(11/25/1999): File created
#
# Description:
#  Displays a full year of links to individual days.
#  Also builds links to each month.
# Input:
#  Expects to be passed the year number by GET.
# Output:
#  HTML page of the selected year.
# Effects:
#  None.
#  
################################

require "config.pl";

# get value
$query = $ENV{'QUERY_STRING'};
($year) = ($query =~ /year=(\d+)/);

# print HTML

print "Content-type: text/html\n\n";
print <<"EOH";
<HTML>
<HEAD>
<TITLE>Calendar for $year</TITLE>
</HEAD>
<BODY>
<CENTER><H2>$year</H2></CENTER>
<TABLE WIDTH=100\% BORDER=1>
EOH

# output the table of months, 2 per row.
foreach $i (1..6) {
    print "<TR VALIGN=TOP>\n<TD WIDTH=50\%>\n";
    &print_month($i, $year);
    print "</TD>\n<TD WIDTH=50\%>\n";
    &print_month($i+6, $year);
    print "</TD>\n</TR>\n";
}

# finish off the file
print <<"EOH";
</TABLE>
</BODY>
</HTML>
EOH

# outta here! not required but good form.
exit(0);

sub print_month {
    # routine to print out months of the calendar
    # NOTE: it is highly recommended that you follow along in the HTML
    #       printed out from here; it should be a little clearer.
    #       Regardless, non-Perl addicts - beware!

    # get local vars + setup parameters
    local($m, $y) = @_;
    local($name) = $month_names[$m-1];
    local(@lines) = `$cal_prog $m $y`;
    local($ds, $dend, $fc);
    local($align) = 'CENTER';
    local($width) = int(100/7);
    local($TD) = "<TD ALIGN=$align WIDTH=$width\%>";

    # trim the output of cal
    shift(@lines);
    chop($lines[0]);

    # these two lines chop off excess whitespace (first)
    # and transform the first line of cal into (almost) valid HTML.
    $lines[0] =~ s/^(.*\S)\s*/$1/g;
    $lines[0] =~ s/ /<\/TD><TD ALIGN=$align>/g;

    # spit out the first part of our calendar
    print <<"EOT";
<TABLE WIDTH=100% BORDER=0>
<TR><TH COLSPAN=7><A HREF="$view_month_url?year=$y\&month=$m">$name</A></TH></TR>
<TR>$TD$lines[0]</TD></TR>
EOT

    # throw away the first line, since we're done with it
    shift(@lines);

    # set up the standard hyperlink/cell combo
    $ds = "$TD<A HREF=\"$view_day_url?year=$y\&month=$m\&day=";
    $dend = "</A></TD>";

    # fix up the first row, which will be missing a few days
    # (since most months don't start on Sunday)
    # I really wish Perl had an op to force array context; then I 
    # could've gotten rid of $fc and put this on one line....
    $fc = split(/\s+/,$lines[0]) - 1;
    $lines[0] = "$TD</TD>"x(7-$fc) . $lines[0];

    # turn each line of cal output into HTML.
    # this code finds each group of whitespace followed by digits and 
    # wraps the digits in a hyperlink
    foreach (@lines) {
	chop;
	$_ = " $_";
	s/\s+(\d+)/$ds$1\">$1$dend/g;
	$_ = "<TR>$_</TR>\n";
    }

    # print the rest
    print @lines;
    print "</TABLE>\n";
}
    
