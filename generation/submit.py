#!/usr/bin/python

import commands
import fileinput
import sys
import os


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


if __name__ == "__main__":

    shell_script = 'job.sh' # this has to exist in the submit folder, 
                            # and to be generic

#   parstage      = sys.argv[1]

# voglio come input
# - nome della generazione, corrisponde alla cartella fatta in madgraph
# - numero di eventi
# - CMSSW folder da source-are
# - valore del parametro EFT da mettere nella generazione
# - crea la cartella di output, controlla che non esista, se esiste abort? (o fai qc)

    condor_cfg   = ''