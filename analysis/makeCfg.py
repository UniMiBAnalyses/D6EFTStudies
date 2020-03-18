#!/usr/bin/python

# take as input a results folder (already postprocessed with postProcess.py) 
# and prepare a cfg file to be passed as argument to read_03.cpp,
# with all variables listed, and that saves in a root file that sample only

import sys
import os
import fnmatch


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


# get the list of files ending with the "pattern"
# in the folder "basefolder",
# if the file name does not contain any of the ID's
# listed in "discard"
def getFilesList (basefolder, pattern, discard):
    matches = []
    myfilenames = []
    for root, dirnames, filenames in os.walk (sys.argv[1]):
        for filename in fnmatch.filter (filenames, pattern):
            fullname = os.path.join (root, filename)
            count = 0
            for elem in discard:
                if ('.'+elem+'.' in fullname) or ('_'+elem+'/' in fullname): 
                    count = 1
                    break
            if count > 0 : continue
            matches.append (os.path.join (os.getcwd (), root, filename))
            myfilenames.append (filename)
    return [matches, myfilenames]        


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


if __name__ == '__main__':

    if len (sys.argv) < 2:
        print ('base folder of the sample missing')
        sys.exit (1)

    print ('reading folder ' + sys.argv[1] + '\n')

    processName = sys.argv[1].split ('/')[-1]
    processName = processName.replace ('_results', '')

    postprocess_file = getFilesList (sys.argv[1], 'postProcess.txt', [])
    if (len (postprocess_file[0]) == 0):
        print 'no postProcess.txt file found, quitting\n'
        sys.exit (1)

    XS = '-1.'
    LHEfiles = ''
    with open (postprocess_file[0][0], 'r') as file:
        getList = 0
        for line in file:
            if 'average XS' in line and 'pb' in line:
            	XS = line.split ()[2]
            if getList == 1:
            	LHEfiles = line
                getList = 0
            if 'LHE files list' in line:
            	getList = 1


    configFileName = sys.argv[1] + '/read_03_input.cfg'
    outf = open (configFileName, 'w')

    outf.write ('[general]\n')
    outf.write ('samples = ' + processName + '\n')
    outf.write ('variables = mjj, mll, ptj1, ptj2, etaj1, etaj2, phij1, phij2, ptl1, ptl2, etal1, etal2, met\n')
    outf.write ('outputFile = ntuple_' + processName + '.root\n')
    outf.write ('applycuts = false\n')
    outf.write ('\n')
    outf.write ('[' + processName + ']\n')
    outf.write ('XS = ' + XS + '\n')
    outf.write ('# pb\n')
    outf.write ('files = ' + LHEfiles + '\n')

    outf.close ()

    print (configFileName + ' generated\n')




#    print (LHEfiles)
#    print ('XS = ' + XS)

