#include "UnorderedArena.hpp"

template<typename T>
UnorderedArena<T>::UnorderedArena(int capacity)
{
    data = new T[capacity];
    size_ = 0;
    capacity_ = N;
}

template<typename T>
UnorderedArena<T>::~UnorderedArena()
{
    delete[] data;
}

template<typename T>
void UnorderedArena<T>::add(T elem)
{
    if (size_ < capacity_) {
    }
}
