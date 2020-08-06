The program MyDiskBench.cpp is used for measuring disk performace. This
is similar to IOZone.

====================================================================================================

Running the Makefile:

1. Please execute the make file using the following command to create an executable mdb.out

make 

2. This mdb.out is the executable that we will use to measure disk performance.
3. Use the following command if there are any dummy_* files that were left (Typically they won't) because of interruption of running operation of
mdb.out.

======================================================================================================

External Libraries:

1. I am using the followin libraries for MyDiskBench.cpp:

    #include <iostream> 
    
    For pthreads:
    #include <thread>
    
    For accurate time:
    #include <chrono>

    For open, close etc.,:
    #include <cstdlib>
    #include <sys/types.h>
    #include <fcntl.h>
    #include <unistd.h>
    
    For string functions:
    #include <cstring>
    #include <string>
    #include <bits/stdc++.h>
    
    For floor:
    #include <cmath>
    
    For itoa:
    #include <stdlib.h>

2. Using c++11 and -pthread in the compilation command is sufficient to get all the libraries. This can be seen in the 
Makefile

===============================================================================================================

Running the Program:

1. I have created a wrapper MyDiskBenchWrapper.py. This uses the cpp file's executable mdb.out, passes the arguments to it 
    and extracts the output from mdb.out and presents to user in readable format.
    Note that the sizes of record and file must be integral multiple of MegaBytes or Giga Bytes
    This can be run using the following command:

    For Sequential Writing of file with 100MB size and 1MB record size with 1 thread:
        python3 MyDiskBenchWrapper.py  -t 1 -r 1m -s 100m -i 2    

    For Random Writing with 2 threads:
        python3 MyDiskBenchWrapper.py  -t 2 -r 1m -s 100m -i 3    

    For Sequential Reading of files dummy_0.bin dummy_1.bin dummy_2.bin and dummy_3.bin with 4 threads.
    Please note file_* files here have to be created before running this command.
        python3 MyDiskBenchWrapper.py  -t 4 -r 1m -s 100m -i 0 -F file_0.bin file_1.bin file_2.bin file_3.bin   


    For Random reading:
    Please note file_* files here have to be created before running this command.
        python3 MyDiskBenchWrapper.py  -t 4 -r 1m -s 100m -i 1 -F file_0.bin file_1.bin file_2.bin file_3.bin   

    The record size and file size can be give in terms of mega bytes or giga bytes. For example, -s 1g for 1 GB or -r 10m for 10 Megabytes


2. The executable takes the following flags:

    // -r for record size
    // -t for number of threads
    // -s size of the file to test (in bytes)
    // -F list of all files like "-F filename filename filename". This number should match with the number of threads
    // -i for pattern

This program does direct disk I/O operations by default. 
