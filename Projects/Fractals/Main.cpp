// Project
#include "LSystem.hpp"
// Engine
#include "Engine/Application.hpp"
// Standard
#include <print>
#include <numbers>

int main()
{
    using namespace LSystems;

    // std::println("Fractal visualizer");
    // D0L const d0l{
    //     GetD0LFromUser()
    // };
    //
    // Stages const stages{
    //     GenerateStages(d0l)
    // };

    Stages const kochIslandStages{ GenerateStages(LSystem{
        .axiom = "F-F-F-F",
        .rules = Rules{Rule{ 'F', "F-F+F+FF-F-F+F" }},
        .stageCount = 5
    }) };

    Stages const snowflakeCurveStages{ GenerateStages(LSystem{
        .axiom = "F",
        .rules = Rules{Rule{ 'F', "F+F-F-F+F" }},
        .stageCount = 4
    }) };

    Engine::Application const engine{ "D0L visualizer", {1280, 720} };
    engine.Run(
        kochIslandStages,
        VisualizationData{
            .absRadiansFrom = 0.5f * std::numbers::pi_v<float>,
        }
    );
}
