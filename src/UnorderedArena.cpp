#include "UnorderedArena.hpp"
#include "raylib.h"

template<typename T>
UnorderedArena<T>::UnorderedArena(int capacity)
  : capacity_(capacity)
  , size_(0)
{
    data = new T[capacity];
}

template<typename T>
UnorderedArena<T>::~UnorderedArena()
{
    delete[] data;
}

template<typename T>
void UnorderedArena<T>::Add(T elem)
{
    if (size >= capacity) {
        TraceLog(LOG_ERROR, "UnorderedArena::Add arena is full");
        return;
    }

    data[size] = elem;
    size_++;
}

template<typename T>
void UnorderedArena<T>::Remove(int index)
{
    if (index >= size) {
        TraceLog(LOG_ERROR, "UnorderedArena::Remove index out of bounds");
        return;
    }

    // no need to swapback if only a single element
    if (size > 1) {
        data[index] = data[size - 1];
    }

    size_--;
}
