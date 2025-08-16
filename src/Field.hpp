#ifndef FIELD_HPP
#define FIELD_HPP

#include "raylib-cpp.hpp"

template<typename T>
class Field
{
  public:
    int width, height;

    Field(int width, int height);
    ~Field();

    T GetCell(int x, int y);
    T SetCell(int x, int y, T value);

    T GetAtPoint(raylib::Vector2 position);

  private:
    T* data;
};

#endif
