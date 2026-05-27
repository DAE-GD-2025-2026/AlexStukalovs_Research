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

    VisualizationData constexpr visualizationData {
        .absRadians = 0.5f * std::numbers::pi_v<float>,
    };

    std::vector<LSystemData> const lSystemData {
        {kochIslandStages, "Koch island", visualizationData },
        {snowflakeCurveStages, "Snowflake curve", visualizationData},
        {kochCurveAStages, "Koch curve A", visualizationData },
        {kochCurveBStages, "Koch curve B", visualizationData },
        {kochCurveCStages, "Koch curve C", visualizationData },
        {kochCurveDStages, "Koch curve D", visualizationData },
        {kochCurveEStages, "Koch curve E", visualizationData },
        {kochCurveFStages, "Koch curve F", visualizationData },
    };

    Engine::Application engine{ "Fractal visualizer"};
    engine.Run(lSystemData);
}
