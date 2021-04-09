#! /usr/bin/env python

import optparse, os, glob, commands, time, datetime, shutil, re, sys

# 'set' is new in Python 2.4, but almost exists in 2.3
import sys, string
version = string.split(string.split(sys.version)[0], ".")
if '2' == version[0] and '3' == version[1]:
    from sets import Set as set


###############################
## ######################### ##
## ## Private Subroutines ## ##
## ######################### ##
###############################

def _backupName (filename):
	"""Returns a backup filename."""
	if _currentTime:
		key = _currentTime
	else:
		filetime = datetime.datetime.fromtimestamp(os.stat(filename).st_mtime)
		# Convert filetime into epoch seconds to int to hex, and drop
		# leadiing'0x'
		key = hex( int( time.mktime( filetime.timetuple() ) ) )[2:]
	return "%s/%s.%s" % (_backupDirectory, filename, key)


def _backupTag (backup):
	"""Returns the tag from a backup file iff it is valid"""
	extMatch = re.search (r'\.([^\.]+)$', backup)
	if extMatch and re.match (r'^[0-9a-fA-F]{8}$', extMatch.group(1)):
		return extMatch.group(1).lower()
	else:
		return None


def _listOfBackupFiles (filename):
	"""Returns a sorted (newest to oldest) list of existing backup files"""
	globMatch = r'%s/%s.*' % (_backupDirectory, filename)
	files = glob.glob (globMatch)
	backupFiles = []
	for backup in files:
		if _backupTag (backup):
			backupFiles.append (backup)
	if backupFiles:
		backupFiles.sort (reverse = True)
		return backupFiles


def _dictOfBackupFiles (filename):
	"""returns a map of the available backup functions.  'key' is tag,
	'value' is actual backup file"""
	# start with list of actual files
	backupList = _listOfBackupFiles (filename)
	backupDict = dict( zip( backupList, backupList) )
	# add equivalent files
	equivFile = "%s/%s" % (_backupDirectory, _backupEquivFile)
	if os.path.exists (equivFile):
		equiv = open (equivFile, "r")			
		for line in equiv:
			line = line.strip()
			words = line.split(':')
			if len(words) < 3: continue
			if words[0] != filename: continue
			key   =  "%s/%s.%s" % (_backupDirectory, filename, words[1])
			value =  "%s/%s.%s" % (_backupDirectory, filename, words[2])
			# make sure the backup file still exists
			if not os.path.exists (value): continue
			backupDict[ key ] = value
	# print backupDict
	# sys.exit(0)
	return backupDict


def _splitFilename (fullFilename):
	"""Returns 'path' and 'filename' as well as changes to the correct
	directory."""
	path, name = os.path.split (fullFilename);
	# if this file is in the same directory we started _OR_ is new
	# directory with a relative path:
	if not path or '/' != path[0]:
		# Are we where we started?
		if os.getcwd() != _startingDir:
			# No?  Fix it.
			os.chdir (_startingDir)
		# if I need to go somewhere else, go there
		if path:
			os.chdir (path)
	else:
		os.chdir (path)
	return path, name


def _fileIsOK (fullFilename, **kwargs):
	"""Checks a full filename to make sure it's appropriate"""
	# make sure this file exists (unless we set in 'exists' to True)
	if not kwargs.get('exists') and not os.path.exists (fullFilename):
		print "File '%s' does not exist.  Skipping." % fullFilename		
		return False
	# We don't do directories here
	if os.path.isdir (fullFilename):
		print "File '%s' is a directory.  Skippng." % fullFilename
		return False
	# if we're stil here, then we're golden
	return True


def _getBackupFileTuple (filename) :
	"""Returns a tuple containing the name of the backup file to use,
	the equivalent file, and whether or not the file being returned is
	the latest file.  Must be called after '_splitFilename()'"""
	backupDict  = _dictOfBackupFiles (filename)
	backupFiles = backupDict.keys()
	backupFiles.sort (reverse=True)
	# Are there any files?
	useBackup = ""
	if not backupFiles:
		print "Could not find any backup files for '%'.  Skipping." % \
			  fullFilename
		return None
	# Did you pass in a key?
	if _matchKey:
		for backup in backupFiles:
			if _backupTag (backup) == _matchKey:
				# we've got it. Stop looking
				useBackup = backup
				break
	# Do we want the latest before a given date?
	elif _matchEpoch:
		maxEpoch = 0;		
		for backup in backupFiles:
			tag = _backupTag (backup)
			if not tag: continue
			backupEpoch = int (tag, 16)
			# Is this one less than our date?
			if backupEpoch <= _matchEpoch:
				# is this the most recent which is still less than our
				# date?
				if backupEpoch > maxEpoch:
					maxEpoch = backupEpoch
					useBackup = backup
			# if new best match
	else:
		useBackup = backupFiles[0]
	if not useBackup:
		print "Could not find backup file for '%' meeting criteria. "\
			  " Skipping." % fullFilename
		return none
	return backupDict.get(useBackup), useBackup, useBackup==backupFiles[0]


def _loadAllNotes():
	"""Loads all notes.  Must be called after '_splitFilename()'"""
	# clear out dictionary
	global _notesDict
	_notesDict = {}
	notesfile = "%s/%s" % (_backupDirectory, _backupNotesFile)
	if not os.path.exists (notesfile):
		# Nothing more to see here folks.  Move along.
		return
	notes = open (notesfile, "r")
	for line in notes:
		line = line.strip();
		notesMatch = re.search (r'([^:]+):(.+)', line)
		if notesMatch:
			_notesDict[ notesMatch.group(1) ] = \
						notesMatch.group(2)

def _backupEquiv (filename):
	"""Looks at all backup files.  It returns the newest tag of the
	backup file that it matches or 'None'"""
	backupFiles = _listOfBackupFiles (filename)
	if not backupFiles: return		
	for backup in backupFiles:
		# Do the sizes match?  If not, don't bother
		if os.path.getsize (backup) != os.path.getsize (filename):
			continue
		if not commands.getoutput ( "diff %s %s" % (filename, backup) ):
			# these files are the same
			return _backupTag (backup)
	return

#####################################
## ############################### ##
## ## User Callable Subroutines ## ##
## ############################### ##
#####################################


def stringToEpoch (string) :
	"""Converts a string to epoch seconds"""
	# check to see if we have an 8-digit hex number
	if re.match (r'^[\da-f]{8}$', string):
		return int (string, 16)
	try:		
		timetuple = time.strptime (string, "%b %d %Y")
	except ValueError:
		try:
			year = datetime.date.today().year
			withyear = "%s %d" % (string, year)		
			timetuple = time.strptime (withyear, "%b %d %H:%M %Y")
		except ValueError:
			print "Can't convert '%s' to epoch seconds.  Aborting" % string
			sys.exit()
		# if we're here, then the year is messed up
		time
	return int( time.mktime( timetuple ) )


def backupFile (fullFilename, **kwargs):
	"""Copies a file to _backupDirectory if not already there"""
	quiet = kwargs.get('quiet')
	if not _fileIsOK (fullFilename) :
		return
	# Split the filename into path and basename and make sure we're in
	# the right directory
	path, name = _splitFilename (fullFilename)
	if not name:
		print "Something is wrong withFile '%s'.  Skippng." % fullFilename
		return
	if not os.path.isdir (_backupDirectory):
		# Make the directory
		# print "making backup directory", os.getcwd()
		os.mkdir (_backupDirectory)
		backupTag = ""
	else:
		# Directory exists, see if we need to backup the file or not
		backupTag = _backupEquiv (name)
	# If we are being asked ot use the current time and we already
	# have a matching file, we want to tell the equivalent file about
	# this
	alreadyBackedUp = False
	if backupTag:		
		alreadyBackedUp = True
		# if we aren't using the '--currentTime' option, then we
		# should check to see if the backup of this file is the latest
		# backup file or not.  If it's not, then we should add an
		# entry to the .equiv file
		isLatest = False
		if not _currentTime:
			backupDict = _dictOfBackupFiles (name)
			# this can't be empty unless I really messed up earlier
			latest = sorted (backupDict.keys(), reverse=True)[0]
			if _backupTag (backupDict.get (latest)) == backupTag:
				isLatest = True
		if _currentTime or not isLatest:
			# This file has already been backed up, but we are asking
			# for a special tag, so make sure we make the two tags as
			# equivalent.
			if _currentTime:
				tag = _currentTime
			else:
				tag = _backupTag (_backupName(name))
			if not quiet:
				print "Current file '%s' has equivalent tags '%s' - '%s'" \
					  % (name, tag, backupTag)
			# Don't write anything if we're not copying anything
			if not options.noCopy:
				equiv = open ("%s/%s" % (_backupDirectory, _backupEquivFile),
							  "a")
				equiv.write ("%s:%s:%s\n" % (name, tag, backupTag))
				equiv.close()
			if not _message:
				# if we don't have any message to write, get out now.
				return "%s <=> %s" % (_currentTime, backupTag)
		else:
			# O.k.  This file has been backed up already and we aren't
			# asking for any special tags, so we don't need to do it
			# again.
			return
	# If we're still here then we 1) have a file and 2) it needs to be
	# backed up
	backup =  _backupName (name)
	# Does the backup file already exist?
	if os.path.exists (backup):
		print "Backup file '%s' already exists for '%s'.  Skipping." % \
			  (backup, fullFilename)
		return
	# Back it up
	if not quiet and not alreadyBackedUp:
		print "backing up file %s to %s" % (fullFilename, backup)
	if not options.noCopy and not alreadyBackedUp:
		shutil.copy2 (name, backup)
		os.chmod (backup, 444)
	# write a message if available
	if _message:
		messageString = "%s: %s" % (os.path.basename (backup), _message)
		if not quiet:
			print "Writing '%s' into notesfile" % messageString
		# Don't actually write anything if we're not copying anything
		if not options.noCopy:
			notes = open ("%s/%s" % (_backupDirectory, _backupNotesFile), "a")
			notes.write ("%s\n" % messageString)
			notes.close()
	if alreadyBackedUp:
		return "%s <=> %s" % (_currentTime, backupTag)
	else :
		return _backupTag (backup)


def restoreBackupFile (fullFilename, **kwargs):
	"""Restores a backup file."""
	quiet = kwargs.get('quiet')
	if not _fileIsOK (fullFilename, exists=True) :
		return "Problem with '%s'" % fullFilename
	# Split the filename into path and basename and make sure we're in
	# the right directory
	path, name = _splitFilename (fullFilename)
	useBackup  = _getBackupFileTuple (name)
	if not useBackup:
		return "No backup file for '%s'" % name

	if options.safeRestore and not _backupEquiv (name):
		# This file has not been backed up.  Skip
		if not quiet:
			print "'%s' is not backed up, but called with safeRestore.  Skipping" \
		% name
		return "Not currently backup"
	
	print "Restoring '%s' to '%s'." % (useBackup[0], fullFilename)
	if not options.noCopy:
		# I use copy and not copy2 because I do not want the old date
		shutil.copy (useBackup[0], name)
	return None

def diffBackupFile (fullFilename):
	"""Diff between current file and backup"""
	if not _fileIsOK (fullFilename) :
		return
	# Split the filename into path and basename and make sure we're in
	# the right directory
	path, name = _splitFilename (fullFilename)
	useBackup  = _getBackupFileTuple (name)
	if not useBackup:
		return
	if useBackup[0] != useBackup[1]:
		print "%s => %s" % (_backupTag (useBackup[0]),
							_backupTag (useBackup[1]))
	diffCommand = "diff %s %s" % (name, useBackup[0])
	print diffCommand
	os.system ( diffCommand )
	print


def listVersions (fullFilename):
	"""list all versions (and notes) for given file"""
	if not _fileIsOK (fullFilename, exists=True) :
		return
	# Split the filename into path and basename and make sure we're in
	# the right directory
	path, name = _splitFilename (fullFilename)
	backupDict = _dictOfBackupFiles (name)
	if not backupDict:
		print "No backup files found for '%s'.  Skipping." % fullFilename
		return
	print "Backup files for '%s':\n" % fullFilename
	_loadAllNotes()
	keys = backupDict.keys()
	keys.sort (reverse=True)
	for backup in keys:
		tag = _backupTag (backup)
		key = "%s.%s" % (name, tag)
		message = _notesDict.get (key)
		actualBackup = backupDict.get (backup)
		if actualBackup != backup:
			# we have an equiv here
			backupString =  "%s (as %s)" % (backup, _backupTag (actualBackup))
		else:
			backupString =  backup
		if message:
			print '%s : "%s"' % (backupString, message)
		else:
			print '%s' % (backupString)
		print commands.getoutput ("ls -l %s" % actualBackup)		
		print
	# Does the current file exist?
	if os.path.exists (name):
		equivTag = _backupEquiv (name)
		if equivTag:
			print "Current file: (same as '%s')" % equivTag
		else:
			print "Current file:"
		print commands.getoutput ("ls -l %s" % name)
		print

def backupDirectory (directoryName):
	if os.path.islink(directoryName):
		# don't bother
		return
	if (os.path.basename(directoryName) == _backupDirectory):
		# backing up teh backup directory would lead to all sorts of
		# problems, so don't do that.
		return
	if not os.path.isdir (directoryName):
		print "'%s' is not a directory.  Skipping." % directoryName
	fullPathName = os.path.abspath (directoryName);
	os.chdir (fullPathName)	
	print "\nDirectory '%s':" % fullPathName
	allfiles = glob.glob("*")
	dirNames = []
	for filename in allfiles:
		if os.path.islink (filename):
			# Again, don't bother
			continue
		if os.path.isdir (filename):
			dirNames.append ("%s/%s" % (fullPathName, filename))
			#print "Directory found: %s" % filename
			continue
		# if we're still here, this file is not a soft link or a
		# directory.  Get the extention to see if we're interested
		extMatch = re.search (r'\.([^\.]+)$', filename)
		if extMatch and extMatch.group(1).lower() in _backupExt:
			fullFilename = "%s/%s" % (fullPathName, filename)
			status = backupFile (fullFilename, quiet=True)
			if status:
				print "  %-20s : %s" % (filename, status)
			else:
				print "  %-20s :" % (filename)
			os.chdir (fullPathName)
	for dir in dirNames:
		print "dir", dir
		backupDirectory (dir)
	os.chdir (fullPathName)



##############################################################################
## ######################################################################## ##
## ##                                                                    ## ##
## ######################################################################## ##
##############################################################################


# Global state variables
_backupDirectory = ".backup"
_backupNotesFile = ".backupNotes"
_backupEquivFile = ".backupEquiv"
_startingDir     = os.getcwd();
_currentTime     = ""
_checkAllBackup  = True
_matchKey        = ""
_matchEpoch      = 0
_message         = ""
_notesDict       = {}
_backupExt       = set ()
_defaultExt      = set ( ['buildfile', 'c', 'cc', 'h', 'hh', 'html',
						  'makefile', 'pl', 'pm', 'py',
						  'tex', 'txt', 'xml'] )


########################
## ################## ##
## ## Main Program ## ##
## ################## ##
########################


if __name__ == "__main__":
	# Setup options parser
	parser = optparse.OptionParser("usage: %prog [options] file1 [file2]\n"\
								   "File backup utility.")
	backupGroup = optparse.OptionGroup (parser, "Backup Options")
	restoreGroup = optparse.OptionGroup (parser, "Restore Options")
	restoreGroup = optparse.OptionGroup (parser, "Restore/Diff Options")
	# General options
	parser.add_option ("--diff", dest="diff",
					   action="store_true", default=False,
					   help="run diff against backup file")
	parser.add_option ("--restore", dest="restore",
					   action="store_true", default=False,
					   help="restore file from backup")
	parser.add_option ("--listVersions", dest="listVersions",
					   action="store_true", default=False,
					   help="show versions of files that are in backup")
	parser.add_option ("--backupDirectory", dest="backupDirectory",
					   action="store_true", default=False,
					   help="Backup all files in this directory and under.")
	# Backup options
	backupGroup.add_option ("-m", "--message", dest="msg", type="string",
							help="message associated with backup file")
	backupGroup.add_option ("--currentTime", dest="currentTime",
							action="store_true", default=False,
							help="use current time instead of file mod time")
	backupGroup.add_option ("--defaultExt", dest="defaultExt",
							action="store_true", default=False,
							help="Uses default extentions with --backupDir")
	backupGroup.add_option ("--noCopy", dest="noCopy",
							action="store_true", default=False,
							help="Does everything except making backup copy")
	# Restore options
	restoreGroup.add_option ("--key", dest="key", type="string",
							 help="restore files to backup key 'restoreKey'")
	restoreGroup.add_option ("--date", dest="date", type="string",
							 help="restore files condition on date " +
							 "'restoreDate'. Format: 'Sep 10 2003' or " +
							 "'Sep 28 20:14'")
	restoreGroup.add_option ("--safeRestore", dest="safeRestore",
							 action="store_true",
							 help="Only restores if file is currently " +
							 "backed up")
	restoreGroup.add_option ("--datestring", dest="datestring", type="string",
							 default="",
							 help="for testing if I can correctly convert "\
							 "your date string")
	# Start parsing
	parser.add_option_group (backupGroup)
	parser.add_option_group (restoreGroup)
	(options, args) = parser.parse_args()
	_currentTime  = options.currentTime
	_message      = options.msg	
	_matchKey     = options.key
	# Deal with special case
	if options.currentTime:
		now = datetime.datetime.now()
		_currentTime = 	key = hex( int( time.mktime( now.timetuple() ) ) )[2:]
		print "Current time key: %s", _currentTime	
	if options.date:
		_matchEpoch = stringToEpoch (options.date)
	if options.defaultExt:
		_backupExt = _defaultExt
	if options.backupDirectory:
		backupDir = ''
		for arg in args:
			if os.path.isdir (arg):
				if not backupDir:
					backupDir = arg
				else:
					# If we're here, that means i've been passed two
					# different valid directories.  That's not good.
					# I should do something about this, but I'm too
					# lazy right now
					pass
			else:
				_backupExt.add (arg)
		if not backupDir:
			backupDir = '.'
		backupDirectory (backupDir)
		sys.exit (0)

	# Main loop
	for fullFilename in args:
		if options.restore:
			restoreBackupFile (fullFilename)
		elif options.diff:
			diffBackupFile (fullFilename)
		elif options.listVersions:
			listVersions (fullFilename)		
		else:
			backupFile (fullFilename)

	# Testing of datestring
	if options.datestring:
		print stringToEpoch (options.datestring)
