#include "unordered_array.hpp"

template<typename T, std::size_t N>
unordered_array<T, N>::unordered_array()
{
    data = new T[N];
    size_ = 0;
    capacity_ = N;
}

template<typename T, std::size_t N>
unordered_array<T, N>::~unordered_array()
{
    delete[] data;
}

template<typename T, std::size_t N>
void unordered_array<T, N>::add(T elem)
{
    if (size_ < capacity_) {
    }
}
