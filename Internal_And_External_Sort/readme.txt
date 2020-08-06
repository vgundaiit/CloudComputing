The code for this assignment is divided into following source files:
    1. MySort.cpp
    2. quicksort.cpp
    3. minheap.cpp

The MySort.cpp program is the major code that handles sort. quicksort.cpp
has implementation of quicksort algorithm and minheap.cpp if helper file 
for implementing of minheap. 

This program is designed for sorting inputs generated using gensort. 

Compilation:
    To compile and create the executable, use the following command:
        make all

    This will run the following command:
        g++ -std=c++11 -g -pthread quicksort.cpp MySort.cpp minheap.cpp -o ms.out

    As seen in the command above, we used c++11 and pthreads library for the 
    implementation.

Running:
    To run the executable, use the following command:
    ./ms.out -F [file_name] -t [number of threads]

    file_name is the input file name that is generated using gensort
    You can specify the number of threads for external sorting but if your file fits in the memory,
    only 1 thread will be used. By default, 48 threads are used for external sorting.

    Once this runs, the executable ms.out creates a file sorted-data to which sorted data
    is written. Also, for external sorting, this executable clears the input file since in some of chameleon nodes, we observed
    there is space issue for 64 gb file size. Hence, the executable after creating chunks and before merging them
    deletes the input file. Also, when you run this command second time, the sorted-data file will be overwritten
    with new sorted-data.