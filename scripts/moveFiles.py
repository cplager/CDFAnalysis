#! /usr/bin/env python

import sys, re, os, socket, time, shutil, string, optparse

# 'set' is new in Python 2.4, but almost exists in 2.3
version = string.split(string.split(sys.version)[0], ".")
if '2' == version[0] and '3' == version[1]:
    from sets import Set as set

# allowed options for config file
allowed = set (['dir', 'host', 'hostequiv', 'backupcmd', 'user'])
allowedMultiple = set (['hostequiv']) # allowed multiple entries

# extentions that end in this should not be broken up from the
# previous term
notSufficientExt = set (['gz'])

# extentions that should be sent as text (when 'txt' is 'auto')
txtExtentions = set (['cc', 'c', 'hh', 'h', 'py', 'pl', 'pm', 'tcl'])

# Image extentions (files that will get <img src> tag)
imageExtentions = set (['png', 'gif', 'jpg'])

# convertedToImage extentions (files that will be matched with an
# image)
convertedToImageExtentions = set (['eps', 'ai', 'xls', 'pdf'])

# initialize variables for command line parcing
dest  = ""
files = []
flags = {}

# default options
# Have script decide if it should append '.txt' or not
flags['txt'] = 'auto'
# If true, it uses this command to backup files.  If you do not have
# this script, just set this to "" instead.
flags['backup'] = "backupNeeded"

#################
# Parce options #
#################

parser = optparse.OptionParser ("Usage: %prog [options] dest file1 [file2]")
parser.add_option ("--tag", dest="tag", type="string",                   
                   help="Adds '_TAG' to filenames")
parser.add_option ("--keepPath", dest="keepPath", action="store_true",
                   default=False, help="Keeps the full path of files")
parser.add_option ("--list", dest="asList", action="store_true",
                   default=False, help="Outputs hrefs as list")
parser.add_option ("--ignoreIf", dest="iif", type="string",
                   help="Will not copy file if matches regex in 'IF'.  " +
                   "Comma separated, '%' as wildcard")
parser.add_option ("--dirMatch", dest="match", type="string",
                   help="Calls self wrapped with 'find' and 'xargs.  Matches "+
                   "file extentions in 'MATCH'")
parser.add_option ("--noCopy", dest="noCopy", action="store_true",
                   default=False, help="Does everything except copy files " +
                   "- Useful with --dirMatch and debugging")
parser.add_option ("--txt", dest="txt", action="store_true", default=False,
                   help="add '.txt' to copied file")
parser.add_option ("--notxt", dest="notxt", action="store_true",
                   default=False,
                   help="prevents '.txt' to copied file")
parser.add_option ("--dm", dest="dm", action="store_true", default=False)
(options, args) = parser.parse_args()
neededArgs = 2
# if we are using the dirMatch option, then we do not need to list
# files as that will be done for us.
if options.match and not options.dm:
    neededArgs = 1
if len(args) < neededArgs:
    parser.error("Must provide destination and at least one file.")
dest  = args[0]
files = args[1:]

# setup find/xargs command
if options.match and not options.dm:
    extentions = options.match.split(",")
    command = "find . -regex '.+\\(" + "\\|".join (extentions) + \
              "\\)' | xargs  "+ " ".join(sys.argv) + " --dm "
    print command
    os.system ( command )
    sys.exit(0)

ignoreList = []
if options.iif:
    pieces = options.iif.split(",")
    for piece in pieces:
        ignoreList.append( '^' + re.sub( r'%', r'.*', piece ) + '$' )

if options.txt:
    flags['txt'] = True
elif options.notxt:
    flags['txt'] = False
else:
    flags['txt'] = 'auto'

# get the environment variable $HOME
config = "%s/.moveFiles.config" % os.environ.get ('HOME', ".")

# Try opening the file
try:
    configFile = open (config, "r")
except:
    print "Could not open configuration file '%s'.  Aborting." % config
    sys.exit(1)

##############
# parce file #
##############
name = ""
configInfo = {}
for line in configFile:
    line = re.sub (r'#.+$', '', line) # remove comment characters
    line = line.strip() # remove inner and outer spaces
    if not line: continue # don't bother with empty lines

    # + name definition
    nameMatch = re.search (r'^\+\s+(\S+)', line)
    if nameMatch:
        name = nameMatch.group (1)
        configInfo[name] = {}
        continue
    attribMatch = re.search (r'^\-\s+(\S+)\s+(\S.+)', line)

    # - attribute definition
    if attribMatch:
        if not name:
            print "Problem with '%': Must define name before attributes:"  % \
                  config
            print "%s\n" % line
            sys.exit(1)
        attribName = attribMatch.group (1).lower()
        value = attribMatch.group (2)
        if attribName not in allowed :
            print "Attribute '%s' not allowed in %s" % (attribName, config)
            print "%s\n" % line
            sys.exit(1)
        if attribName in allowedMultiple:            
            configInfo[name][attribName] = re.split (r'\s+', value)
        else:
            configInfo[name][attribName] = value
        continue

##################################################################
# check to make sure that we have what we need for the requested #
# definition                                                     #
##################################################################
if dest not in configInfo:
    print "Information about destination '%s' is not "\
          "available in '%s'.  Aborting." % (dest, config)
    sys.exit(1)
hostInfo = configInfo[dest]
if 'dir' not in hostInfo or 'host' not in hostInfo:
    print "Sufficient information ('dir', 'host') are not available "\
          "for destination '%' in '%'.  Aborting." % (dest, config)

# get current host name
thisComputer = socket.gethostname()
hostComputer = hostInfo['host']

# are we on the target computer
onHost = False
if thisComputer == hostComputer:
    onHost = True
else:
    # do we have equivalent host matching
    equivs = hostInfo.get ('hostequiv')
    if equivs:
        for regex in equivs:
            if re.match (regex, thisComputer):
                onHost = True
                break

######################
# Prepare The Files! #
######################
monthKey  =  time.strftime("%y%m")
dateKey   = time.strftime("%y%m%d_")
targetDir = hostInfo['dir'] + "/" + monthKey
filesMap = {}
for sourceFile in files:
    if not os.path.exists (sourceFile):
        print "File '%s' does not exist.  Aborting." % sourceFile
        sys.exit(1)
    ignoreFile = False
    for ignoreRegex in ignoreList:
        if re.match (ignoreRegex, sourceFile):
            print "Ignoring file '%s'" % sourceFile
            ignoreFile = True
            break
    if ignoreFile: continue
    if options.keepPath:
        # trim leading './' if it is there
        displayName = re.sub (r'^./', '', sourceFile)
        targetFile = dateKey + re.sub (r'/', '__', displayName)
    else:
        displayName = os.path.basename (sourceFile)
        targetFile = dateKey + displayName
    ######################
    # Add Additional Tag #
    ######################
    tag = options.tag
    if tag:
        # break up the filename
        parts = targetFile.split (".")
        # should we consider combining the last two pieces?
        last = len (parts) - 1
        if last >= 2 and parts[last].lower() in notSufficientExt:
            parts[last -1:] = [parts[last - 1] + "." + parts[last]]
            last = len (parts) - 1
        # do we have enough pieces to bother messing with this
        if last >= 1:
            parts[last - 1] = parts[last - 1] + "_" + tag
        else:
            # if we're here, then we can't find any extention to
            # remove before placing the tag, so we'll just put it on
            # the end.
            parts[last] = parts[last] + "_" + tag
        targetFile = ".".join (parts)
    #######################
    # Should we add .txt? #
    #######################
    txt = flags.get ('txt')
    if 'auto' == txt:
        extMatch = re.search (r'\.([^\.]+)$', targetFile)
        if extMatch:
            if extMatch.group(1).lower() in txtExtentions:
                txt = True
            else:
                txt = False            
        else:
            txt = False
    if txt:
        targetFile = targetFile + ".txt"
    # Add the full path
    fullTargetFile = targetDir + "/" + targetFile
    # store the results
    filesMap[displayName] = (sourceFile, fullTargetFile, targetFile)

################################    
## Copy Files to New Location ##    
################################    
for displayName, Tuple in filesMap.iteritems():
    if options.noCopy: continue
    sourceFile     = Tuple[0]
    fullTargetFile = Tuple[1]
    targetFile     = Tuple[2]
    #################################
    # Copy the file to the log area #
    #################################
    if onHost:
        # We are on the host, so everything is local
        backup = flags.get ('backup')
        if backup:
            cmd = "cd %s; %s %s >/dev/null 2>&1" % \
                  (targetDir, backup, targetFile)
            os.system (cmd)
        shutil.copy2 (sourceFile, fullTargetFile)
    else:
        # Remote copy
        user = hostInfo.get ('user', "")
        if user:
            user = "-l " + user
        backup = hostInfo.get ('backupcmd', 0)
        if backup:
            cmd = "ssh %s %s \"cd %s; %s %s\"  >/dev/null 2>&1" \
                  % (user, hostComputer, \
                     targetDir, \
                     backup, targetFile)
            os.system (cmd)
        cmd = "scp %s %s %s:%s" % (user, sourceFile, \
                                   hostComputer, fullTargetFile)
        os.system (cmd)

###############################
## Print out HTML references ##
###############################
print
if (options.asList):
    print "<ul>"
for displayName, Tuple in filesMap.iteritems():
    sourceFile     = Tuple[0]
    fullTargetFile = Tuple[1]
    targetFile     = Tuple[2]
    if (options.asList):
        print "   <li>",
    # Get the extention
    extMatch = re.search (r'\.([^\.]+)$', targetFile)
    printed = False
    if extMatch:
        extention = extMatch.group(1).lower()
        if extention in imageExtentions:
            print '<p><img src="%s"><br>' % targetFile
            printed = True
        elif extention in convertedToImageExtentions:
            # Do we have a file of the same
            extReg = "%s%s" % (extention, "$")
            for imgExt in imageExtentions:
                imageFile = re.sub (extReg, imgExt, displayName, re.IGNORECASE)
                if imageFile in filesMap:
                    print '<p><a href="%s"><img src="%s"></a><br>' % \
                          (targetFile, imageFile)
                    printed = True
    if not printed:
        print '<a href="%s"><tt>%s</tt></a>' % (targetFile, displayName)
if (options.asList):
    print "</ul>"
