// Signature must be retained. Implementation must be added.

#ifndef SORT_H
#define SORT_H

#include "Exceptions.h"
#include "ResizableArray.h"
#include "Comparator.h"
// debug get rid of!

template <typename T>
class Sorter {
private:
    // additional member functions (methods) and variables (fields) may be added
	static void swap(int a, int b, ResizableArray<T>* array);
	static void quicksort(ResizableArray<T>& array, const Comparator<T>& comparator, int low, int high);
	static int partition(ResizableArray<T>& array, const Comparator<T>& comparator, int low, int high);
public:
    static void sort(ResizableArray<T>& array, const Comparator<T>& comparator);
	
};

// implementation goes here

//// simple swap method to avoid overcrowding the bubble sort algorithm!
//template <typename T>
//void Sorter<T>::swap(T* elem1, T* elem2)
//{
//	T temp = *elem1;
//	*elem1 = *elem2;
//	*elem2 = temp;
//}

// simple swap method to avoid overcrowding the bubble sort algorithm!
template <typename T>
void Sorter<T>::swap(int a, int b, ResizableArray<T>* array)
{ 
	T temp = array->get(a);
	array->replaceAt(array->get(b), a);
	array->replaceAt(temp, b);
}

template <typename T>
void Sorter<T>::quicksort(ResizableArray<T>& array, const Comparator<T>& comparator, int low, int high)
{
	if (low < high)
	{
		int pi = partition(array, comparator, low, high);
		quicksort(array, comparator, low, pi - 1);
		quicksort(array, comparator, pi + 1, high);
	}
	return;
}

template <typename T>
int Sorter<T>::partition(ResizableArray<T>& array, const Comparator<T>& comparator, int low, int high)
{
	T pivot = array[high];
	int i = (low - 1);

	for (int j = low; j <= high - 1; j++)
	{
		if (comparator.compare(array[j], pivot) == 0 || comparator.compare(array[j], pivot) == -1)
		{
			i++;
			swap(i, j, &array);
		}
	}
	swap(i + 1, high, &array);
	return (i + 1); 
}

// I am using the bubble sort algorithm. some of this code is similar to the bubble sort code on the slides from the class.
template <typename T>
void Sorter<T>::sort(ResizableArray<T>& array, const Comparator<T>& Comparator)
{
	quicksort(array, Comparator, 0, (signed int)array.getSize() - 1);
	/*for (unsigned int i = 0; i < array.getSize() - 1; i++)
	{
		for (unsigned int j = 0; j < array.getSize() - i - 1; j++)
		{
			if (Comparator.compare(array[j], array[j + 1]) == 1)
			{
				
				swap(j, j + 1, &array);
			} 
		}
	}*/
}

#endif