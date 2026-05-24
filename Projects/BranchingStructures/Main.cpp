// Project
#include "LSystem.hpp"
// Engine
#include "Engine/Application.hpp"
#include "Engine/Utils.hpp"

int main()
{
    using namespace LSystems;

    Stages const aStages{ GenerateStages(LSystem{
        .axiom = "F",
        .rules = Rules{Rule{ 'F', "F[+F]F[-F]F" }},
        .stageCount = 5,
    }) };
    VisualizationData constexpr aVisualiztion{
        .startingLineLengthPx = 50.f,
        .absLengthScaleFactor = 6.f,
        .absRadians = Engine::Utils::ToRadians(25.7f),
        .startRadians = Engine::Utils::ToRadians(90.f)
    };

    Engine::Application const engine{ "D0L visualizer", {1280, 720} };
    engine.Run(
        aStages,
        aVisualiztion
    );
}
