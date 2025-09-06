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

    auto windowFlags =
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

    ///// FNL ImGui stuff
    int fnlNoiseType = 0;
    int fnlSeed = 1337;
    float fnlFrequency = 0.75f;

    int fnlFractalType = 0;
    int fnlFractalOctaves = 4;
    float fnlFractalLacunarity = 2.0f;
    float fnlFractalGain = 0.5f;
    float fnlFractalWeightedStrength = 0.0f;
    float fnlFractalPingPongStrength = 2.0f;

    int fnlCellularType = 1;
    int fnlCellularReturnType = 1;
    float fnlCellularJitter = 1.0f;

    static const char* enumNoiseType[] = {
        "OpenSimplex2", "OpenSimplex2S", "Cellular", "Perlin", "Value Cubic", "Value"
    };
    static const char* enumFractalType[] = { "None", "FBm", "Ridged", "Ping Pong" };
    static const char* enumCellularType[] = { "Euclidian", "Euclidian Sq", "Manhattan", "Hybrid" };
    static const char* enumCellularReturnType[] = { "Cell Value",     "Distance",       "Distance 2",
                                                    "Distance 2 Add", "Distance 2 Sub", "Distance 2 Mul",
                                                    "Distance 2 Div" };

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
            ImGui::Begin("side panel", nullptr, windowFlags);

            ImGui::SeparatorText("Heightmap");
            bool fnlConfigChanged = false;

            if (ImGui::Combo("Noise Type", &fnlNoiseType, enumNoiseType, IM_ARRAYSIZE(enumNoiseType))) {
                simulation.noise.SetNoiseType((FastNoiseLite::NoiseType)fnlNoiseType);
                fnlConfigChanged = true;
            }
            if (ImGui::DragInt("Seed", &fnlSeed)) {
                simulation.noise.SetSeed(fnlSeed);
                fnlConfigChanged = true;
            }
            if (ImGui::DragFloat("Frequency", &fnlFrequency, 0.0002f)) {
                simulation.noise.SetFrequency(fnlFrequency);
                fnlConfigChanged = true;
            }
            if (ImGui::Checkbox("Normalize", &simulation.normalize_heightmap)) {
                fnlConfigChanged = true;
            }

            ImGui::TextUnformatted("Fractal");
            if (ImGui::Combo("Fractal Type", &fnlFractalType, enumFractalType, IM_ARRAYSIZE(enumFractalType))) {
                simulation.noise.SetFractalType((FastNoiseLite::FractalType)fnlFractalType);
                fnlConfigChanged = true;
            }
            ImGui::BeginDisabled(fnlFractalType == 0);
            if (ImGui::DragInt("Octaves", &fnlFractalOctaves, 0.1f, 1, 20)) {
                simulation.noise.SetFractalOctaves(fnlFractalOctaves);
                fnlConfigChanged = true;
            }
            if (ImGui::DragFloat("Lacunarity", &fnlFractalLacunarity, 0.01f)) {
                simulation.noise.SetFractalLacunarity(fnlFractalLacunarity);
                fnlConfigChanged = true;
            }
            if (ImGui::DragFloat("Gain", &fnlFractalGain, 0.01f)) {
                simulation.noise.SetFractalGain(fnlFractalGain);
                fnlConfigChanged = true;
            }
            if (ImGui::DragFloat("Weighted Strength", &fnlFractalWeightedStrength, 0.01f)) {
                simulation.noise.SetFractalWeightedStrength(fnlFractalWeightedStrength);
                fnlConfigChanged = true;
            }
            ImGui::BeginDisabled(fnlFractalType != (int)FastNoiseLite::FractalType_PingPong);
            if (ImGui::DragFloat("Ping Pong Strength", &fnlFractalPingPongStrength, 0.01f)) {
                simulation.noise.SetFractalPingPongStrength(fnlFractalPingPongStrength);
                fnlConfigChanged = true;
            }
            ImGui::EndDisabled();
            ImGui::EndDisabled();

            ImGui::TextUnformatted("Cellular");
            ImGui::BeginDisabled(fnlNoiseType != (int)FastNoiseLite::NoiseType_Cellular);
            if (ImGui::Combo("Distance Function", &fnlCellularType, enumCellularType, IM_ARRAYSIZE(enumCellularType))) {
                simulation.noise.SetCellularDistanceFunction((FastNoiseLite::CellularDistanceFunction)fnlCellularType);
                fnlConfigChanged = true;
            }
            if (ImGui::Combo("Return Type",
                             &fnlCellularReturnType,
                             enumCellularReturnType,
                             IM_ARRAYSIZE(enumCellularReturnType))) {
                simulation.noise.SetCellularReturnType((FastNoiseLite::CellularReturnType)fnlCellularReturnType);
                fnlConfigChanged = true;
            }
            if (ImGui::DragFloat("Jitter", &fnlCellularJitter, 0.01f)) {
                simulation.noise.SetCellularJitter(fnlCellularJitter);
                fnlConfigChanged = true;
            }
            ImGui::EndDisabled();

            if (fnlConfigChanged) {
                simulation.initializeTerrain();
            }

            ImGui::SeparatorText("Erosion");

            ImGui::BeginDisabled(simulation.is_running);
            {
                ImGui::DragInt("Particles", &simulation.options.num_particles, 100.0f, 0);
                ImGui::DragFloat("Min Volume", &simulation.options.min_volume, 0.0001f, 0.0001f, 1.0f);
                ImGui::DragFloat("Sediment Transfer", &simulation.options.sediment_transfer, 0.001f, 0.0f, 1.0f);
                ImGui::DragFloat("Evaporation", &simulation.options.evaporation, 0.001f, 0.0f, 1.0f);
                ImGui::DragFloat("Sediment Ratio", &simulation.options.sediment_ratio, 0.01f, 0.0f);
                ImGui::DragFloat("Friction", &simulation.options.friction, 0, 1.0);
                ImGui::DragFloat("Gravity", &simulation.options.gravity, 0.001f, 0.0f);
                ImGui::DragFloat("Soil Evaporation", &simulation.options.soil_evaporation, 0.001f, 0.0f, 1.0f);
                ImGui::DragFloat("Soil Absorption", &simulation.options.soil_absorption, 0.001f, 0.0f, 1.0f);
            }
            ImGui::EndDisabled();

            ImGui::Separator();

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
