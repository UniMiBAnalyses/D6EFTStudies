#!/usr/bin/env python

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
import argparse 
from math import sqrt 
import glob


# usual_errors = []
# usual_errors.append ('stty: standard input: Inappropriate ioctl for device')
# usual_errors.append ('impossibile creare la directory "tmp"')
# usual_errors.append ('WARNING: In non-interactive mode release checks e.g. deprecated releases, production architectures are disabled.')
# usual_errors.append ('No handlers could be found for logger "madevent.cards"')
usual_errors = ['mkdir', 'stty', 'WARNING', 'No handlers could be found for logger "madevent.cards"' ]

def readCondorReport (path):
    lista = glob.glob (path+ '/*.log')
    if len (lista) > 1:
        print ('too many log files, exiting\n')
        sys.exit (1)
    filename = lista[0]
    normal = 0
    endcodes = []
    jobID = '-1'
    endcode = '-1'
    runtime = '00:00:00'
    with open (filename, 'r') as f :
        linecount = 5
        for line in f.readlines () :
            linecount += 1
            if 'Job terminated' in line :
                linecount = 0
                jobID =  line.split ()[1][1:-1].split ('.')[1]
            if linecount == 1 :
                if 'Abnormal' in line :
                  endcode = line.split ()[4][:-1]
                else :
                  endcode = line.split ()[5][:-1]
                if endcode == '0' : normal += 1
            if linecount == 2 :
                runtime = line.split ()[2][:-1]
            if linecount == 3 :    
                endcodes.append ([jobID, endcode, runtime])                 
                jobID = '-1'
                endcode = '-1'
                runtime = '00:00:00'

#    print endcodes
#    print len (endcodes)
#    print ('normally terminated', normal)
    return endcodes
    
  
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


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
                if err in line.split(':')[0]: 
                    known = 1
                    break
            if known == 1: continue
            counter = counter + 1
    if counter > 0: return [True,( filename.split('_')[-1]).split('.')[-2], filename] #modified to match the naming convention of gridpack gen files
    return [False, (filename.split('_')[-1]).split('.')[-2], filename]
  

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


# get the list of files ending with the "pattern"
# in the folder "basefolder",
# if the file name does not contain any of the ID's
# listed in "discard"
def getFilesList (basefolder, pattern, discard):
    matches = []
    myfilenames = []
    for root, dirnames, filenames in os.walk (basefolder):
        for filename in fnmatch.filter (filenames, pattern):
            fullname = os.path.join (root, filename)
            count = 0
            for elem in discard:
                if ('.'+elem+'.' in fullname) or ('_'+elem+'/' in fullname) or ('_'+elem+'.' in fullname):
                 #added or ('_'+elem+'.' in fullname) to match naming convention of gridpack gen files
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
    line = subprocess.check_output(['tail', '-1', filename]).decode("utf-8")
    return (line== "</LesHouchesEvents>\n")


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def findXS (filename):
    objFile = open (filename, 'r')
    fileContent = objFile.read ().split ()
    return fileContent[-4]


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def findXSwE (filename):

    print("checking ", filename)
    XSline = ''
    Nline = ''
    XS_ERROR_LINE = ''
    with open(filename, 'r') as file:
        task = ''
        counter = 0
        for line in file:
            if task=='get n':
                counter+=1
                Nline=line 
                task=''
            if 'run finished, produced number of events:' in line:
                task='get n'
            if '# original cross-section:' in line:
                XSline = line
                counter+=1
            if '# PDF variation:' in line:
                XS_ERROR_LINE=line
                counter+=1
            if counter == 3:
                break
    crossSection = float (XSline.split ()[3])
    try:
        crossSectionError = crossSection*float (XS_ERROR_LINE.split ()[3][1:-1])/100.
    except:
        crossSectionError = crossSection*0.01 # if there is no # PDF variation, add dummy pdf uncertainty 0.01
    numberOfEvents = int (Nline.split ()[0])
    #print("check ", filename)
    print(crossSection, crossSectionError, numberOfEvents)
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
    print("debug:")
    print(XS)
    sumW = sum (1. / (e*e) for x, e, n in singleGenInfoList)
    print(sumW)
    XS = XS / sumW
    XSe = sqrt (1. / sumW)

    return [XS, XSe]


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def makeNtupleProdCfg (basefolder,outfolder, LHEfiles, XS):

    jetsVars = ['mjj', 'ptj1', 'ptj2', 'etaj1', 'etaj2', 'phij1', 'phij2', 'deltaetajj', 'deltaphijj']
    # jjs = ['V', 'V2', 'noV', 'mjjmax', 'mjjnomax']
    # varList = [jetsVar + '_' + jj for jetsVar in jetsVars for jj in jjs]
    leptonsVars = ['mll', 'ptl1', 'ptl2', 'etal1', 'etal2', 'ptll']
    # varList = [v.replace('mjj_mjj', 'mjj_') for v in varList]
    # varList = varList + jetsVars + 
    varList = jetsVars + leptonsVars
    rwgtList = ['rwgt_{}'.format(i) for i in range(1, 821)]

    processName = basefolder.split ('/')[-1]
    processName = processName.replace ('_results', '')

    configFileName = basefolder + '/read_03_input_new.cfg'
    outf = open (configFileName, 'w')

    outf.write ('[general]\n')
    outf.write ('samples = ' + processName + '\n')
    outf.write ('variables = {0}\n'.format(', '.join(varList+rwgtList)))
    outf.write ('outputFile = '+ outfolder +'/ntuple_' + processName + '.root\n')
    outf.write ('applycuts = false\n')
    outf.write ('\n')
    outf.write ('[' + processName + ']\n')
    outf.write ('XS = ' + ','.join(list(map(lambda k: str(k[0]), XS))) + '\n')
    outf.write ('XSerrs = ' + ','.join(list(map(lambda k: str(k[1]), XS))) + '\n')
    outf.write ('# pb\n')
    outf.write ('files = ' + LHEfiles + '\n')

    outf.close ()

    print (configFileName + ' generated\n')


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


if __name__ == '__main__':


    #parse arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('--debug', action='store_true', default=False, help='debugging mode')
    parser.add_argument("-b", "--basefolder", action="store", type=str, help="base folder of the input files")
    parser.add_argument("-t", "--task", action="store", type=str, default="" , help="choose a specific task: check, clean, unzip, rezip; optional, otherwise all except 'clean' are performed")
    parser.add_argument("-o", "--outNtuple", action="store", type=str, default="basefolder", help="output directory for the ntuples created by read_03.cpp")


    args = parser.parse_args()

    DEBUG = args.debug
    basefolder = args.basefolder
    outfolderNtuple = args.outNtuple
    task = args.task #task is optional, default "", doing all the task except for clean

    print ('folder:\t', basefolder)
    print ('---------------------------------------------')

    
    if "check" in task:
        print ('checking errors\n')
        print('blabla')
        files_err = getFilesList (basefolder, '*.err', [])

        issues = [errFileHasIssues (file) for file in files_err[0]]
        print(issues[0])
        discard = [ID for prob, ID, filename in issues if prob == True]
        for elem in discard: print ('ignoring job ' + elem)
        sys.exit (0)
      
    elif "clean" in task:
        print ('cleaning folder ' + basefolder + ' from job reports...\n')
        postprocess_file = getFilesList (basefolder, 'postProcess.txt', [])
        if (len (postprocess_file[0]) == 0):
            print 'no postProcess.txt file found, quitting\n'
            sys.exit (0)

         # remove the .out and .err only for successful jobs
        files_err = getFilesList (basefolder, '*.err', [])
        issues = [errFileHasIssues (file) for file in files_err[0]]
        removeError = [filename for prob, ID, filename in issues if prob == False]
        for file in removeError: os.remove (file)
        removeOutput = [filename.replace ('.err','.out') for filename in removeError]
        for file in removeOutput: os.remove (file)

    elif "unzip" in task:
        print ('unzipping...')
        os.system ('for fil in  `find  ' + basefolder + ' -name \"*gz\"` ; do gunzip $fil ; done')
        sys.exit (0)
 
    elif "rezip" in task:
        files_lhe = getFilesList (basefolder, '*.lhe', [])
        for filename in files_lhe[0] : 
            print ('gzip ' + filename)
            subprocess.call (['gzip', str (filename)])
        sys.exit (0)

    #Now doing the normal code
    if "basefolder" in outfolderNtuple:  #default: the output folder is the same as the input one
        outfolderNtuple = args.basefolder
        if DEBUG: print "Default output folder : ", outfolderNtuple   


    # FIXME use the numbers to discard failed jobs
    # FIXME make a histogram of the time duration of jobs
    # readCondorReport (basefolder)

    # collect the list of err files
    # ---- ---- ---- ---- ---- ---- ---- ---- ---- 

    discard=[] #check if something went wrong in the generation and discard those files. Prevent findXSwE to raise 'not found' err.
    #print ('reading folder ' + basefolder + '\n')

    print ('checking error reports...')
    files_err = getFilesList (basefolder, '*.err', [])
    issues = [errFileHasIssues (file) for file in files_err[0]]
    print(issues)
    discard = [ID for prob, ID, filename in issues if prob == True]

    #print ('checking not finished runs...')
    #files_run = getFilesList (basefolder, '*running', [])
    #discard = discard + [name.split ('_')[3] for name in files_run[1]]

    for elem in discard: print ('ignoring job ' + elem)

    # unzip LHE files
    ## ---- ---- ---- ---- ---- ---- ---- ---- ---- 

    #print ('unzipping...')
    ## https://linuxhandbook.com/execute-shell-command-python/
    #os.system ('for fil in  `find  ' + basefolder + ' -name \"*gz\"` ; do gunzip $fil ; done')
    #
    ## check lhe files integrity
    ## ---- ---- ---- ---- ---- ---- ---- ---- ---- 

    #print ('checking LHE files integrity...')
    files_lhe = getFilesList (basefolder, '*.lhe', discard)
    closure = [checkClosure (file) for file in files_lhe[0]]

    allOK = 0
    for i in range (len (closure)):
        if (closure[i] == False):
            print (files_lhe[1] + 'not properly closed')
            allOK = allOK + 1
    if allOK > 0: 
        print ('found ' + allOK + ' files not properly closed')
    else:
        print ('all files closed regularly') 

    if DEBUG: 
        print ('\nLIST OF LHE FILES:')
        print (','.join (files_lhe[0]))
        print ('\n')

    # get number of events from LHE files
    # ---- ---- ---- ---- ---- ---- ---- ---- ---- 
    # this returns a list with the same elements found in findWSwE, but with different ordering
#    NB = [int (countEvents (file)) for file in files_lhe[0]]

    # add final report to the results folder
    # ---- ---- ---- ---- ---- ---- ---- ---- ---- 

    files_out = getFilesList (basefolder, '*.out', discard)
    if DEBUG:
        print(files_out)
    XSs = [findXSwE (file) for file in files_out[0]]

    totXS = calcTotXS (XSs)
    print ('average XS: ' + str (totXS[0]) + ' +- ' + str (totXS[1]) + ' pb')
    print ('average XS: ' + str (1000. * totXS[0]) + ' +- ' + str (1000. * totXS[1]) + ' fb')

    outputfile = open (basefolder+'/postProcess.txt' ,'w')
    outputfile.write ('average XS: ' + str (totXS[0]) + ' +- ' + str (totXS[1]) + ' pb\n')
    outputfile.write ('average XS: ' + str (1000. * totXS[0]) + ' +- ' + str (1000. * totXS[1]) + ' fb\n\n')
    outputfile.write ('LHE files list:\n' + ','.join (files_lhe[0]) + '\n')
    outputfile.close ()

    # add cfg file for read_03 to the results folder
    # ---- ---- ---- ---- ---- ---- ---- ---- ---- 

    makeNtupleProdCfg (basefolder,outfolderNtuple, ','.join (files_lhe[0]), XSs)

