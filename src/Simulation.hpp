#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "FastNoiseLite.h"
#include "ScalarField.hpp"
#include "raylib-cpp.hpp" // IWYU pragma: keep
#include <array>

const int NUM_MESH_INSTANCES = 32;

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
    raylib::Image heightmap_img;
    raylib::Texture heightmap_tex;
    raylib::Shader shader;

    raylib::Model model;
    std::array<raylib::Matrix, NUM_MESH_INSTANCES> instance_transforms;

    void renderTexture();

  public:
    Simulation(int meshResolution = 256);

    void Update();
    void Render();
};

#endif
