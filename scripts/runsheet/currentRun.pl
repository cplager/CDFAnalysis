#!/usr/bin/perl
require "config.pl";

chdir $data_dir;
open (LISTING, "ls -lt |");

while (<LISTING>) {
	if (/(\d{6}-\d{2}-\d{2}-\d{4})\.dat/) {
		$url = "$view_run_url?run=$1";
		print "Status: 302 Redirect\n";
		print "Pragma: no-cache\n";
		print "Location: $url\n\n";
		last;
	}
}
