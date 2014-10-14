#! /usr/bin/python

#Name: Jared Wasinger
#ONID Email:  wasingej@onid.oregonstate.edu
#Class: CS344-001
#Assignment #1


import sys, urllib2, pdb

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("error, 2 arguments must be supplied")
        quit(1)
    
    args_list = sys.argv

    response = urllib2.urlopen(args_list[1])
    resource = response.read()
    
    file_obj = open(sys.argv[2], 'w+')
    file_obj.write(resource)
    file_obj.close()
