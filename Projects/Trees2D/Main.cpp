// Project
#include "LSystem.hpp"
// Engine
#include "Engine/Application.hpp"
#include "Engine/Utils.hpp"
// Standard
#include <cmath>

int main()
{
    using namespace LSystems;

    Stages const hexagonStages{ GenerateStages(LSystem{
        .axiom = "L",
        .rules = {{ 'L', "F[-L]+L" }},
        .stageCount = 15,
    }) };
    VisualizationData constexpr hexagonVisualization{
        .absRadians = Engine::Utils::ToRadians(30.f),
        // .absRadians = Engine::Utils::ToRadians(60.f),
        .startRadians = Engine::Utils::ToRadians(90.f)
    };

    Stages const snailStages{ GenerateStages(LSystem{
        .axiom = "L",
        .rules = {{ 'L', "[-FL]" }},
        .stageCount = 50,
    }) };
    VisualizationData const snailVisualization{
        .absRadians = Engine::Utils::ToRadians(90.f),
        .startRadians = 0.f,
        .lengthFactor = std::sqrtf(2.f),
    };

    Stages const hTreeStages{ GenerateStages(LSystem{
        .axiom = "L",
        .rules = {{ 'L', "[-FL][+FL]" }},
        .stageCount = 15,
    }) };
    VisualizationData const hTreeVisualization{
        .absRadians = Engine::Utils::ToRadians(90.f),
        .startRadians = Engine::Utils::ToRadians(90.f),
        .lengthFactor = std::sqrtf(2.f),
    };


    Engine::Application const engine{ "2D trees", {720, 720} };
    engine.Run(
        // snailStages,snailVisualization
        hTreeStages, hTreeVisualization
    );

    // Further reading
    // https://gpfault.net/posts/generating-trees.txt.html
}
