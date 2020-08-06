#!/usr/bin/env python3

"""

Note to TA:

    This is a wrapper function that calls MyDiskBench.cpp. This script takes care of parsing the 
    arguments and collecting the output from MyDiskBench.cpp and presenting it to the users
    in readable format i.e., IOPS and Throughput in MBPS

    Record size and file size can be given in bytes of KBs or MBs or GBs
    Access patterns are:
        0 for sequential read
        1 for random read
        2 for sequential write
        3 for random write

    No need to give list of files using -F if the user is trying sequential write or random wite operations

"""

import sys
import os
import re

listOfFiles = ""
numThreads = -1
access_pattern = -1
recSize = -1
fileSize = -1

if(len(sys.argv) < 3):
    print ("Missing arguments... \nUsage ./MyDiskBenchWrapper.py -i [access pattern] -r [record size] -F [list of files] -s [file size] -t [number of threads]")
    exit()


#Parse the input arguments from the command line
for i in range(len(sys.argv)):
    if i+1 != len(sys.argv):
        if sys.argv[i] == '-t':
            numThreads = sys.argv[i+1]
        if sys.argv[i] == '-r':
            recSize = sys.argv[i+1]
        if sys.argv[i] == '-i':
            access_pattern = sys.argv[i+1]     
        if sys.argv[i] == '-s':
            fileSize = sys.argv[i+1]

if(access_pattern == -1 or numThreads == -1 or fileSize == -1 or recSize == -1):
    print ("Missing arguments...\nUsage ./MyDiskBenchWrapper.py -i [access pattern] -r [record size] -F [list of files] -s [file size] -t [number of threads]")
    exit()

numThreadsInt = int(numThreads)

if recSize[-1] == "G" or recSize[-1] == "g":
    recSizeInt = float(recSize[0:-1])*1024*1024*1024
elif recSize[-1] == "M" or recSize[-1] == "m":
    recSizeInt = float(recSize[0:-1])*1024*1024
elif recSize[-1] == "K" or recSize[-1] == "k":
    recSizeInt = float(recSize[0:-1])*1024
else:
    recSizeInt = float(recSize)

access_patternInt = int(access_pattern)

if fileSize[-1] == "G" or fileSize[-1] == "g":
    fileSizeInt = float(fileSize[0:-1])*1024*1024*1024
elif fileSize[-1] == "M" or fileSize[-1] == "m":
    fileSizeInt = float(fileSize[0:-1])*1024*1024
elif fileSize[-1] == "K" or fileSize[-1] == "k":
    fileSizeInt = float(fileSize[0:-1])*1024
else:
    fileSizeInt = float(fileSize)

for i in range(len(sys.argv)):
    if sys.argv[i] == '-F':
        count = 0
        numThreadsInt = int(numThreads)
        while count < numThreadsInt:
            listOfFiles += " " + sys.argv[i+1]
            i += 1
            count += 1

if ((access_patternInt == 0 or access_pattern == 1) and len(listOfFiles) == 0):
    print ("List of files not given")    
    exit()    

#Run the underlying MyDiskBench.cpp
if (access_patternInt == 2 or access_patternInt == 3):
    cmd = './mdb.out -t '+ str(numThreads) +' -r ' + str(recSizeInt) + ' -s ' + str(fileSizeInt) + ' -i '+ str(access_pattern)
else:
    cmd = './mdb.out -t '+ str(numThreads) +' -r ' + str(recSizeInt) + ' -s ' + str(fileSizeInt) + ' -i '+ str(access_pattern) + ' -F'+ listOfFiles

#Run the command
output = os.popen(cmd).read()

#Take average to find the average value
totalTime = 0
avgTime = 0

arr = re.split('[:\n]',output)

for i in range(1,len(arr),2):
    totalTime += float(arr[i])

#Average time is the the average of time taken by all the threads
avgTime = float(totalTime/numThreadsInt)

#This gives the average throughput combining the throughput of all the threads
#Convert time to MBPS = (size_of_file) * (number of threads)/ (time taken * 1024 *1024)
speed = (fileSizeInt * numThreadsInt)/(avgTime*1024*1024)

print("Average throughput in MBPS : " + str(round(speed,3)) +" MBPS")

#Convert time to IOPS = (size_of_file)/(rec_size * time_taken) 
iops = (fileSizeInt*numThreadsInt)/(recSizeInt*avgTime)
print("Iops: " + str(round(iops,3)) + " IOPS")

#Delete dummy_* files created by MyDiskBenchWrapper.cpp
cmd = "rm -f dummy_*"
deleteoutput = os.popen(cmd).read()

