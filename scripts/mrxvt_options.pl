#!/usr/bin/env perl

use warnings;
use strict;

# these used to be #defines
use constant Xterm_tab		  => 61; # change tab title
use constant Xterm_tabterm	  => 62; # change tab and terminal title
use constant Xterm_newtab	  => 63; # create a new tab with title
use constant Xterm_prevtab	  => 64; # switch to previous tab
use constant Xterm_nexttab	  => 65; # switch to next tab
use constant Xterm_tint		  => 66; # change tinting color
use constant Xterm_shade	  => 67; # change shade level
use constant Xterm_encode	  => 68; # change encoding
use constant Xterm_hide		  => 69; # hide/show tabbar
use constant Xterm_opacity	  => 70; # set opacity level
use constant Xterm_tabbtn	  => 71; # hide/show tabbar button
use constant Xterm_tabfg	  => 72; # change active tab fg
use constant Xterm_tabbg	  => 73; # change tabbar/active tab bg
use constant Xterm_itabfg	  => 74; # change inactive tab fg
use constant Xterm_itabbg	  => 75; # change inactive tab bg
use constant Xterm_trans	  => 76; # toggle background transparency
use constant Xterm_moveleft	  => 77; # move active tab to left
use constant Xterm_moveright  => 78; # move active tab to right
use constant Xterm_verybold	  => 79; # toggle bold font for color text
use constant Xterm_hotkeys	  => 80; # toggle hotkeys
use constant Xterm_saveconfig => 81; # save configuration
use constant Xterm_bgfade	  => 82; # set bg fading degree


my $arg = "";
if (@ARGV && $ARGV[0] =~ /^-/) {
   $arg = shift @ARGV;
}

my $emptyTitle = "No Title";

my $title = $ARGV[0] || $emptyTitle;

if ($arg =~ /^-st/) {
   # hide/show tab
   printf ("%c]%d;%s%c", 033, Xterm_hide, "t", 007); 
   exit;
}

if ($arg =~ /^-so/) {
   # show/hide toolbar
   printf ("%c]%d;%c", 033, Xterm_tabbtn, 007);
   exit;
}

if ($arg =~ /^-ss/) {
   # hide/show scroll bar
   printf ("%c]%d;%s%c", 033, Xterm_hide, "s", 007); 
   exit;
}

if ($arg =~ /^-pt/) {
   # previous tab
   printf ("%c]%d;%c", 033, Xterm_prevtab, 007);
   exit;
}

if ($arg =~ /^-nt/) {
   # next tab
   printf ("%c]%d;%c", 033, Xterm_nexttab, 007);
   exit;
}

my $help = "Usage: $0 -option [title]
(meant for use with mrxvt http://materm.sourceforge.net/)
-h  => this Help screen
-t  => set Title to [title]
-n  => New tab with title [title]
-nt => switch to Next Tab
-pt => switch to Previous Tab
-so => Show/hide tOol bar
-st => Show/hide Tabs
-ss => Show/hide Scroll bar
\n";

if ($arg && $arg !~ /^-t/) {
   die $help;
}

if (!$arg && $title eq $emptyTitle) {
   die $help;
}


# if we're here, then there's nothing else to do
# set title to $title
printf ("%c]%d;%s%c", 033, Xterm_tabterm, $title, 007);
exit;



if (0) {







}


#		else if (!strcmp ("-n", argv[1]))
#			/* switch to next tab */
#			


#/*
# *  All portions of code are copyright by their respective author/s.
# *  Copyright (c) 2004    Jingmin Zhou <jimmyzhou@users.sourceforge.net>
# *
# *  This program is free software; you can redistribute it and/or modify
# *  it under the terms of the GNU General Public License as published by
# *  the Free Software Foundation; either version 2 of the License, or
# *  (at your option) any later version.
# *
# *  This program is distributed in the hope that it will be useful,
# *  but WITHOUT ANY WARRANTY; without even the implied warranty of
# *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# *  GNU Library General Public License for more details.
# *
# *  You should have received a copy of the GNU General Public License
# *  along with this program; if not, write to the Free Software
# *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
# */
#
#/*
#** This program acts as a wrapper of the escape sequence to set the
#** tab and terminal title with user supplied string.
#*/
#
##include <stdio.h>
##include <strings.h>
#
#
#/*
#** mrxvt extensions of XTerm OSCs: ESC ] Ps;Pt (ST|BEL)
#** Example: echo "\e]61;newtitle\a"
#*/
##define Xterm_tab			(61) /* change tab title */
##define Xterm_tabterm		(62) /* change tab and terminal title */
##define Xterm_newtab		(63) /* create a new tab with title */
##define Xterm_prevtab		(64) /* switch to previous tab */
##define Xterm_nexttab		(65) /* switch to next tab */
##define Xterm_tint			(66) /* change tinting color */
##define Xterm_shade			(67) /* change shade level */
##define Xterm_encode		(68) /* change encoding */
##define Xterm_hide			(69) /* hide/show tabbar */
##define Xterm_opacity		(70) /* set opacity level */
##define Xterm_tabbtn		(71) /* hide/show tabbar button */
##define Xterm_tabfg			(72) /* change active tab fg */
##define Xterm_tabbg			(73) /* change tabbar/active tab bg */
##define Xterm_itabfg		(74) /* change inactive tab fg */
##define Xterm_itabbg		(75) /* change inactive tab bg */
##define Xterm_trans			(76) /* toggle background transparency */
##define Xterm_moveleft		(77) /* move active tab to left */
##define Xterm_moveright		(78) /* move active tab to right */
##define Xterm_verybold		(79) /* toggle bold font for color text */
##define Xterm_hotkeys		(80) /* toggle hotkeys */
##define Xterm_saveconfig	(81) /* save configuration */
##define Xterm_bgfade		(82) /* set bg fading degree */
#
#
#static void
#usage (int argc, char** argv)
#{
#	fprintf (stderr, "Usage:\n");
#	fprintf (stderr, "  %s -t tab_title  # set tab/terminal title\n",
#		argv[0]);
#	fprintf (stderr, "  %s -c tab_title  # create a new tab\n",
#		argv[0]);
#	fprintf (stderr, "  %s -e encoding   # change encoding method\n",
#		argv[0]);
#	fprintf (stderr, "  %s -g color      # change tinting color\n",
#		argv[0]);
#	fprintf (stderr, "  %s -s number     # change shade of tinting\n",
#		argv[0]);
#	fprintf (stderr, "  %s -o [-+]number # change opacity level\n",
#		argv[0]);
#	fprintf (stderr, "  %s -p            # activate previous tab\n",
#		argv[0]);
#	fprintf (stderr, "  %s -n            # activate next tab\n",
#		argv[0]);
#	fprintf (stderr, "  %s -a            # hide/show tabbar buttons\n",
#		argv[0]);
#	fprintf (stderr, "  %s -H [m|t|s]    # hide/show tabbar/scrollbar/menubar\n",
#		argv[0]);
#	exit (1);
#}
#
#
#void
#main (int argc, char** argv)
#{
#	if (2 != argc && 3 != argc)	{
#		usage (argc, argv);
#	}
#
#	if (3 == argc &&
#		strcmp ("-t", argv[1]) &&
#		strcmp ("-c", argv[1]) &&
#		strcmp ("-e", argv[1]) &&
#		strcmp ("-g", argv[1]) &&
#		strcmp ("-s", argv[1]) &&
#		strcmp ("-o", argv[1]) &&
#		strcmp ("-H", argv[1]))	{
#		usage (argc, argv);
#		exit (1);
#	}
#
#	if (2 == argc)	{
#		if (!strcmp ("-H", argv[1]))
#			/* switch to previous tab */
#			fprintf (stdout, "%c]%d;%c", 033, Xterm_hide, 007);
#		else if (!strcmp ("-a", argv[1]))
#			/* switch to previous tab */
#			fprintf (stdout, "%c]%d;%c", 033, Xterm_tabbtn, 007);
#		else if (!strcmp ("-p", argv[1]))
#			/* switch to previous tab */
#			fprintf (stdout, "%c]%d;%c", 033, Xterm_prevtab, 007);
#		else if (!strcmp ("-n", argv[1]))
#			/* switch to next tab */
#			fprintf (stdout, "%c]%d;%c", 033, Xterm_nexttab, 007);
#		else if (!strcmp ("-t", argv[1]) ||
#			!strcmp ("-c", argv[1]))
#			usage (argc, argv);
#			/* switch to next tab */
#		else
#			/* set tab title */
#			fprintf (stdout, "%c]%d;%s%c", 033, Xterm_tab, argv[1], 007);
#	}
#	else if (3 == argc && !strcmp ("-t", argv[1]))	{
#		/* set tab and terminal title */
#		fprintf (stdout, "%c]%d;%s%c", 033, Xterm_tabterm, argv[2], 007);
#	}
#	else if (3 == argc && !strcmp ("-c", argv[1]))	{
#		/* create new tab with user supplied title */
#		fprintf (stdout, "%c]%d;%s%c", 033, Xterm_newtab, argv[2], 007);
#	}
#	else if (3 == argc && !strcmp ("-e", argv[1]))	{
#		/* change encoding method */
#		fprintf (stdout, "%c]%d;%s%c", 033, Xterm_encode, argv[2], 007);
#	}
#	else if (3 == argc && !strcmp ("-g", argv[1]))	{
#		/* change tinting color */
#		fprintf (stdout, "%c]%d;%s%c", 033, Xterm_tint, argv[2], 007);
#	}
#	else if (3 == argc && !strcmp ("-s", argv[1]))	{
#		/* change shade value for tinting */
#		fprintf (stdout, "%c]%d;%s%c", 033, Xterm_shade, argv[2], 007);
#	}
#	else if (3 == argc && !strcmp ("-o", argv[1]))	{
#		/* change shade value for tinting */
#		fprintf (stdout, "%c]%d;%s%c", 033, Xterm_opacity, argv[2], 007);
#	}
#	else if (3 == argc && !strcmp ("-H", argv[1]))	{
#		/* show/hide tabbar/scrollbar/menubar */
#		fprintf (stdout, "%c]%d;%s%c", 033, Xterm_hide, argv[2], 007);
#	}
#
#	exit (0);
#}

