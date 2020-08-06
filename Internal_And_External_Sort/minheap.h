#ifndef MINHEAP_H
#define MINHEAP_H

struct MinHeapNode
{
	// The element to be stored
	char* element;

	// index of the array from which the element is taken
	int i;
};

struct comp
{
	bool operator()(const MinHeapNode lhs, const MinHeapNode rhs) const;
};

#endif