#include "Simulation.hpp"
#include "imgui/imgui.h"
#include "imgui/rlImGui.h"
#include "raylib-cpp.hpp" // IWYU pragma: keep

int main()
{
    int screenWidth = 800;
    int screenHeight = 600;
    raylib::Window w(screenWidth, screenHeight, "terrain");

    rlImGuiSetup(true);

    SetTargetFPS(60);

    /* raylib::Color backgroundColor(0xE2EFFFFF); */
    raylib::Color backgroundColor(0x222222FF);

    raylib::Camera3D camera(raylib::Vector3(20.0, 30.0, 20.0),
                            raylib::Vector3(0.0f, 0.0f, 0.0f),
                            raylib::Vector3(0.0f, 1.0f, 0.0f),
                            45.0f,
                            CAMERA_PERSPECTIVE);

    Simulation simulation(256);

    auto window_flags =
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

    while (!w.ShouldClose()) {
        camera.Update(CAMERA_ORBITAL);
        simulation.Update();

        BeginDrawing();
        rlImGuiBegin();
        {
            ClearBackground(backgroundColor);

            camera.BeginMode();
            {
                simulation.Render();
                DrawGrid(20, 1.0f);
            }
            camera.EndMode();

            DrawFPS(10, 10);
            DrawText(TextFormat("remaining particles: %d", simulation.particles_remaining), 10, 50, 20, WHITE);

            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(250, screenHeight));
            ImGui::Begin("side panel", nullptr, window_flags);
            ImGui::Text("Simulation Settings");
            ImGui::SliderInt("Number of Particles", &simulation.options.num_particles, 0, 100000000);

            ImGui::End();
        }
        rlImGuiEnd();
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();

    return 0;
}
