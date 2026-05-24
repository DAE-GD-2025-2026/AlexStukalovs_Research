// Project
#include "Fractals.hpp"
#include "D0L.hpp"
// Engine
#include "Engine/Application.hpp"
// Standard
#include <print>

void LSystems::RunD0LVisualizer() {
    std::println("Fractal visualizer");

    // LSystems::D0L const d0l{
    //     LSystems::GetD0LFromUser()
    // };

    // LSystems::Stages const stages{
    //     LSystems::GenerateStages(d0l)
    // };

    std::string const axiom{ };
    Vector2f constexpr windowDims{ 1280, 720 };

    Engine::Application engine{ "D0L visualizer", windowDims };
    float constexpr lineLengthPx{ 400.f };
    engine.Run(Engine::VisualizationData{
        .stages = Stages{"F-F-F-F"},
        .startingPointPx = 0.5f * windowDims + 0.5f * Vector2f{-lineLengthPx, lineLengthPx},
        .startingLineLengthPx = lineLengthPx
    });
}
