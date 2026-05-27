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
        .stageCount = 10,
    }) };
    VisualizationData constexpr pseudoTreeVisualization{
        .startRadians = Engine::Utils::ToRadians(90.f),
        .absRadians = Engine::Utils::ToRadians(30.f),// tree
    };
    VisualizationData constexpr hexagonVisualization{
        .startRadians = Engine::Utils::ToRadians(90.f),
        .absRadians = Engine::Utils::ToRadians(60.f),// hexagon
    };

    Stages const snailStages{ GenerateStages(LSystem{
        .axiom = "L",
        .rules = {{ 'L', "[-FL]" }},
        .stageCount = 50,
    }) };
    VisualizationData const snailVisualization{
        .startRadians = 0.f,
        .absRadians = Engine::Utils::ToRadians(90.f),
        .lengthDividend = std::sqrtf(2.f),
    };

    Stages const hTreeStages{ GenerateStages(LSystem{
        .axiom = "L",
        .rules = {{ 'L', "[-FL][+FL]" }},
        .stageCount = 10,
    }) };
    VisualizationData const hTreeVisualization{
        .startRadians = Engine::Utils::ToRadians(90.f),
        .absRadians= Engine::Utils::ToRadians(90.f),
        .lengthDividend = std::sqrtf(2.f),
    };

    Stages const tree2DStages{ GenerateStages(LSystem{
        .axiom = "FL",
        .rules = {{'L', "[-FL][+FL]"}},
        .stageCount = 8,
    }) };
    VisualizationData constexpr tree2DVisualization{
        .startRadians = Engine::Utils::ToRadians(90.f),
        .absRadians = Engine::Utils::ToRadians(20.f),
        .absRadiansAddend = Engine::Utils::ToRadians(5.f),

        .startLengthPx = 100.f,
        .divideLengthByStage = true,
        .absLengthAddend = 10,

        .widthPx = 5.f,
        .divideWidthByStage = true,
    };

    std::vector<LSystemData> const lSystemData {
        {hexagonStages, "Pseudo-tree", pseudoTreeVisualization},
        {hexagonStages, "Hexagon", hexagonVisualization},
        {snailStages, "Snail", snailVisualization},
        {hTreeStages, "H tree", hTreeVisualization},
        {tree2DStages, "Tree 2D", tree2DVisualization},
    };

    Engine::Application engine{"Tree 2D"};
    engine.Run(lSystemData);

    // Further reading
    // https://gpfault.net/posts/generating-trees.txt.html
}
