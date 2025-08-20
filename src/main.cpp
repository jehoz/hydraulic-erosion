#include "FastNoiseLite.h"
#include "Simulation.hpp"
#include "raylib-cpp.hpp" // IWYU pragma: keep

int main()
{
    int screenWidth = 800;
    int screenHeight = 600;
    raylib::Window w(screenWidth, screenHeight, "terrain");

    SetTargetFPS(60);

    // raylib::Color backgroundColor(0xE2EFFFFF);
    raylib::Color backgroundColor(0x222222FF);

    raylib::Camera3D camera(raylib::Vector3(20.0, 30.0, 20.0),
                            raylib::Vector3(0.0f, 0.0f, 0.0f),
                            raylib::Vector3(0.0f, 1.0f, 0.0f),
                            45.0f,
                            CAMERA_PERSPECTIVE);

    Simulation simulation(256);

    while (!w.ShouldClose()) {
        camera.Update(CAMERA_ORBITAL);

        BeginDrawing();
        {
            ClearBackground(backgroundColor);

            camera.BeginMode();
            {
                simulation.Render();
                DrawGrid(20, 1.0f);
            }
            camera.EndMode();

            DrawFPS(10, 10);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
