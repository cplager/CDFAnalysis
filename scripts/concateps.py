#!/usr/bin/env python
#
# concateps.py - Concatenate EPS files into one postscript file
#                Each EPS file gets its own page
#
#

import sys, getopt

# Some postscript strings

docHeaderPS = """%%!PS-Adobe-2.0
%%%%Creator: concateps.py (Adam L. Lyon  21-Oct-1997)
%%%%Pages: %d
%%%%PageOrder: Ascending
%%%%BoundingBox: 0 0 612 792
%%%%EndComments
"""

epsHeaderPS = """%%%%Page: %d %d
%%%%BeginDocument: %s
"""

epsTrailerPS = """%%EndDocument

"""

docTrailerPS = """%%Trailer
end
%%EOF
"""

######################################################################
# usage(): Print out usage stuff

def usage(com):
    print 'Usage:', com, 'epsfile1 epsfile2 ...'
    sys.exit(1)

######################################################################
# concatEPS( fileList)  - Concatenate EPS files in fileList
def ConcatEPS( fileList ):
    
    numPages = len(fileList)
    
    # Write out the document header
    sys.stdout.write(docHeaderPS % numPages)

    page = 1

    # Loop over the files
    for fileName in fileList:
	
	# Open the file
	try:
	    inEps = open(fileName, 'r')
	except:
	    sys.stderr.write('Cannot open file '+fileName)
	    sys.exit(1)

	sys.stderr.write('Merging with file '+fileName+'\n')
	
	# Write the EPS file header
	sys.stdout.write(epsHeaderPS % (page, page, fileName) )
	
	# Include the postscript (since files can be big, read one line
	#  at a time so memory isn't overflowed
	line = 'nnn'
	while line:
	    line = inEps.readline()
	    sys.stdout.write(line)

	# Write the EPS trailer
	sys.stdout.write(epsTrailerPS)
	
	page = page + 1

    # Write the document trailer
    sys.stdout.write(docTrailerPS)

    return


######################################################################
######################################################################

if __name__ == '__main__':
    
    import getopt
    
    # Parse arguments
    try:
	optlist, fileList = getopt.getopt(sys.argv[1:], '')
    except getopt.error:
	usage(sys.argv[0])

    # Must have at least one eps file
    if len(fileList) == 0: usage(sys.argv[0])

    # Do it
    ConcatEPS(fileList)


