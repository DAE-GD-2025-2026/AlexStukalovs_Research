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

    Vector2f constexpr windowDims{ 1280, 720 };

    Stages const stages{ GenerateStages(D0L{
        .axiom = "F-F-F-F",
        .rules = Rules{Rule{ 'F', "F-F+F+FF-F-F+F" }},
        .stageCount = 2
    }) };

    Engine::Application engine{ "D0L visualizer", windowDims };
    float constexpr lineLengthPx{ 25.f };
    engine.Run(Engine::VisualizationData{
        .stage = stages.back(),
        .lineLengthPx = lineLengthPx
    });
}
