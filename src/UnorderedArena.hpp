#ifndef UNORDERED_ARENA_HPP
#define UNORDERED_ARENA_HPP

#include "raylib.h"
#include <cstddef>

/*! A fixed-capacity arena that uses swapback for removals, making it pretty
 * quick to remove arbitrary elements, but it doesn't preserve the order of its
 * contents.
 */
template<typename T>
class UnorderedArena
{

    T* data;

    size_t capacity_;
    size_t size_;

  public:
    const size_t& capacity = capacity_;
    const size_t& size = size_;

    UnorderedArena(size_t capacity)
      : capacity_(capacity)
      , size_(0)
    {
        data = new T[capacity];
    }

    ~UnorderedArena() { delete[] data; }

    T& operator[](size_t i) { return data[i]; }

    using iterator = T*;
    iterator begin() { return &data[0]; }
    iterator end() { return &data[size_]; }

    void Add(T elem)
    {
        if (size >= capacity) {
            TraceLog(LOG_ERROR, "UnorderedArena::Add arena is full");
            return;
        }

        data[size] = elem;
        size_++;
    }

    void Remove(size_t index)
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
};

#endif
