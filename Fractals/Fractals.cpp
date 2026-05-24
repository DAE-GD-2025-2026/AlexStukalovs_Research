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

    Stages const stages{ GenerateStages(D0L{
        .axiom = "F-F-F-F",
        .rules = Rules{Rule{ 'F', "F-F+F+FF-F-F+F" }},
        .stageCount = 2
    }) };

    Engine::Application const engine{ "D0L visualizer", {1280, 720} };
    engine.Run(Engine::VisualizationData{
        .stages = stages,
        .lineLengthPx = 25.f
    });

    // I have multiple stages, I want to depict them all
    // I want to switch between stages using left and right keys

}
