#script.py

#the script moves the datacards from a certain folder to this git repo

import fnmatch
import os
import shutil


matches = []
myfilenames = []
for root, dirnames, filenames in os.walk("/Users/giorgio/madgraph_generations/"):


    for filename in fnmatch.filter(filenames, 'param_card.dat'):
        matches.append (os.path.join (root, filename))
        myfilenames.append (filename)


relative_paths=[os.path.relpath(full_path, '/Users/giorgio/madgraph_generations/') for full_path in matches]
matches_splitted=[a.split("/") for a in relative_paths]


for j in range(len(matches)):
    #we create a directory in D6EFTStudies/generation/datacards
    os.chdir("/Users/giorgio/Desktop/tesi/D6EFTStudies/generation/datacards")
    for i in matches_splitted[j][:len(matches_splitted[j])-1]:
        try:

            os.mkdir(i)
        except:
            #it was already there
            pass
        os.chdir(i)
    #we copy the param_card.dat but also run_card and proc_card
    shutil.copy(matches[j],".")
    shutil.copy(os.path.dirname(matches[j])+'/proc_card_mg5.dat',".")
    shutil.copy(os.path.dirname(matches[j])+'/run_card.dat',".")
