#include "Simulation.hpp"
#include "imgui/imgui.h"
#include "imgui/rlImGui.h"
#include "raylib-cpp.hpp" // IWYU pragma: keep

int main()
{
    int screenWidth = 1280;
    int screenHeight = 720;
    raylib::Window w(screenWidth, screenHeight, "terrain");

    rlImGuiSetup(true);

    SetTargetFPS(60);

    /* raylib::Color backgroundColor(0xE2EFFFFF); */
    raylib::Color backgroundColor(0x0F0F0FFF);

    raylib::Camera3D camera(raylib::Vector3(20.0, 30.0, 20.0),
                            raylib::Vector3(0.0f, 0.0f, 0.0f),
                            raylib::Vector3(0.0f, 1.0f, 0.0f),
                            45.0f,
                            CAMERA_PERSPECTIVE);

    Simulation simulation;

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

            DrawFPS(screenWidth - 80, screenHeight - 20);

            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(350, screenHeight));
            ImGui::Begin("side panel", nullptr, window_flags);
            ImGui::Text("Simulation Settings");

            ImGui::BeginDisabled(simulation.is_running);
            {
                ImGui::SliderInt(
                  "Particles", &simulation.options.num_particles, 0, 100000000, "%d", ImGuiSliderFlags_Logarithmic);
                ImGui::SliderFloat(
                  "Min Volume", &simulation.options.min_volume, 0, 1.0, "%f", ImGuiSliderFlags_Logarithmic);
                ImGui::SliderFloat("Sediment Transfer", &simulation.options.sediment_transfer, 0, 1.0);
                ImGui::SliderFloat("Evaporation", &simulation.options.evaporation, 0, 1.0);
                ImGui::SliderFloat("Sediment Ratio", &simulation.options.sediment_ratio, 0, 10.0);
                ImGui::SliderFloat("Friction", &simulation.options.friction, 0, 1.0);
                ImGui::SliderFloat("Gravity", &simulation.options.gravity, 0, 100.0);
                ImGui::SliderFloat("Soil Evaporation", &simulation.options.soil_evaporation, 0, 1.0);
                ImGui::SliderFloat("Soil Absorption", &simulation.options.soil_absorption, 0, 1.0);
            }
            ImGui::EndDisabled();

            if (simulation.is_running) {
                ImGui::Text("%d / %d particles",
                            simulation.options.num_particles - simulation.particles_remaining,
                            simulation.options.num_particles);
                if (ImGui::Button("Cancel Simulation")) {
                    simulation.Cancel();
                }
            } else {
                if (ImGui::Button("Launch Simulation")) {
                    simulation.Launch();
                }
            }

            ImGui::End();
        }
        rlImGuiEnd();
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();

    return 0;
}
