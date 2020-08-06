#include <iostream>
#include <algorithm>
#include <queue>
#include <limits>
#include <fstream>
#include <cstring>
#include "minheap.h"

using namespace std;


bool comp::operator()(const MinHeapNode lhs, const MinHeapNode rhs) const{
    int res = strcmp(lhs.element,rhs.element);
    bool op;
    if(res > 0){
        return true;
    }
    else{
        return false;
    }
}

