#ifndef FIELD_HPP
#define FIELD_HPP

#include "raylib-cpp.hpp"

/*! An abstraction for treating a 2D grid of values as a continuous field.
 *
 * Can be sampled at any point and the result will be a linear combination of
 * the four nearest cells in the underlying grid.
 */
class ScalarField
{
    float* data;

    int index(int x, int y);

  public:
    int width, height;

    ScalarField(int width, int height);
    ~ScalarField();

    float GetCell(int x, int y);
    void SetCell(int x, int y, float value);

    float Get(raylib::Vector2 position);
    raylib::Vector2 Gradient(raylib::Vector2 position);
    void Modify(raylib::Vector2 position, float delta);
};

#endif
