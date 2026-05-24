// Project
#include "LSystem.hpp"
// Engine
#include "Engine/Application.hpp"
#include "Engine/Utils.hpp"

int main()
{
    using namespace LSystems;

    Stages const stages{ GenerateStages(LSystem{
        .axiom = "L",
        .rules = {{ 'L', "F[-L]+L" }},
        .stageCount = 15,
    }) };
    VisualizationData constexpr visualization{
        .absRadians = Engine::Utils::ToRadians(30.f),
        .startRadians = Engine::Utils::ToRadians(90.f)
    };

    Engine::Application const engine{ "2D trees", {720, 720} };
    engine.Run(
        stages, visualization
    );

    // Further reading
    // https://gpfault.net/posts/generating-trees.txt.html
}
