#include "Simulation.hpp"
#include "raylib.h"
#include <algorithm>
#include <cstdint>

void WaterParticle::Init(float x, float y)
{
    position.x = x;
    position.y = y;
    velocity = raylib::Vector2(0.0f, 0.0f);
    volume = 1.0f;
}

Simulation::Simulation(int meshResolution)
  : particles(UnorderedArena<WaterParticle>(256))
  , terrain_height(ScalarField(meshResolution, meshResolution))
  , terrain_wet(ScalarField(meshResolution, meshResolution))
{
    rng = std::mt19937(12345);
    w_dist = std::uniform_real_distribution<float>(0, terrain_height.width);
    h_dist = std::uniform_real_distribution<float>(0, terrain_height.height);

    heightmap_img = raylib::Image(meshResolution, meshResolution, BLACK);
    heightmap_tex = LoadTextureFromImage(heightmap_img);

    wetmap_img = raylib::Image(meshResolution, meshResolution, BLACK);
    wetmap_tex = LoadTextureFromImage(wetmap_img);

    // initialize heightmap with noise
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
    noise.SetFractalType(FastNoiseLite::FractalType_Ridged);
    noise.SetFractalGain(0.5f);
    noise.SetFractalOctaves(6);
    noise.SetFrequency(0.75);

    for (int y = 0; y < meshResolution; y++) {
        for (int x = 0; x < meshResolution; x++) {
            float height =
              noise.GetNoise(static_cast<float>(x) / meshResolution, static_cast<float>(y) / meshResolution);

            // remap from [-1, 1] to [0, 1]
            height = (height + 1.0) / 2.0;

            terrain_height.SetCell(x, y, height);
        }
    }
    renderTextures();

    // NOTE: if I generate the mesh with Mesh::Plane, it has some weird extra
    // geometry (not sure if this is a bug or I'm just stupid).  So instead I
    // use Heightmap and just overwrite the shader with my own.
    auto mesh = raylib::Mesh::Heightmap(heightmap_img, raylib::Vector3(16, 0, 16));
    model.Load(mesh);

    for (int i = 0; i < NUM_MESH_INSTANCES; i++) {
        float z_offset = static_cast<float>(i) / NUM_MESH_INSTANCES;
        instance_transforms[i] = raylib::Matrix::Translate(-8, z_offset, -8);
    }

    shader = LoadShader("src/shaders/heightmap.vert", "src/shaders/heightmap.frag");
    shader.locs[SHADER_LOC_MATRIX_MODEL] = shader.GetLocationAttrib("instanceTransform");

    model.materials[0].shader = shader;
    model.materials[0].maps[0].texture = heightmap_tex;
    model.materials[0].maps[1].texture = wetmap_tex;

    particles_remaining = options.num_particles;
    while (particles.size < particles.capacity && particles_remaining > 0) {
        particles.Add(WaterParticle());
        particles_remaining--;
    }
    TraceLog(LOG_INFO, "Initialized %d water particles.", particles.size);
}

void Simulation::Update()
{
    for (int i = 0; i < 100; i++)
        for (auto& p : particles) {
            // reinitialize (or delete) dead particles
            if (p.volume < options.min_volume) {
                if (particles_remaining > 0) {
                    float x = w_dist(rng);
                    float y = h_dist(rng);
                    p.Init(x, y);
                    particles_remaining--;
                } else {
                    continue;
                }
            }

            // movement
            auto init_pos = p.position;
            auto grav_force = terrain_height.Gradient(p.position).Scale(options.gravity);

            p.velocity = p.velocity.Scale(1.0f - options.friction).Subtract(grav_force);
            p.position = p.position.Add(p.velocity.Normalize());
            if (p.position.x < 0 || p.position.x >= terrain_height.width || p.position.y < 0 ||
                p.position.y >= terrain_height.height) {
                // if particle goes out of bounds, just set its volume to zero so it gets reinitialized next iter
                p.volume = 0;
                continue;
            }

            // sediment transfer
            float delta_elev = terrain_height.Get(p.position) - terrain_height.Get(init_pos);
            float delta_sed = delta_elev * options.sediment_transfer;
            if (delta_sed > 0) {
                delta_sed *= options.sediment_ratio;
            }
            terrain_height.Modify(init_pos, delta_sed);

            // evaporate
            p.volume *= 1.0f - options.evaporation;
        }
}

void Simulation::Render()
{
    renderTextures();

    DrawMeshInstanced(model.meshes[0], model.materials[0], instance_transforms.data(), NUM_MESH_INSTANCES);
}

/*! Renders the values of the terrain_height field to the heightmap texture.
 */
void Simulation::renderTextures()
{
    for (int y = 0; y < heightmap_img.height; y++) {
        for (int x = 0; x < heightmap_img.width; x++) {
            /*  HEIGHTMAP  */
            float height = terrain_height.GetCell(x, y);
            uint32_t height_i = static_cast<uint32_t>(std::clamp(height, 0.0f, 1.0f) * 0xFFFFFF);

            // encode the values in all three channels of an RGB pixel so
            // that we get 2^24 discrete values instead of only 256
            raylib::Color height_color = raylib::Color(static_cast<uint8_t>(height_i >> 16),
                                                       static_cast<uint8_t>(height_i >> 8),
                                                       static_cast<uint8_t>(height_i),
                                                       255);
            heightmap_img.DrawPixel(x, y, height_color);

            /*  WETMAP  */
            float wet = terrain_wet.GetCell(x, y);
            uint32_t wet_i = static_cast<uint32_t>(std::clamp(wet, 0.0f, 1.0f) * 0xFFFFFF);

            raylib::Color wet_color = raylib::Color(
              static_cast<uint8_t>(wet_i >> 16), static_cast<uint8_t>(wet_i >> 8), static_cast<uint8_t>(wet_i), 255);
            wetmap_img.DrawPixel(x, y, wet_color);
        }
    }

    heightmap_tex.Update(heightmap_img.data);
    wetmap_tex.Update(wetmap_img.data);
}
