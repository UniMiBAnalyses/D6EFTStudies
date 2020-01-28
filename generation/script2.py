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
    for root, dirnames, filenames in os.walk (basefolder):
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


    # collect the list of err files
    # ---- ---- ---- ---- ---- ---- ---- ---- ----
    #not working with sys.argv, instead using input
    EVENT_NAME=sys.argv[1]
    distributions=['sm','lin','quad']
    total_files=[]
    total_xs=[]
    for i in distributions:
        print ('checking error reports...')
        files_err = getFilesList ('/Users/giorgio/'+EVENT_NAME+'_'+i, '*.err', [])
        issues = [errFileHasIssues (file) for file in files_err[0]]
        discard = [ID for prob, ID in issues if prob == True]

        print ('checking not finished runs...')
        files_run = getFilesList ('/Users/giorgio/'+str(EVENT_NAME)+'_'+str(i), '*running', [])
        discard = discard + [name.split ('_')[3] for name in files_run[1]]

        for elem in discard: print ('ignoring job ' + elem)

        # unzip LHE files
        # ---- ---- ---- ---- ---- ---- ---- ---- ----

        print ('unzipping...')
        # https://linuxhandbook.com/execute-shell-command-python/
        os.system ('for fil in  `find  ' + '/Users/giorgio/'+str(EVENT_NAME)+'_'+str(i)+ ' -name \"*gz\"` ; do gunzip $fil ; done')

        # check lhe files integrity
        # ---- ---- ---- ---- ---- ---- ---- ---- ----

        print ('checking LHE files integrity...')
        files_lhe = getFilesList ('/Users/giorgio/'+str(EVENT_NAME)+'_'+str(i), '*.lhe', discard)
        closure = [checkClosure (file) for file in files_lhe[0]]

        allOK = 0
        for j in range (len (closure)):
            if (closure[j] == False):
                print (files_lhe[1] + 'not properly closed')
                allOK = allOK + 1
        if allOK > 0:
            print ('found ' + allOK + ' files not properly closed')
        else:
            print ('all files closed regularly')

        print ('\nLIST OF LHE FILES:')
        print (','.join (files_lhe[0]))
        print ('\n')

        total_files.append(files_lhe[0])
        # get number of events from LHE files
        # ---- ---- ---- ---- ---- ---- ---- ---- ----
        # this returns a list with the same elements found in findWSwE, but with different ordering
    #    NB = [int (countEvents (file)) for file in files_lhe[0]]


        # calculate crossSection
        # ---- ---- ---- ---- ---- ---- ---- ---- ----

        matches = []
        myfilenames = []
        for root, dirnames, filenames in os.walk ('/Users/giorgio/'+str(EVENT_NAME)+'_'+str(i)):
            for filename in fnmatch.filter(filenames, '*banner.txt'):
                matches.append (os.path.join (root, filename))
                myfilenames.append (filename)
        print(matches)
        XS = [float (findXS (file)) for file in matches]
        try:
            xs= sum (XS) / len (XS)
        except ZeroDivisionError:
            xs=XS
            print(XS)
        total_xs.append(xs)

        """
        matches = []
        myfilenames = []
        for root, dirnames, filenames in os.walk ('/Users/giorgio/'+str(EVENT_NAME)+'_'+str(i)+'/Events/run_01/run_01_tag_1_banner.txt'):
            for filename in fnmatch.filter(filenames, '*banner.txt'):
                matches.append (os.path.join (root, filename))
                myfilenames.append (filename)

        XS = [float (findXS (file)) for file in matches]
        print(XS)"""

        #xs=findXS('/Users/giorgio/'+str(EVENT_NAME)+'_'+str(i)+'/Events/run_01/run_01_tag_1_banner.txt')

        """
        files_banner = getFilesList ('/Users/giorgio/'+str(EVENT_NAME)+'_'+str(i)+'/Events/run_01/', '*.txt', discard)
        print(files_banner)
        XS=[float(findXS(file)) for file in files_banner[0]]
        if len(XS)!=0:
            xs=sum(XS)/len(XS)
        else:
            xs=0
        total_xs.append(xs)
"""


    outputfile = open ('/Users/giorgio/Desktop/tesi/D6EFTStudies/analysis/cfg/'+EVENT_NAME+'.cfg' ,'w')
    text='[general]\nsamples=sm,lin,quad\nvariables=ptl1, ptl2, ptj1, ptj2, mjj, mll, met, etaj1, etaj2, etal1, etal2'

    outputfile.write(text+"\noutfile="+EVENT_NAME+'.root\n')
    for i in range(3):
        outputfile.write('['+distributions[i]+']\nXS='+str(total_xs[i])+'\nfiles='+','.join(total_files[i])+'\n')

    outputfile.close()

    #now that we have a cfg file we should convert it with read_03
    os.chdir('/Users/giorgio/Desktop/tesi/D6EFTStudies/analysis/')
    if os.path.exists("/Users/giorgio/Desktop/tesi/D6EFTStudies/analysis/read_03"):
        print('eccoci')
        os.system("./read_03 "+'cfg/'+EVENT_NAME+'.cfg')
    else:
        os.chdir('/Users/giorgio/Desktop/tesi/D6EFTStudies/analysis/')
        os.system('c++ -o read_03 `root-config --glibs --cflags` CfgParser.cc utils.cc LHEF.cc -lm read_03.cpp')
        os.system('./read_03 cfg/'+EVENT_NAME+'.cfg')
