#include "ScalarField.hpp"
#include <cassert>

ScalarField::FieldPoint::FieldPoint(ScalarField& parent,
                                    raylib::Vector2 position)
  : parent(parent)
{
    float y_ipart;
    float y_fpart = std::modf(position.y, &y_ipart);
    float x_ipart;
    float x_fpart = std::modf(position.x, &x_ipart);

    int y0 = static_cast<int>(y_ipart);
    int y1 = y0 + 1 < parent.height ? y0 + 1 : y0;
    int x0 = static_cast<int>(x_ipart);
    int x1 = x0 + 1 < parent.width ? x0 + 1 : x0;

    offset = raylib::Vector2(x_fpart, y_fpart);

    nw = &parent.data[parent.index(x0, y0)];
    ne = &parent.data[parent.index(x1, y0)];
    sw = &parent.data[parent.index(x0, y1)];
    se = &parent.data[parent.index(x1, y1)];
}

ScalarField::ScalarField(int width, int height)
  : width(width)
  , height(height)
{
    data = new float[width * height];
}

ScalarField::~ScalarField()
{
    delete[] data;
}

float ScalarField::GetCell(int x, int y)
{
    int i = index(x, y);
    return data[i];
}

void ScalarField::SetCell(int x, int y, float value)
{
    int i = index(x, y);
    data[i] = value;
}

float ScalarField::Get(raylib::Vector2 position)
{
    FieldPoint point = FieldPoint(*this, position);
    return (*point.nw) * (1 - point.offset.x) * (1 - point.offset.y) +
           (*point.ne) * point.offset.x * (1 - point.offset.y) +
           (*point.sw) * (1 - point.offset.x) * point.offset.y +
           (*point.se) * point.offset.x * point.offset.y;
}

void ScalarField::Modify(raylib::Vector2 position, float delta)
{
    FieldPoint point = FieldPoint(*this, position);
    (*point.nw) += delta * (1.0f - point.offset.x) * (1.0f - point.offset.y);
    (*point.ne) += delta * point.offset.x * (1.0f - point.offset.y);
    (*point.sw) += delta * (1.0f - point.offset.x) * point.offset.y;
    (*point.se) += delta * point.offset.x * point.offset.y;
}

int ScalarField::index(int x, int y)
{
    assert(x > 0 && x < width);
    assert(y > 0 && y < height);
    return y * width + x;
}
