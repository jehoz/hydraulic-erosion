#include "Simulation.hpp"

WaterParticle::WaterParticle(raylib::Vector2 pos)
{
    position = pos;
    velocity = raylib::Vector2(0.0f, 0.0f);
    volume = 1.0f;
}
