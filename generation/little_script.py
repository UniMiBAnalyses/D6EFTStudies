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

    #op_list=['Hl1','Hq3','HW','ll1','qq11','qq31','qq3','HDD','W','Hbox','HWB','ll','qq1','Hl3','Hq1']
    #for op in op_list:
    total_files=[]
    total_xs=[]
    #xs=[-5.1216162e-05,1.5928681e-06] # this is for HWB
    #xs=[-5.9245262e-05, 2.274168e-06] #this is for HDD
    #xs=[0.00016516602,0.0025452691] #this is for W
    #xs=[8.28590797619e-05,2.5177002e-05] #this is for HW
    #xs=[-1.9591292e-05,3.7160972e-06] #this is for Hbox
    #xs=[6.9489613e-07,5.5387031e-07] #this is for Hl1
    #xs=[-1.6932688e-08,8.92019818182e-08] #this is for ll
    #xs=[-0.0022319754,0.00027206436] #this is for Hl3
    xs=[-0.0057684027,0.0322345407407] #this is for qq3
    #xs=[0.00162001420561,0.000515397896226] #this is Hq3
    #xs=[-0.000812370326733,0.0102571792308] #this is for qq1
    #xs=[0.00167207356436,0.000152297176471] #this is for ll1
    #xs=[-0.000977960841584,0.0100415628] #this is for qq11
    #xs=[-0.00456990115702,0.0329335475248] #this is for qq31
    #xs=[8.3041389e-05,8.46956018963e-05] #this is for Hq1

    op='qq3'
    path = '/Users/giorgio/madgraph_generations/'+op+'/'+op+'_0p3_'
    distributions=['lin','quad']
    for i in distributions:
        print ('checking error reports...')
        files_err = getFilesList (path+str(i)+'/Events/', '*.err', [])
        issues = [errFileHasIssues (file) for file in files_err[0]]
        discard = [ID for prob, ID in issues if prob == True]

        print ('checking not finished runs...')
        files_run = getFilesList (path+str(i), '*running', [])
        discard = discard + [name.split ('_')[3] for name in files_run[1]]

        for elem in discard: print ('ignoring job ' + elem)

        # unzip LHE files
        # ---- ---- ---- ---- ---- ---- ---- ---- ----

        print ('unzipping...')
         #https://linuxhandbook.com/execute-shell-command-python/
        os.system ('for fil in  `find  ' + path+str(i)+'/ -name \"*gz\"` ; do gunzip -f $fil ; done')


        print ('checking LHE files integrity...')
        files_lhe = getFilesList (path+str(i)+'/Events/', '*.lhe', discard)
        closure = [checkClosure (file) for file in files_lhe[0]]

        allOK = 0
        for j in range (len (closure)):
            if (closure[j] == False):
                print (','.join(files_lhe[1]) + 'not properly closed')
                allOK = allOK + 1
        if allOK > 0:
            print ('found ' + str(allOK) + ' files not properly closed')
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

        """matches = []
        myfilenames = []
        for root, dirnames, filenames in os.walk (path+str(i)+'/Events_0/'):
            for filename in fnmatch.filter(filenames, '*banner.txt'):
                matches.append (os.path.join (root, filename))
                myfilenames.append (filename)
        XS = [float (findXS (file)) for file in matches]
        try:
            xs= sum (XS) / len (XS)
        except ZeroDivisionError:
            xs=XS
            print(XS)
        total_xs.append(xs)"""
    #print(total_xs[0])
    #print(total_xs[1])
    print(total_files[0])
    print(total_files[1])

    outputfile = open ('/Users/giorgio/Desktop/tesi/D6EFTStudies/analysis/cfg/'+op+'_0p3.cfg' ,'w')
    text='[general]\nsamples='+','.join(distributions)+'\nvariables=ptl1, ptl2, ptj1, ptj2, mjj, mll, met, etaj1, etaj2, phij1,phij2'

    outputfile.write(text+'\noutputFile='+op+'_0p3.root\n')
    for i in range(2):
        outputfile.write('['+distributions[i]+']\nXS='+str(xs[i]*10**3)+'\nfiles='+','.join(total_files[i])+'\n')
    #outputfile.write('[quad]\nXS='+str(xs*10**3)+'\nfiles='+','.join(total_files)+'\n')

    outputfile.close()
