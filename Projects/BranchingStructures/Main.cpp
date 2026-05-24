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
        .absRadians = Engine::Utils::ToRadians(25.7f),
        .startRadians = Engine::Utils::ToRadians(90.f)
    };

    Stages const bStages{ GenerateStages(LSystem{
        .axiom = "F",
        .rules = {{'F', "F[+F]F[-F][F]"}},
        .stageCount = 5
    })};
    VisualizationData constexpr bVisualiztion{
        .absRadians = Engine::Utils::ToRadians(20.f),
        .startRadians = Engine::Utils::ToRadians(90.f)
    };

    Stages const cStages{ GenerateStages(LSystem{
        .axiom = "F",
        .rules = {{'F', "FF-[-F+F+F]+[+F-F-F]"}},
        .stageCount = 4
    })};
    VisualizationData constexpr cVisualiztion{
        .absRadians = Engine::Utils::ToRadians(22.5f),
        .startRadians = Engine::Utils::ToRadians(90.f)
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
        .absRadians = Engine::Utils::ToRadians(20.f),
        .startRadians = Engine::Utils::ToRadians(90.f)
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
        .absRadians = Engine::Utils::ToRadians(25.7f),
        .startRadians = Engine::Utils::ToRadians(90.f)
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
        .absRadians = Engine::Utils::ToRadians(22.5f),
        .startRadians = Engine::Utils::ToRadians(90.f)
    };

    Engine::Application const engine{ "Branching structures", {720, 720} };
    engine.Run(
        // aStages, aVisualiztion
        // bStages, bVisualiztion
        // cStages, cVisualiztion
        // dStages, dVisualiztion
        // eStages, eVisualiztion
        fStages, fVisualiztion
    );
}
