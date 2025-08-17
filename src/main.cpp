#include "FastNoiseLite.h"
#include "raylib-cpp.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int main()
{
    int screenWidth = 800;
    int screenHeight = 600;
    raylib::Window w(screenWidth, screenHeight, "terrain");

    SetTargetFPS(60);

    raylib::Color backgroundColor(0xE2EFFFFF);

    raylib::Camera3D camera(raylib::Vector3(20.0, 30.0, 20.0),
                            raylib::Vector3(0.0f, 0.0f, 0.0f),
                            raylib::Vector3(0.0f, 1.0f, 0.0f),
                            45.0f,
                            CAMERA_PERSPECTIVE);

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
    noise.SetFractalType(FastNoiseLite::FractalType_Ridged);
    noise.SetFractalGain(0.5f);
    noise.SetFractalOctaves(6);
    noise.SetFrequency(0.0075 / 2.0);

    raylib::Image heightmap(256, 256, BLACK);
    for (int y = 0; y < 256; y++) {
        for (int x = 0; x < 256; x++) {
            float height =
              noise.GetNoise(static_cast<float>(x), static_cast<float>(y));
            // remap from [-1, 1] to [0, 1]
            height = (height + 1.0) / 2.0;
            heightmap.DrawPixel(
              x, y, raylib::Color(raylib::Vector3(0, 0, height)));
        }
    }
    raylib::Texture2D heightmapTex = LoadTextureFromImage(heightmap);

    raylib::Mesh mesh = GenMeshHeightmap(heightmap, raylib::Vector3(16, 8, 16));
    raylib::Model terrainModel = LoadModelFromMesh(mesh);
    terrainModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = heightmapTex;

    raylib::Matrix modelTransform = MatrixTranslate(-8, 0, -8);

    while (!w.ShouldClose()) {
        camera.Update(CAMERA_ORBITAL);

        BeginDrawing();
        {
            ClearBackground(backgroundColor);

            camera.BeginMode();
            {
                DrawMesh(terrainModel.meshes[0],
                         terrainModel.materials[0],
                         modelTransform);

                DrawGrid(20, 1.0f);
            }
            camera.EndMode();

            DrawFPS(10, 10);
        }
        EndDrawing();
    }

    UnloadTexture(heightmapTex);
    UnloadModel(terrainModel);
    CloseWindow();

    return 0;
}
