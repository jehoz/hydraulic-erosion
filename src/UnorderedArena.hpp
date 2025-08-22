#ifndef UNORDERED_ARENA_HPP
#define UNORDERED_ARENA_HPP

#include <cstddef>
#include <iterator>

/*! A fixed-capacity arena that uses swapback for removals, making it pretty
 * quick to remove arbitrary elements, but it doesn't preserve the order of its
 * contents.
 */
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

    void Add(T);
    void Remove(int index);

  private:
    T* data;

    int size_;
    int capacity_;
};

#endif
