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
        .rules = {{ 'F', "F[+F]F[-F]F" }},
        .stageCount = 5,
    }) };
    VisualizationData constexpr aVisualiztion{
        .absRadiansFrom= Engine::Utils::ToRadians(25.7f),
        .absRadians = Engine::Utils::ToRadians(90.f)
    };

    Stages const bStages{ GenerateStages(LSystem{
        .axiom = "F",
        .rules = {{'F', "F[+F]F[-F][F]"}},
        .stageCount = 5
    })};
    VisualizationData constexpr bVisualiztion{
        .absRadiansFrom= Engine::Utils::ToRadians(20.f),
        .absRadians = Engine::Utils::ToRadians(90.f)
    };

    Stages const cStages{ GenerateStages(LSystem{
        .axiom = "F",
        .rules = {{'F', "FF-[-F+F+F]+[+F-F-F]"}},
        .stageCount = 4
    })};
    VisualizationData constexpr cVisualiztion{
        .absRadiansFrom= Engine::Utils::ToRadians(22.5f),
        .absRadians = Engine::Utils::ToRadians(90.f)
    };

    Stages const dStages{ GenerateStages(LSystem{
        .axiom = "X",
        .rules = {
            {'X', "F[+X]F[-X]+X"},
            {'F', "FF"}
        },
        .stageCount = 7
    })};
    VisualizationData constexpr dVisualiztion{
        .absRadiansFrom= Engine::Utils::ToRadians(20.f),
        .absRadians = Engine::Utils::ToRadians(90.f)
    };

    Stages const eStages{ GenerateStages(LSystem{
        .axiom = "X",
        .rules = {
            {'X', "F[+X][-X]FX"},
            {'F', "FF"}
        },
        .stageCount = 7
    })};
    VisualizationData constexpr eVisualiztion{
        .absRadiansFrom= Engine::Utils::ToRadians(25.7f),
        .absRadians = Engine::Utils::ToRadians(90.f)
    };

    Stages const fStages{ GenerateStages(LSystem{
        .axiom = "X",
        .rules = {
            {'X', "F-[[X]+X]+F[+FX]-X"},
            {'F', "FF"}
        },
        .stageCount = 5
    })};
    VisualizationData constexpr fVisualiztion{
        .absRadiansFrom= Engine::Utils::ToRadians(22.5f),
        .absRadians = Engine::Utils::ToRadians(90.f)
    };

    Engine::Application const engine{ "Branching structures", {720, 720} };
    engine.Run(
        // aStages, aVisualiztion
        // bStages, bVisualiztion
        // cStages, cVisualiztion
        // dStages, dVisualiztion
        eStages, eVisualiztion
        // fStages, fVisualiztion
    );
    // TODO:
    // 1. Define L-Systems in a file
    // 2. Add ability to switch between L-Systems at runtime
    // 3. Add text explaining the controls
}
