#ifndef UNORDERED_ARRAY_HPP
#define UNORDERED_ARRAY_HPP

#include <cstddef>
#include <iterator>

template<typename T, std::size_t N>
struct unordered_array
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

    unordered_array<T, N>();
    ~unordered_array();

    void add(T);

  private:
    T* data;

    int size_;
    int capacity_;
};

#endif
