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

    // Stages const hexagonStages{ GenerateStages(LSystem{
    //     .axiom = "L",
    //     .rules = {{ 'L', "F[-L]+L" }},
    //     .stageCount = 15,
    // }) };
    // VisualizationData constexpr hexagonVisualization{
    //     .absRadiansFrom= Engine::Utils::ToRadians(30.f),
    //     // .absRadiansFrom= Engine::Utils::ToRadians(60.f),
    //     .startRadians = Engine::Utils::ToRadians(90.f)
    // };
    //
    // Stages const snailStages{ GenerateStages(LSystem{
    //     .axiom = "L",
    //     .rules = {{ 'L', "[-FL]" }},
    //     .stageCount = 50,
    // }) };
    // VisualizationData const snailVisualization{
    //     .absRadiansFrom= Engine::Utils::ToRadians(90.f),
    //     .startRadians = 0.f,
    //     .lengthFactor = std::sqrtf(2.f),
    // };
    //
    // Stages const hTreeStages{ GenerateStages(LSystem{
    //     .axiom = "L",
    //     .rules = {{ 'L', "[-FL][+FL]" }},
    //     .stageCount = 15,
    // }) };
    // VisualizationData const hTreeVisualization{
    //     .absRadiansFrom= Engine::Utils::ToRadians(90.f),
    //     .startRadians = Engine::Utils::ToRadians(90.f),
    //     .lengthFactor = std::sqrtf(2.f),
    // };

    Stages const tree2DStages{ GenerateStages(LSystem{
        .axiom = "FL",
        // .rules = {{ "L(g)", R"(
        //                     F(Rand(100/(g+1)-10, 100/(g+1)+10), 5/(g+1))
        //                     [R(Rand(-20-5, -20+5))L(g+1)]
        //                     [R(Rand(20-5, 20+5))L(g+1)]
        //                     )" }},
        .rules = {{'L', "[-FL][+FL]"}},
        .stageCount = 15,
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

    Engine::Application const engine{ "2D trees", {720, 720} };
    engine.Run(
        // snailStages,snailVisualization
        // hTreeStages, hTreeVisualization
        tree2DStages, tree2DVisualization
    );

    // Further reading
    // https://gpfault.net/posts/generating-trees.txt.html
}
