#include "raylib-cpp.hpp"

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

    raylib::Image heightmap = LoadImage("noise.png");
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
