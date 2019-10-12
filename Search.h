// Signature must be retained. Implementation must be added.

#ifndef SEARCH_H
#define SEARCH_H

#include "Exceptions.h"
#include "ResizableArray.h"
#include "Comparator.h"
// returns FIRST array location matching the given item (based on the comparator)
// if not found, returns -(location to insert + 1)
template <typename T>
long long binarySearch(const T& item, const ResizableArray<T>& array, const Comparator<T>& comparator) { // still working on this!
	long int left = 0, right = array.getSize() - 1;
	long long mid = -1;
	while (left <= right)
	{
		
		mid = (left + (right)) / 2;
		if (comparator.compare(array[mid], item) == 0)
		{
			while (--mid != -1)
			{
				if (comparator.compare(array[mid], item) != 0)
				{
					return ++mid;
				}
			}
			if (mid == -1)
			{
				return ++mid;
			}
			else
			{
				return mid;
			}
		}
		if (comparator.compare(array[mid], item) < 0)
		{
			left = mid + 1;
		}
		else
		{
			right = mid - 1;
		}
	}
	
	return (long long)-(mid - 1);
}

#endif