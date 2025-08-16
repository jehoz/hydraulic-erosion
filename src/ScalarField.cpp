#include "ScalarField.hpp"
#include <cassert>

struct FieldPoint
{
    // bounding coordinates of point
    int y0, y1, x0, x1;

    // distance from (x0,y0) to point
    raylib::Vector2 offset;

    // value of bounding cells on grid
    float nw, ne, sw, se;

    FieldPoint(ScalarField* field, raylib::Vector2 position)
    {
        float y_ipart;
        float y_fpart = std::modf(position.y, &y_ipart);
        float x_ipart;
        float x_fpart = std::modf(position.x, &x_ipart);

        y0 = static_cast<int>(y_ipart);
        y1 = y0 + 1 < field->height ? y0 + 1 : y0;
        x0 = static_cast<int>(x_ipart);
        x1 = x0 + 1 < field->width ? x0 + 1 : x0;

        offset = raylib::Vector2(x_fpart, y_fpart);

        nw = field->GetCell(x0, y0);
        ne = field->GetCell(x1, y0);
        sw = field->GetCell(x0, y1);
        se = field->GetCell(x1, y1);
    }
};

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
    FieldPoint point = FieldPoint(this, position);

    return 0.0f;
}

int ScalarField::index(int x, int y)
{
    assert(x > 0 && x < width);
    assert(y > 0 && y < height);
    return y * width + x;
}
