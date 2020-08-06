#include <iostream>
#include <bits/stdc++.h> 
#include <string>
#include <cstring>
#include "quicksort.h"

using namespace std;  
  

void swap(char **s,char **t){
    char* temp = *s;
    *s = *t;
    *t = temp;
}

//partition function
long long partition(char** arr,long low, long high){
    char* pivot = arr[high];
    long long i = low-1;

    //In a loop, compare every element with pivot 
    //and if the current element is small, swap it with  
    //element at index i
    string ps = string(pivot);
    for (long long j = low; j < high; j++)
    {
        string js = string(arr[j]);
        
        if(js.compare(0,10,ps) < 0){
        //if(strncmp(arr[j],arr[i],10)){
            i++;
            swap(&arr[i],&arr[j]);
        }
    }
    swap(arr[i+1],arr[high]);
    return i+1;
}   

//sorting function
void QuickSortHelper(char** arr,long long low,long long high){
    if(low<high){
        //Find the partitioning
        long long pi = partition(arr,low,high);
        QuickSortHelper(arr,low,pi-1);
        QuickSortHelper(arr,pi+1,high);
    }
}

void QuickSort(char** arr,long long low,long long high){
    QuickSortHelper(arr,low,high);
}


void printArray(string arr[], long long size)  
{  
    int i;  
    for (i = 0; i < size; i++)  
        cout << arr[i] << " ";  
    cout << endl;  
}
