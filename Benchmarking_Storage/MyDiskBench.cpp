#include <iostream> 
//For pthreads
#include <thread>
//For accurate time
#include <chrono>
//For open, close etc.,
#include <cstdlib>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
//For string functions
#include <cstring>
#include <string>
#include <bits/stdc++.h>
//For floor
#include <cmath>
//For itoa
#include <stdlib.h>

//Access patterns
#define READ_SEQUENTIAL 0
#define READ_RANDOM 1
#define WRITE_SEQUENTIAL 2
#define WRITE_RANDOM 3

/*
    Note to TA: 
        1. This code is not intended to be used directly by the end user since the 
        output of this program is not user friendly. It returns the time taken for reading/writing
        by each thread. The wrapper program MyDiskBenchWrapper uses this file to extract
        this output in terms of time and calculates throughput in MBPS and IOPS.
        2. chrono timer is used from "chrono" library to get the time accurately
        3. Block, page and record are used synonymously in this code.
        4. Flags used:
            -r for record size
            -t for number of threads
            -s size of the file to test
            -F list of all files like "-F filename filename filename"
            -i for pattern
        5. The program uses POSIX threads by default and does direct I/O operations.
        6. 
 */

//The functions getRandomValue and getRandomeBlockNumber are used to generate random record number to read/write
double getRandomValue(void){
    return ((double)rand()/(RAND_MAX));
}
int getRandomBlockNumber(int total_blocks){
    return std::floor(int(getRandomValue()*total_blocks));
}

//Function that reads the file in the given access pattern
void readFile(char* file,int access_pattern,long long file_size,long block_size) 
{ 
    int fp = open(file,O_RDWR|O_DIRECT);
    if(fp == -1){
        std::cout<<"Unable to open the file : "<<file;
        exit(0);
    }

    //Buffer to store the data that is read
    char* buf = (char *)aligned_alloc(block_size,block_size);
    double time_taken;

    //If the read is sequential, keep reading the file until you reach the end of the file
    //and measure the time taken for this.
    if(access_pattern == READ_SEQUENTIAL){
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        while(read(fp,buf,block_size)){
        };
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        time_taken = time_span.count();
    }
    //If the read is random, then select a random block on the file, use lseek to go to that location
    //and then read the block. 
    if(access_pattern == READ_RANDOM){
        //Find the number of read operations to perform
        long num_reads = long(file_size/block_size);
        long count = 0;
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();        
        while(count < num_reads){
            //Generate a random block number
            int blockNum = getRandomBlockNumber(long(file_size/block_size));
            //Use seek to go to given position
            lseek(fp,blockNum*block_size,SEEK_SET);
            read(fp,buf,block_size);
            count++;
        };
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        time_taken = time_span.count();
    }
    //Print the time taken.
    std::string outputstring;
    outputstring = " It took these many seconds : " + std::to_string(time_taken);
    outputstring += " \n";
    std::cout<<outputstring;    
}

//Function that writes to the file in the given access pattern
void writeFile(char* file,char* content,int access_pattern,long long file_size,int block_size){
    
    int fp = open(file,O_CREAT|O_RDWR|O_DIRECT,0600);
    if(fp == -1){
        std::cout<<"Can't open the file!";
        exit(0);
    }
    //Number of writes to create a file of given size
    long num_of_writes = (long)file_size/block_size;
    double time_taken;
    //For sequential writing, keep writing to the page from file pointer's current position until the file size is reached
    if(access_pattern == WRITE_SEQUENTIAL){
        long count = 0;
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        while(count < num_of_writes){
            write(fp,content,block_size);
            count++;
        };
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        time_taken = time_span.count();
    }
    
    //For random writing, select a block randomly and use lseek to move the pointer to the block
    //and perform write operation
    if(access_pattern == WRITE_RANDOM){
        long count = 0;
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        while(count < num_of_writes){
            //Generate a random number
            int blockNum = getRandomBlockNumber(long(file_size/block_size));
            //Use seek to go to given position
            lseek(fp,blockNum*block_size,SEEK_SET);
            write(fp,content,block_size);
            count++;
        };

        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        time_taken = time_span.count();
    }

    std::string outputstring;
    outputstring = " It took these many seconds : " + std::to_string(time_taken);
    outputstring += " \n";
    std::cout<<outputstring;    
}

int main(int argc, char* argv[]) 
{ 
    // -r for record size
    // -t for number of threads
    // -T is not required because we are using POSIX threads by default
    // -s size of the file to test
    // -F list of all files like "-F filename filename filename"
    // -I to use DIRECT I/O for all file operations. This is done by default
    // -z To include small record sizes. This is not required!
    // -O gives results in operations per second
    // -i for pattern

    int num_of_threads = -1;
    long record_size = -1;
    
    // 0 for sequential read
    // 1 for random read
    // 2 for sequential write
    // 3 for random write
    
    int access_pattern = -1;
    long long size_of_file = -1;

    //Get all the arguments except the list of files
    for (int i = 0; i < argc; i++)
    {
        if(i+1 != argc){
            if(strcmp(argv[i],"-t") == 0){
                num_of_threads = std::stoi(argv[i+1]);
            }
            if(strcmp(argv[i],"-r") == 0){
                record_size = std::stol(argv[i+1]);
            }
            if(strcmp(argv[i],"-i") == 0){
                access_pattern = std::stoi(argv[i+1]);
            }
            if(strcmp(argv[i],"-s") == 0){
                size_of_file = std::stoll(argv[i+1]);
            }
        }
    }

    //List of files. There will be one thread for one file
    char *list_of_files[num_of_threads]; 

    //List of files
    for (int i = 0; i < argc; i++)
    {
        if(strcmp(argv[i],"-F") == 0){
            int count = 0;        
            //MODIFY THIS TO DETECT THE NUMBER OF CORRECTLY GIVEN FILES
            while(count < num_of_threads){
                list_of_files[count] = (char*)malloc(sizeof(argv[i+1])+1);
                strcpy(list_of_files[count],argv[i+1]);
                count++;
                i++;
            }
            break;
        }
    }

    //Throw an error if arguments are not given correctly
    if(num_of_threads == -1 || access_pattern == -1 || record_size == -1 || size_of_file == -1){
        std::cout<<"Incorrect Usage of the command"<<std::endl;
    }

    std::thread th[num_of_threads];

    //Create threads and run the readFile function in each thread
    if(access_pattern == READ_RANDOM || access_pattern == READ_SEQUENTIAL){
        for (size_t i = 0; i < num_of_threads ;i++){
            th[i] = std::thread(readFile,(char*)list_of_files[i],access_pattern,size_of_file,record_size);
        }
    }

    //Create threads and run the writeFile function in each thread
    if(access_pattern == WRITE_RANDOM || access_pattern == WRITE_SEQUENTIAL){
        //Create block content
        char* content = (char *)aligned_alloc(record_size,record_size);
        //The content here holds the content that has to be written to the disk
        //Here initializing to character a
        for (size_t i = 0; i < record_size; i++)
        {
            content[i] = 'a';
        }

        //Create dummy files. One per thread to use for writing
        for (size_t i = 0; i < num_of_threads ;i++){
            std::string file = "dummy_";
            std::string file_num = std::to_string(i);
            std::string dummy_file = file+file_num;
            int n = dummy_file.length();
            char* filearray = (char *)malloc(n+1);
            strcpy(filearray,dummy_file.c_str());
            strcat(filearray,".bin");
            list_of_files[i] = filearray;
            th[i] = std::thread(writeFile,(char*)list_of_files[i],content,access_pattern,size_of_file,record_size);
        }
    }
        
    //Join all the threads.
	for (size_t i = 0; i < num_of_threads; i++)
	{
		th[i].join();
	}
} 


