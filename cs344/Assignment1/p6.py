#! /usr/bin/python
#Name: Jared Wasinger
#ONID Email:  wasingej@onid.oregonstate.edu
#Class CS344-001
#Assignment #1

import sys, getopt, os, pdb, subprocess

def get_opt_value(opt_str, opts):
    for opt in opts:
        if opt[0] == opt_str:
            return opt[1]
    
    return None

def mkdir_p(path_to_dir, dir_name):
    try:
        os.makedirs(path_to_dir + dir_name)
    except OSError:
        print "directory: " + dir_name + " already exists"

if __name__ == "__main__":
    optlist, args = getopt.getopt(sys.argv[1:], 't:c:', ['term=', 'class='])
    
    if get_opt_value('-t', optlist) and get_opt_value('--term', optlist):
        print "error argument supplied twice"
        quit(1)

    if get_opt_value('-c', optlist) and get_opt_value('--class', optlist):
        print "error argument supplied twice"
        quit(1)
    
    term = ''
    cls = ''
   
    if get_opt_value('-c', optlist):
        cls = get_opt_value('-c', optlist)
    elif get_opt_value('--class', optlist):
        cls = get_opt_value('--class', optlist)
    else:
        print "error class argument requred"
        quit(1)

    if get_opt_value('-t', optlist):
        term = get_opt_value('-t', optlist)
    elif get_opt_value('--term', optlist):
        term = get_opt_value('--term', optlist)
    else:
        print "error, term argument required"
        quit(1)
    
    mkdir_p(term + '/' + cls + '/', 'assignments')
    mkdir_p(term + '/' + cls + '/', 'examples')
    mkdir_p(term + '/' + cls + '/', 'exams')
    mkdir_p(term + '/' + cls + '/', 'lecture_notes')
    mkdir_p(term + '/' + cls + '/', 'submissions')
    
    cmd_str = 'ln -s /usr/local/classes/eecs/' + term + '/' + cls + '/src src_class'
    subprocess.call(cmd_str, shell=True) 
