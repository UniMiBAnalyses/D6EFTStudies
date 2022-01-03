import argparse
import configparser
import ROOT
import subprocess
from math import ceil
import threading
import os
import glob
import shutil 
import sys
class Worker(threading.Thread):
    def __init__(self,  i, job_xs_files ):
        threading.Thread.__init__(self)
        self.i = i
        self.job_xs_files = job_xs_files

    def run(self):
        jobDir = basefolder+"/job_{}".format(i)
        if os.path.isdir(jobDir):
            shutil.rmtree(jobDir)
        os.makedirs(jobDir, exist_ok=True)
        nFilesPerThread = ceil(len(self.job_xs_files)/args.nThreads)


        
        for j in range(args.nThreads):
            # this thread will be working with files [j*args.maxNumber:(j+1)*args.maxNumber]
            thread_xs_files = self.job_xs_files[j*nFilesPerThread:(j+1)*nFilesPerThread]

            configTmp = configparser.ConfigParser()
            configTmp.optionxform = str
            configTmp['general'] = {}
            for key in config['general']:
                configTmp['general'][key] = config['general'][key]
            configTmp['general']['outputFile'] = "should_be_changed_by_cluster_script"

            XS = sum ([x / (e*e) for x, e, f in thread_xs_files])
            sumW = sum ([1. / (e*e) for x, e, f in thread_xs_files])
            XS = XS / sumW
            configTmp[sample_name] = {}
            configTmp[sample_name]['XS'] = str(XS)
            configTmp[sample_name]['files'] = ','.join(list(map(lambda k: k[2], thread_xs_files)))
            with open(jobDir+"/config_" + str(j) +".cfg", 'w') as file:
                configTmp.write(file)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-cfg"  ,"--read03_cfg", help="Path to cfg card" , required=True)
    parser.add_argument('-b'    , '--doBatch', type=bool, help="Run reading jobs on condor", default=False)
    parser.add_argument('-n'    , '--nThreads', type=int, help="Number of threads", default=4)
    parser.add_argument('-hadd' , '--doHadd', type=bool, help="Join ntuples from lheReaderLHE folder matching sample name from cfg card", default=False)
    parser.add_argument("-max","--maxNumber", type=int, help="Max number of files per thread", default=5)
    
    args = parser.parse_args()
    
    config = configparser.ConfigParser()
    config.optionxform = str
    config.read(args.read03_cfg)
    sample_name = config['general']['samples']
    xs_list = list(map(lambda k: float(k), config[sample_name]['XS'].split(',')))
    xs_errs_list = list(map(lambda k: float(k), config[sample_name]['XSerrs'].split(',')))
    files_list = config[sample_name]['files'].split(',')
    xs_files = list(zip(xs_list, xs_errs_list, files_list))

    """
    Structure of folders for this script:
    - readLhe.py
    - readLheScript.py
    - lheReaderFolder
        - jobs
            - Sample Name (gp_Zjj_ewk)
                - job_0
                    - config_0.py
                    - submit.jdl
                    ...
        - EFT_LHE
            - gp_ntuples
                - ntuple_sample_name_0.py
                ...
                - ntuple_sample_name.py (the complete one)
    """

    # this folder will contain ntuples from jobs and merged ones
    if os.path.islink("lheReaderFolder/EFT_LHE"):
        os.makedirs("lheReaderFolder/EFT_LHE/gp_ntuples", exist_ok=True)
    else:
        print("You should create a symlink to your eos EFT_LHE inside lheReaderFolder")
        sys.exit(0)
    basefolder = "lheReaderFolder/jobs/"+sample_name
    os.makedirs(basefolder, exist_ok=True)
    
    if args.doBatch:
        nJobs = ceil(len(files_list)/(args.nThreads * args.maxNumber))
        # nJobs = 1
        filesPerJob = ceil(args.nThreads * args.maxNumber)
        print("Number of files per job: {}".format(filesPerJob))

        

        threads = []
        for i in range(nJobs):
            # this jobs will be working with files [i*filesPerJob: (i+1)*filesPerJob]
            job_xs_files = xs_files[i*filesPerJob: (i+1)*filesPerJob]
            threads.append(Worker(i, job_xs_files))
            threads[-1].setDaemon(True)
            threads[-1].start()

        for th in threads:
            th.join()
        

        shFile =  "#!/bin/bash\n"
        shFile += "export X509_USER_PROXY=/afs/cern.ch/user/g/gpizzati/.proxy\n"
        shFile += "python3 {}/readLheThreadingScript.py $1 {} {}\n".format(os.path.abspath("."), os.path.abspath("."), sample_name)
        with open('lheReaderFolder/readLheScript.sh', 'w') as file:
            file.write(shFile)
        process = subprocess.Popen("chmod +x lheReaderFolder/readLheScript.sh", shell=True)
        process.wait()
        

        print("Submitting files", end='')


        ### Write .jdl 
        jdl = "Universe = vanilla\n"
        jdl += '+JobFlavour = "espresso"\n'
        jdl += "Executable = readLheScript.sh\n"
        jdl += "Arguments = $(Process)\n"
        jdl += f"request_cpus = {int(args.nThreads/2)}\n"
        jdl += "should_transfer_files = YES\n"
        jdl += "when_to_transfer_output = ON_EXIT\n"
        jdl += f"Log = jobs/{sample_name}/job_$(Process)/job.log\n"
        jdl += f"Output = jobs/{sample_name}/job_$(Process)/job.out\n"
        jdl += f"Error = jobs/{sample_name}/job_$(Process)/job.err\n"
        jdl += f'transfer_output_remaps = "ntuple_total.root = EFT_LHE/gp_ntuples/ntuple_{sample_name}_$(Process).root"\n'
        jdl += f"Queue {nJobs} \n"

        with open(f"lheReaderFolder/submit_{sample_name}.jdl","w") as file:
            file.write(jdl)

        workingPath = os.path.abspath(".")
        os.chdir("lheReaderFolder/")
        process = subprocess.Popen(f"condor_submit submit_{sample_name}.jdl", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        process.wait()
        os.chdir(workingPath)


        print("\nSubmitted jobs to condor")
        

    elif args.doHadd:
        rootFiles = glob.glob("lheReaderFolder/EFT_LHE/gp_ntuples/ntuple_{}_*.root".format(sample_name))
        command = "hadd -j {} -f".format(args.nThreads)
        command += " lheReaderFolder/EFT_LHE/gp_ntuples/ntuple_{}.root".format(sample_name)
        command += " " + ' '.join(rootFiles)
        process = subprocess.Popen(command, shell=True)
        process.wait()
    
        # # correcting xs in _nums histo
        XS = sum ([x / (e*e) for x, e, f in xs_files])
        sumW = sum ([1. / (e*e) for x, e, f in xs_files])
        XS = XS / sumW
        rootFile = "lheReaderFolder/EFT_LHE/gp_ntuples/ntuple_{}.root".format(sample_name)
        f = ROOT.TFile(rootFile, "UPDATE")
        histoName = "{}_nums".format(sample_name)
        h1 = f.Get(histoName)
        h1.SetBinContent(1, XS)
        h1.Write()
