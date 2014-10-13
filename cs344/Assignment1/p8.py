#! /usr/bin/python

import subprocess, sys, pdb

if __name__ == "__main__":
    args_str = ''
    for arg in sys.argv[1:]:
        args_str += arg + ' '
    
    print subprocess.call("cal"+' ' + args_str, shell=True)
