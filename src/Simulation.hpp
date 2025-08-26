#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "FastNoiseLite.h"
#include "ScalarField.hpp"
#include "UnorderedArena.hpp"
#include "raylib-cpp.hpp" // IWYU pragma: keep
#include <array>
#include <random>

const int NUM_MESH_INSTANCES = 1;

struct ErosionOptions
{
    int num_particles = 50000;
    float min_volume = 0.01;
    float sediment_transfer = 0.5;
    float evaporation = 0.01;
    float sediment_ratio = 3;
    float friction = 0.05;
    float gravity = 12.0;
    float soil_evaporation = 0.00025;
    float soil_absorption = 0.5;
};

struct WaterParticle
{
    raylib::Vector2 position;
    raylib::Vector2 velocity;
    float volume;

    void Init(raylib::Vector2 position);
};

class Simulation
{
    UnorderedArena<WaterParticle> particles;
    ErosionOptions options;

    // rng
    std::mt19937 rng;
    std::uniform_real_distribution<float> w_dist;
    std::uniform_real_distribution<float> h_dist;

    // heightmap
    ScalarField terrain_height;
    raylib::Image heightmap_img;
    raylib::Texture heightmap_tex;

    // wetmap
    ScalarField terrain_wet;
    raylib::Image wetmap_img;
    raylib::Texture wetmap_tex;

    // rendering stuff
    raylib::Shader shader;
    raylib::Model model;
    std::array<raylib::Matrix, NUM_MESH_INSTANCES> instance_transforms;

    void renderTextures();

  public:
    Simulation(int meshResolution = 256);

    void Update();
    void Render();
};

#endif
