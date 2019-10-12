#ifndef RESIZABLE_ARRAY_H
#define RESIZABLE_ARRAY_H

#include "Exceptions.h"

const unsigned long DEFAULT_ARRAY_CAPACITY = 10;        // capacity used in no arg constructor

template <typename T>
class ResizableArray {
private:
    unsigned long capacity = DEFAULT_ARRAY_CAPACITY;    // maximum capacity, in items
    unsigned long size = 0;                             // actual number of items currently in array
    T* data = NULL;                                     // pointer to array of any type
    void doubleCapacity();                              // method to double array capacity
    void halveCapacity();                               // method to halve array capacity
public:
    ResizableArray();                                   // constructs array with default capacity
    ResizableArray(unsigned long capacity);             // constructs array with specified capacity
    virtual ~ResizableArray();                          // frees array space as object is deleted
    void add(T item);                                   // adds item, increments size, doubles capacity as necessary
    void addAt(T item, unsigned long index);            // adds item at index, shifts following, doubles capacity as necessary
    void replaceAt(T item, unsigned long index);        // replaces item at index, otherwise unchanged
    void removeAt(unsigned long index);                 // removes item at index, shifts following back
    T get(unsigned long index) const;                   // returns (copy of) item at index
    unsigned long getSize() const;                      // returns number of items currently in array
    unsigned long getCapacity() const;                  // returns the current capacity of the array
    T operator[](unsigned long index) const;            // returns (copy of) item at index
    void clear();                                       // removes all items from array, resets to default capacity, set size to 0
};

// Add your implementation below this line. Do not add or modify anything above this line.

// Private methods

// method to double array capacity
template <typename T>
void ResizableArray<T>::doubleCapacity()
{
	T* new_data = new T[this->capacity * 2];
	if (new_data == NULL)
	{
		throw new ExceptionMemoryNotAvailable();
	}
	for (unsigned long index = 0; index < this->capacity; index++)
	{
		new_data[index] = this->data[index];
	}
	delete[] this->data;
	this->data = new_data;
	this->capacity = this->capacity * 2;
}

// method to halve array capacity
template <typename T>
void ResizableArray<T>::halveCapacity()
{
	T* new_array = new T[(long)(this->capacity / 2l)];
	if (new_array == NULL)
	{
		throw new ExceptionMemoryNotAvailable();
	}
	for (unsigned long index = 0; index < this->size; index++)
	{
		new_array[index] = this->data[index];
	}
	delete[] this->data;
	this->data = new_array;
	this->capacity = (long)(this->capacity / 2);
}

// Public methods

// constructs array with default capacity
template <typename T>
ResizableArray<T>::ResizableArray()
{
	this->data = new T[this->capacity];
}

// constructs array with specified capacity
template <typename T>
ResizableArray<T>::ResizableArray(unsigned long capacity)
{
	this->data = new T[capacity];
	if (this->data == NULL)
	{
		throw new ExceptionMemoryNotAvailable();
	}
	this->capacity = capacity;
}

// frees array space as object is deleted
template <typename T>
ResizableArray<T>::~ResizableArray()
{
	delete[] this->data;
	this->data = NULL;
}

// adds item, increments size, doubles capacity as necessary
template <typename T>
void ResizableArray<T>::add(T item)
{
	if (this->capacity == this->size)
	{
		this->doubleCapacity();
	}
	this->data[size++] = item;
}

// adds item at index, shifts following, doubles capacity as necessary
template <typename T>
void ResizableArray<T>::addAt(T item, unsigned long index) // should double if neccesary.
{
	if (index >= this->size) // only work with filled portion (size)
	{
		throw new ExceptionIndexOutOfRange(); // EOM.
	}
	if (this->size + 1 > this->capacity)
	{
		this->doubleCapacity();
	}
	T* new_array = new T[this->size + 1];
	for (unsigned long i = 0; i < index; i++)
	{
		new_array[i] = this->data[i];
	}
	new_array[index] = item;
	for (unsigned long i = index + 1; i < this->size + 1; i++)
	{
		new_array[i] = this->data[i - 1];
	}
	this->size++;
	delete[] this->data;
	this->data = new_array;
}

// replaces item at index, otherwise unchanged
template <typename T>
void ResizableArray<T>::replaceAt(T item, unsigned long index)
{
	if (index >= this->size) // only work with filled portion (size)
	{
		throw new ExceptionIndexOutOfRange(); // EOM.
	}
	// delete this->data[index];
	this->data[index] = item;
}

// removes item at index, shifts following back
template <typename T>
void ResizableArray<T>::removeAt(unsigned long index) // should halve is neccesary.
{
	if (index >= this->size) // only work with filled portion (size)
	{
		throw new ExceptionIndexOutOfRange(); // EOM.
	}
	for (unsigned long i = index; i < size - 1; i++)
	{
		// delete this->data[i];
		this->data[i] = this->data[i + 1];
	}
	// delete this->data[(int)size - 1];
	this->size--;
	// check to possible halving!
	if (this->capacity > 1 && (this->capacity / 2) >= size)
	{
		this->halveCapacity();
	}
}

// returns (copy of) item at index
template <typename T>
T ResizableArray<T>::get(unsigned long index) const
{
	if (index >= this->size) // only work with filled portion (size)
	{
		throw new ExceptionIndexOutOfRange(); // EOM.
	}
	return this->data[index];
}

// returns number of items currently in array
template <typename T>
unsigned long ResizableArray<T>::getSize() const
{
	return this->size;
}

// Driller v1.0 TODO

template <typename T>
T ResizableArray<T>::operator[](unsigned long index) const
{
	return this->get(index);
}

template <typename T>
unsigned long ResizableArray<T>::getCapacity() const
{
	return this->capacity;
}



#endif