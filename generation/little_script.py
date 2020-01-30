#little_script.py

#the script create a cfg file for all the sm generations

import fnmatch
import os
import shutil
import re



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
    #print filename,re.search("\d*[E]*[+-]*"fileContent[-4]
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
    # python script.py HD VBS_e+_mu+ 0.02

    discard=[]
    print ('checking LHE files integrity...')
    files_lhe = getFilesList ('/Users/giorgio/madgraph_generations/', '*unweighted_events.lhe', [])
    for path in files_lhe[0]:
        print(type(path))
        search = re.search(".*sm/.*",path)
        found=False
        try:
            found=search.group()
        except:
            discard.append(path)
        if not found:
            if path not in discard:
                discard.append(path)

    total_files=[i for i in files_lhe[0] if i not in discard]

    # get number of events from LHE files
    # ---- ---- ---- ---- ---- ---- ---- ---- ----
    # this returns a list with the same elements found in findWSwE, but with different ordering
    #    NB = [int (countEvents (file)) for file in files_lhe[0]]


    # calculate crossSection
    # ---- ---- ---- ---- ---- ---- ---- ---- ----
    discard=[]
    matches = []
    myfilenames = []
    for root, dirnames, filenames in os.walk ('/Users/giorgio/madgraph_generations/'):
        for filename in fnmatch.filter(filenames, '*banner.txt'):
            path=os.path.join (root, filename)
            matches.append(path)
            search = re.search(".*sm/.*",path)
            found=False
            try:
                found=search.group()
            except:
                discard.append(path)
            if not found:
                if path not in discard:
                    discard.append(path)

    matches=[i for i in matches if i not in discard]
    print('\n\n'.join(matches))
    XS = [float (findXS (file)) for file in matches]
    prova=[str (findXS (file)) for file in matches]
    print('\n\n'.join(prova))
    try:
        xs= sum (XS) / len (XS)
    except ZeroDivisionError:
        xs=XS
        print(XS)
    total_xs=xs
    print(total_xs)


    outputfile = open ('/Users/giorgio/Desktop/tesi/D6EFTStudies/analysis/cfg/pure_sm.cfg' ,'w')
    text='[general]\nsamples=sm\nvariables=ptl1, ptl2, ptj1, ptj2, mjj, mll, met, etaj1, etaj2, phij1,phij2'

    outputfile.write(text+'\noutputFile=pure_sm.root\n')

    outputfile.write('[sm]\nXS='+str(total_xs*10**3)+'\nfiles='+','.join(total_files)+'\n')

    outputfile.close()
