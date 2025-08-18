#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "FastNoiseLite.h"
#include "ScalarField.hpp"
#include "raylib-cpp.hpp" // IWYU pragma: keep

struct InitialTerrain
{
    FastNoiseLite noise;
    int resolution = 256;
};

struct WaterParticle
{
    raylib::Vector2 position;
    raylib::Vector2 velocity;
    float volume;

    WaterParticle(raylib::Vector2 position);
};

class Simulation
{
    ScalarField terrain_height;

    raylib::Texture heightmap_tex;

    raylib::Model model;

  public:
    Simulation(int meshResolution = 256);
};

#endif
