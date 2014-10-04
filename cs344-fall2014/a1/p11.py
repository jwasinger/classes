import sys

for line in sys.stdin:
    print line

quit()

if not line:
    #read from keyboard
    print "hello world!"

while line:
    print "lines!"
