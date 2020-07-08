#!/usr/bin/python

import commands
import fileinput
import sys
import os
import shutil


def create_job_sub (resultfile, sourcefile, replace):
    fin = open (sourcefile, 'r')
    data = fin.read ()
    fin.close ()
    for elem in replace:
        data=data.replace (elem[0], elem[1])
    fout = open (resultfile, 'w')
    fout.write (data)
    fout.close ()


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


if __name__ == "__main__":

    if (len(sys.argv) < 3):
        print 'usage: submit.py process_folder_relative_path output_base_folder [events(10000)] [jobs(100)] [shell script(job.sh)]'
        sys.exit (1)

    process_folder = sys.argv[1]
    # check that it exists
    if (not os.path.exists(os.getcwd () + '/' + process_folder)):
        print ('the folder ' + process_folder + ' does not exist, exiting\n')
        sys.exit (1)

    process_name = sys.argv[1].split ('/')[-1]

    results_folder = sys.argv[2] + '/' + process_name + '_results'

    if (os.path.exists(results_folder)):
        content = os.listdir (results_folder)
        if len (content) > 0:
            print ('the folder ' + results_folder + ' exists not empty, exiting\n')
            sys.exit (1)
    else:
        os.mkdir (results_folder)

    events_number  = '10000'
    if (len (sys.argv) > 3):
        events_number = sys.argv[3]

    jobs_number  = '100'
    if (len (sys.argv) > 4):
        jobs_number = sys.argv[4]

    shell_script_template   = 'job.sh'
    if (len (sys.argv) > 5):
        shell_script_template = sys.argv[5]
    if (not os.path.exists(os.getcwd () + '/' + shell_script_template)):
        print ('the file ' + shell_script_template + ' does not exist, exiting\n')
        sys.exit (1)

    job_file = results_folder + '/' + shell_script_template
    shutil.copy2 (shell_script_template, job_file)

    replace = [
        ['PROCESS_NAME_CHANGEME' , process_name  ],
        ['BASE_FOLDER_CHANGEME'  , os.getcwd ()  ],
        ['EVENTS_NUMBER_CHANGEME', events_number ],
        ['JOBS_NUMBER_REPLACEME' , jobs_number   ],
        ['EXECUTABLE_CHANGEME'   , job_file      ]
      ]
    submit_file = results_folder + '/' + process_name + '.sub'
    create_job_sub (submit_file, 'example.sub', replace)

    # submit the condor job
    os.system ('condor_submit ' + submit_file)

