#!/usr/bin/python3
from sys import argv
import configparser
import subprocess
import threading
import os
import glob
class Worker(threading.Thread):
    def __init__(self, i):
        threading.Thread.__init__(self)
        self.i = i

    def run(self):
        if (os.path.isfile(jobDir+ f"/config_{self.i}.cfg")):
            logfile = open(jobDir+f"/log_{self.i}.log", 'w')
            logfile.write(f"Workin on thread {self.i}\n")
            configTmp = configparser.ConfigParser()
            configTmp.optionxform = str
            configTmp.read(jobDir+ f"/config_{self.i}.cfg")
            sample_name = configTmp['general']['samples']
            config = configparser.ConfigParser()
            config.optionxform = str
            config['general'] = {}
            for key in configTmp['general']:
                config['general'][key]=configTmp['general'][key]

            config[sample_name] = {}
            for key in configTmp[sample_name]:
                config[sample_name][key]=configTmp[sample_name][key]
            
            config['general']['outputFile'] = os.path.abspath(".")+f"/ntuple_{self.i}.root" 
            del configTmp
            with open(f"config_{self.i}.cfg", "w") as file:
                config.write(file)
            readPath = '/'.join(path.split("/")[:-1]+['analysis'])
            process = subprocess.Popen("cd {}; ./read_03 {}; cd {}; rm config_{}.cfg;".format(readPath, os.path.abspath(".")+f"/config_{self.i}.cfg", os.path.abspath("."), self.i), shell=True, stdout=logfile, stderr=logfile)
            process.wait()
            logfile.close()

if __name__ == "__main__":
    jobId = argv[1]
    path = argv[2]
    sample_name = argv[3]
    jobDir = path+f"/lheReaderFolder/jobs/{sample_name}/job_{jobId}"
    
    nThreads = len(glob.glob(jobDir+"/config_*.cfg"))
    threads = []

    for i in range(nThreads):
        threads.append(Worker(i))
        threads[-1].setDaemon(True)
        threads[-1].start()

    for th in threads:
        th.join()
    
    process = subprocess.Popen("du -sh ntuple_*.root", shell=True)
    process.wait()
    print("Done reading LHEs")
    files = glob.glob("ntuple_*.root")
    print(f"Merging {files}")
    command = "hadd -j {}".format(nThreads)
    command += " ntuple_total.root"
    command += " " + ' '.join(files)
    process = subprocess.Popen(command, shell=True)
    process.wait()

    print("Done merging files")