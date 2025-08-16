#include "Field.hpp"
#include <cassert>

template<typename T>
struct FieldPoint
{
    // bounding coordinates of point
    int y0, y1, x0, x1;

    // distance from (x0,y0) to point
    raylib::Vector2 offset;

    // value of bounding cells on grid
    T nw, ne, sw, se;

    FieldPoint(Field<T>* parent, raylib::Vector2 position)
    {
        float y_ipart;
        float y_fpart = std::modf(position.y, &y_ipart);
        float x_ipart;
        float x_fpart = std::modf(position.x, &x_ipart);

        y0 = static_cast<int>(y_ipart);
        y1 = y0 + 1 < parent->height ? y0 + 1 : y0;
        x0 = static_cast<int>(x_ipart);
        x1 = x0 + 1 < parent->width ? x0 + 1 : x0;

        // TODO populate bounding cell values
    }
};

template<typename T>
Field<T>::Field(int width, int height)
  : width(width)
  , height(height)
{
    data = new T[width * height];
}

template<typename T>
Field<T>::~Field()
{
    delete[] data;
}

template<typename T>
T Field<T>::GetCell(int x, int y)
{
    int i = index(x, y);
    return data[i];
}

template<typename T>
void Field<T>::SetCell(int x, int y, T value)
{
    int i = index(x, y);
    return data[i] = value;
}

template<typename T>
T Field<T>::GetAtPoint(raylib::Vector2 position)
{
    // TODO
}

template<typename T>
int Field<T>::index(int x, int y)
{
    assert(x > 0 && x < width);
    assert(y > 0 && y < height);
    return y * width + x;
}
