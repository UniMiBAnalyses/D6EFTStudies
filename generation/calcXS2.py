#!/usr/bin/python

# unzip LHE files in a folder
# calculate the XS of the samples in a generation
# verify the integrity of all LHE files
# compare the number of generated events present in the lhe file to the ones reported in the log
# prepare list of files for CFG file
# if asked, remove log files


# FIXME improve using the job.out file, which contains the uncertainty as well


import sys
import fnmatch
import os
import subprocess


def getFilesList (basefolder, pattern):
    matches = []
    myfilenames = []
    for root, dirnames, filenames in os.walk (sys.argv[1]):
        for filename in fnmatch.filter(filenames, pattern):
            matches.append (os.path.join (root, filename))
            myfilenames.append (root+'/'+filename)
    return [matches, myfilenames]        


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def countEvents (filename):
    objFile = open (filename, 'r')
    return objFile.read ().count("<event>")


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def checkClosure (filename):
    objFile = open (filename, 'r')
    fileContent = objFile.read ().split ()
    return (fileContent[-1] == "</LesHouchesEvents>")


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def findXS (filename):
    objFile = open (filename, 'r')
    fileContent = objFile.read ().split ()
    return fileContent[-4]


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def findXSwE (filename):
    XSline = ''
    Nline = ''
    with open(filename, 'r') as file:
        counter = 0
        for line in file:
            if counter > 0:
                Nline = line
                break
            if 'Cross-section' in line:
                XSline = line
                counter = counter + 1
    crossSection = float (XSline.split ()[2])
    crossSectionError = float (XSline.split ()[4])
    numberOfEvents = int (Nline.split ()[4])
    return (crossSection, crossSectionError, numberOfEvents)


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


if __name__ == '__main__':

    if len (sys.argv) < 2:
        print ('base folder of the sample missing')
        sys.exit ()

    # unzip LHE files
    # ---- ---- ---- ---- ---- ---- ---- ---- ---- 

    # https://linuxhandbook.com/execute-shell-command-python/
    os.system ('for fil in  `find  ' + sys.argv[1] + ' -name \"*gz\"` ; do gunzip $fil ; done')
    
    # check lhe files integrity
    # ---- ---- ---- ---- ---- ---- ---- ---- ---- 

    files_lhe = getFilesList (sys.argv[1], '*.lhe')
    closure = [checkClosure (file) for file in files_lhe[0]]

    allOK = 0
    for i in range (len(closure)):
        if (closure[i] == False):
            print (files_lhe[1] + 'not properly closed')
            allOK = allOK + 1
    if allOK > 0: 
        print ('found ' + allOK + ' files not properly closed')
    else:
        print ('all files closed regularly') 

    sys.exit ()


    # get number of events from LHE files
    # ---- ---- ---- ---- ---- ---- ---- ---- ---- 

    NB = [float (countEvents (file)) for file in files_lhe[0]]

    # collect the list of out files
    # ---- ---- ---- ---- ---- ---- ---- ---- ---- 

    files_out = getFilesList (sys.argv[1], '*.out')


# - get XS, XSe, N from .out files
# - calculate the total XS as weighted average, and provide uncertainty as well
# - compare the number of generated events with the ones present in the lhe files,
#   this is more complicated as requires matching of outfile and lhe file


    # collect the list of banner files
    # ---- ---- ---- ---- ---- ---- ---- ---- ---- 

#     files_banner = getFilesList (sys.argv[1], '*banner.txt')

 
    # calculate the cross-section
    # ---- ---- ---- ---- ---- ---- ---- ---- ---- 

#     matches_log = []
#     for root, dirnames, filenames in os.walk (sys.argv[1]):
#         for filename in fnmatch.filter(filenames, '*banner.txt'):
#             matches.append (os.path.join (root, filename))
#             myfilenames.append (filename)
# 
# 
# 
# 
# 
#     XS = [float (findXS (file)) for file in matches]
#     print sum (XS) / len (XS)
# 
# 
#     if len (sys.argv) > 2:
#     	print ('\n')
#     	for i in range (len(XS)):
#     		print (myfilenames[i] + ':\t' + str (XS[i]) + ' pb')
#         print ('\n')
#         print ('number of files: ' + str (len (XS)))
#         print ('average XS: ' + str (sum (XS) / len (XS)) + ' pb')
#         print ('average XS: ' + str (1000. * sum (XS) / len (XS)) + ' fb')



