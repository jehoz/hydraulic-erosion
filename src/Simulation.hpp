#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "raylib-cpp.hpp"

struct WaterParticle
{
    raylib::Vector2 position;
    raylib::Vector2 velocity;
    float volume;

    WaterParticle(raylib::Vector2 position);
};

#endif
