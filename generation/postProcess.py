#!/usr/bin/python

# unzip LHE files in a folder
# calculate the XS of the samples in a generation
# verify the integrity of all LHE files
# compare the number of generated events present in the lhe file to the ones reported in the log
# prepare list of files for CFG file
# if asked, remove log files


import sys
import fnmatch
import os
import subprocess
from math import sqrt 


usual_errors = []
usual_errors.append ('stty: standard input: Inappropriate ioctl for device')


# check whether in the err file there are unexpected errors,
# or solely some known irrelevant ones,
# listed in usual_errors
# True = there is a problem
# False = everything is OK
def errFileHasIssues (filename):
    counter = 0
    with open (filename, 'r') as file:
        for line in file:
            known = 0
            for err in usual_errors:
                if err in line: 
                    known = 1
                    break
            if known == 1: continue
            counter = counter + 1
    if counter > 0: return [True, filename.split('.')[-2]]
    return [False, filename.split('.')[-2]]
  

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


# get the list of files ending with the "pattern"
# in the folder "basefolder",
# if the file name does not contain any of the ID's
# listed in "discard"
def getFilesList (basefolder, pattern, discard):
    matches = []
    myfilenames = []
    for root, dirnames, filenames in os.walk (sys.argv[1]):
        for filename in fnmatch.filter(filenames, pattern):
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


# this function runs on a list of outputs
# of the findXSwE function,
# i.e. a list of tuples
# from the tuples the relavant info is the XS and the XS uncertainty
def calcTotXS (singleGenInfoList):
#    XSs = [info[0] for info in singleGenInfoList] # single cross sections
#    Ws = [ 1. / (info[1] * info[1]) for info in singleGenInfoList] # weights
#    sumW = sum (Ws)

    XS = sum (x / (e*e) for x, e, n in singleGenInfoList)
    sumW = sum (1. / (e*e) for x, e, n in singleGenInfoList)
    XS = XS / sumW
    XSe = sqrt (1. / sumW)

    return [XS, XSe]


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


if __name__ == '__main__':

    if len (sys.argv) < 2:
        print ('base folder of the sample missing')
        sys.exit ()

    # collect the list of err files
    # ---- ---- ---- ---- ---- ---- ---- ---- ---- 

    files_err = getFilesList (sys.argv[1], '*.err', [])
    issues = [errFileHasIssues (file) for file in files_err[0]]
    discard = [ID for prob, ID in issues if prob == True]

    files_run = getFilesList (sys.argv[1], '*running', [])
    discard = discard + [name.split ('_')[3] for name in files_run[1]]

    for elem in discard: print ('ignoring job ' + elem)

    # unzip LHE files
    # ---- ---- ---- ---- ---- ---- ---- ---- ---- 

    # https://linuxhandbook.com/execute-shell-command-python/
    os.system ('for fil in  `find  ' + sys.argv[1] + ' -name \"*gz\"` ; do gunzip $fil ; done')
    
    # check lhe files integrity
    # ---- ---- ---- ---- ---- ---- ---- ---- ---- 

    files_lhe = getFilesList (sys.argv[1], '*.lhe', discard)
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

    print ('\nLIST OF LHE FILES:')
    print (','.join (files_lhe[0]))
    print ('\n')

    # get number of events from LHE files
    # ---- ---- ---- ---- ---- ---- ---- ---- ---- 
    # this returns a list with the same elements found in findWSwE, but with different ordering
#    NB = [int (countEvents (file)) for file in files_lhe[0]]


    # collect the list of out files
    # ---- ---- ---- ---- ---- ---- ---- ---- ---- 

    files_out = getFilesList (sys.argv[1], '*.out', discard)
    XSs = [findXSwE (file) for file in files_out[0]]

    totXS = calcTotXS (XSs)
    print ('average XS: ' + str (totXS[0]) + ' +- ' + str (totXS[1]) + ' pb')
    print ('average XS: ' + str (1000. * totXS[0]) + ' +- ' + str (1000. * totXS[1]) + ' fb')

