#include "Simulation.hpp"
#include "raylib.h"
#include <algorithm>
#include <cstdint>
#include <iostream>

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
    noise.SetFrequency(0.75);

    // intialize heightmap
    heightmap_img = raylib::Image(meshResolution, meshResolution, BLACK);

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

    renderTexture();
    heightmap_tex = LoadTextureFromImage(heightmap_img);

    // initialize mesh instances and shaders
    auto mesh =
      raylib::Mesh::Heightmap(heightmap_img, raylib::Vector3(16, 8, 16));
    model.Load(mesh);

    for (int i = 0; i < NUM_MESH_INSTANCES; i++) {
        float z_offset = static_cast<float>(i) / NUM_MESH_INSTANCES;
        instance_transforms[i] = raylib::Matrix::Translate(-8, z_offset, -8);
    }

    shader =
      LoadShader("src/shaders/heightmap.vert", "src/shaders/heightmap.frag");
    shader.locs[SHADER_LOC_MATRIX_MODEL] =
      shader.GetLocationAttrib("instanceTransform");

    model.materials[0].shader = shader;
    model.materials[0].maps[0].texture = heightmap_tex;
    // model.materials[0].maps[1].texture = heightmap_tex;
}

void Simulation::Update()
{
    // TODO
}

void Simulation::Render()
{
    renderTexture();

    DrawMeshInstanced(model.meshes[0],
                      model.materials[0],
                      instance_transforms.data(),
                      NUM_MESH_INSTANCES);
}

/*! Renders the values of the terrain_height field to the heightmap texture.
 */
void Simulation::renderTexture()
{
    for (int y = 0; y < heightmap_img.height; y++) {
        for (int x = 0; x < heightmap_img.width; x++) {
            float height = terrain_height.GetCell(x, y);
            uint32_t height_i =
              static_cast<uint32_t>(std::clamp(height, 0.0f, 1.0f) * 0xFFFFFF);

            // encode the values in all three channels of an RGB pixel so
            // that we get 2^24 discrete values instead of only 256
            raylib::Color color =
              raylib::Color(static_cast<uint8_t>(height_i >> 16),
                            static_cast<uint8_t>(height_i >> 8),
                            static_cast<uint8_t>(height_i),
                            255);
            heightmap_img.DrawPixel(x, y, color);
        }
    }

    heightmap_tex.Update(heightmap_img.data);
}
