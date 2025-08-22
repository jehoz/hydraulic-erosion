#ifndef UNORDERED_ARENA_HPP
#define UNORDERED_ARENA_HPP

#include <cstddef>
#include <iterator>

template<typename T>
struct UnorderedArena
{
    struct Iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using differece_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
    };

    const int& size = size_;
    const int& capacity = capacity_;

    UnorderedArena<T>(int capacity);
    ~UnorderedArena();

    void add(T);

  private:
    T* data;

    int size_;
    int capacity_;
};

#endif
