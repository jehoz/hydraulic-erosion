#include "Simulation.hpp"

WaterParticle::WaterParticle(raylib::Vector2 position)
  : position(position)
{
    velocity = raylib::Vector2(0.0f, 0.0f);
    volume = 1.0f;
}

Simulation::Simulation(int meshResolution)
  : terrain_height(ScalarField(meshResolution, meshResolution))
{
    // setup default heightmap generator
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
    noise.SetFractalType(FastNoiseLite::FractalType_Ridged);
    noise.SetFractalGain(0.5f);
    noise.SetFractalOctaves(6);
    noise.SetFrequency(0.0075 / 2.0);

    terrain_height = ScalarField(meshResolution, meshResolution);

    raylib::Image heightmap(meshResolution, meshResolution, BLACK);
    for (int y = 0; y < meshResolution; y++) {
        for (int x = 0; x < meshResolution; x++) {
            float height =
              noise.GetNoise(static_cast<float>(x) / meshResolution,
                             static_cast<float>(y) / meshResolution);
            // remap from [-1, 1] to [0, 1]
            height = (height + 1.0) / 2.0;

            terrain_height.SetCell(x, y, height);
        }
    }
    heightmap_tex = LoadTextureFromImage(heightmap);
}
