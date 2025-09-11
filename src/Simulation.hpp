#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "FastNoiseLite.h"
#include "ScalarField.hpp"
#include "UnorderedArena.hpp"
#include "raylib-cpp.hpp" // IWYU pragma: keep
#include <array>
#include <random>

const int NUM_MESH_INSTANCES = 8;

struct ErosionOptions
{
    // Number of water particles to spawn and simulate
    int num_particles = 50000;

    // Percent volume reduction of particle at each time-step
    float volume_decay = 0.01;

    // Minimum volume of a particle before it is culled
    float min_volume = 0.01;

    // Scale factor for how much sediment is eroded/deposited at each time-step
    float terrain_erodibility = 0.5;

    // Amount of sediment that is deposted relative to how much is eroded by the particle
    float sediment_ratio = 1.0;

    // Percent speed reduction of water particle at each time-step
    float friction = 0.05;

    // Scale factor for how much terrain slope affects particle velocity
    float gravity = 12.0;

    // Percent moisture reduction of terrain at each time-step
    float soil_evaporation = 0.00025;

    // Scale factor for how much moisture a water particle adds to the ground
    float soil_absorption = 0.5;
};

struct WaterParticle
{
    raylib::Vector2 position;
    raylib::Vector2 velocity;
    float volume = 0;

    void Init(float x, float y);
};

class Simulation
{
    UnorderedArena<WaterParticle> particles;
    bool is_running_ = false;
    int meshResolution = 256;

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

  public:
    FastNoiseLite noise;
    bool normalize_heightmap = false;
    ErosionOptions options;

    int particles_remaining;
    const bool& is_running = is_running_;

    Simulation();

    void Launch();
    void Cancel();

    void Update();
    void Render();

    void initializeTerrain();
    void renderTextures();
};

#endif
