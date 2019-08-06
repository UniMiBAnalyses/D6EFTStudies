#!/usr/bin/python

import sys
import fnmatch
import os


def countEvents (filename):
    objFile = open (filename, 'r')
    return objFile.read ().count("<event>")


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


def checkClosure (filename):
    objFile = open (filename, 'r')
    fileContent = objFile.read ().split ()
    return (fileContent[-1] == "</LesHouchesEvents>")


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


if __name__ == '__main__':

    if len (sys.argv) < 2:
        print ('base folder of the sample missing')
        sys.exit ()
        
    matches = []
    myfilenames = []
    for root, dirnames, filenames in os.walk (sys.argv[1]):
        for filename in fnmatch.filter(filenames, '*.lhe'):
            matches.append (os.path.join (root, filename))
            myfilenames.append (root+'/'+filename)
   
    closure = [checkClosure (file) for file in matches]
    XS = [float (countEvents (file)) for file in matches]

    for i in range (len(XS)):
        print (myfilenames[i] + ':\t' + str (XS[i]) + ' evt')

    print ('\n')

    allOK = 0
    for i in range (len(closure)):
        if (closure[i] == False):
            print (myfilenames[i] + 'not properly closed')
            allOK = allOK + 1
    if allOK > 0: 
        print ('found ' + allOK + ' files not properly closed')
    else:
        print ('all files closed regularly') 


