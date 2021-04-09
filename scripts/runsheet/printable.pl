#!/usr/bin/perl -n

next if /FORM/;
next if /INPUT/;
next if /TEXTAREA/;
$optioning = 1 if /SELECT/;
$optioning = 0 if /\/SELECT/;
next if $optioning;
print;
