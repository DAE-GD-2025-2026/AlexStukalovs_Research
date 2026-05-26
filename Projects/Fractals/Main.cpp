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

    Stages const kochCurveAStages{ GenerateStages(LSystem{
        .axiom = "F-F-F-F",
        .rules = Rules{Rule{ 'F', "FF-F-F-F-F-F+F" }},
        .stageCount = 3
    }) };

    Stages const kochCurveBStages{ GenerateStages(LSystem{
        .axiom = "F-F-F-F",
        .rules = Rules{Rule{ 'F', "FF-F-F-F-FF" }},
        .stageCount = 3
    }) };

    Stages const kochCurveCStages{ GenerateStages(LSystem{
        .axiom = "F-F-F-F",
        .rules = Rules{Rule{ 'F', "FF-F+F-F-FF" }},
        .stageCount = 3
    }) };

    Stages const kochCurveDStages{ GenerateStages(LSystem{
        .axiom = "F-F-F-F",
        .rules = Rules{Rule{ 'F', "FF-F--F-F" }},
        .stageCount = 4
    }) };

    Stages const kochCurveEStages{ GenerateStages(LSystem{
        .axiom = "F-F-F-F",
        .rules = Rules{Rule{ 'F', "F-FF--F-F" }},
        .stageCount = 4
    }) };

    Stages const kochCurveFStages{ GenerateStages(LSystem{
        .axiom = "F-F-F-F",
        .rules = Rules{Rule{ 'F', "F-F+F-F-F" }},
        .stageCount = 4
    }) };

    Engine::Application engine{ "D0L visualizer"};
    engine.Run(
        // kochIslandStages,
        // snowflakeCurveStages,
        // kochCurveAStages,
        // kochCurveBStages,
        // kochCurveCStages,
        // kochCurveDStages,
        // kochCurveEStages,
        kochCurveFStages,
        VisualizationData{
            .absRadians = 0.5f * std::numbers::pi_v<float>,
        }
    );
}
