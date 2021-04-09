#!/usr/bin/env python
#
# getLumi.py - Copyright 2010 Mario Kadastik
#
# Version 1.0, date 12 April 2010
# 
# This is a temporary tool to calculate the integrated luminosity of CMS based on a JSON file
#
# Usage: python getLumi.py in.json
#
# Where the in.json is either a JSON file you get from CRAB by e.g. crab -report or a JSON listing the good runs from some other source
#
# The code requires lumi_by_LS.csv present in the current working directory. You can get the file from: 
#  https://twiki.cern.ch/twiki/bin/view/CMS/LumiWiki2010Data
# 

import json, csv, sys

runsAfter = 0
if ( len(sys.argv) > 2 ) :
	runsAfter = sys.argv[2]
	

my_lumi_json=file(sys.argv[1],'r')
my_lumi_dict = json.load(my_lumi_json)

myListOfRuns = []

for run, lumis in my_lumi_dict.items():
	
	#print "\nRun is"
	#print run
	#print "\nLumi is"
	#print lumis
	
	myListOfRuns.append(run)
									

print "\n\nUse this list of runs for your crab file:"

myListOfRuns.sort()



for run in myListOfRuns :
	if (run > runsAfter):
		sys.stdout.write ("%s," % ( run ))



