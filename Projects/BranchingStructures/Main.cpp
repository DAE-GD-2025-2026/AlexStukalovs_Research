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
        .startRadians = Engine::Utils::ToRadians(90.f),
        .absRadians = Engine::Utils::ToRadians(25.7f),
    };

    Stages const bStages{ GenerateStages(LSystem{
        .axiom = "F",
        .rules = {{'F', "F[+F]F[-F][F]"}},
        .stageCount = 5
    })};
    VisualizationData constexpr bVisualiztion{
        .startRadians = Engine::Utils::ToRadians(90.f),
        .absRadians = Engine::Utils::ToRadians(20.f),
    };

    Stages const cStages{ GenerateStages(LSystem{
        .axiom = "F",
        .rules = {{'F', "FF-[-F+F+F]+[+F-F-F]"}},
        .stageCount = 4
    })};
    VisualizationData constexpr cVisualiztion{
        .startRadians = Engine::Utils::ToRadians(90.f),
        .absRadians = Engine::Utils::ToRadians(22.5f),
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
        .startRadians = Engine::Utils::ToRadians(90.f),
        .absRadians = Engine::Utils::ToRadians(20.f),
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
        .startRadians = Engine::Utils::ToRadians(90.f),
        .absRadians = Engine::Utils::ToRadians(25.7f),
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
        .startRadians = Engine::Utils::ToRadians(90.f),
        .absRadians = Engine::Utils::ToRadians(22.5f),
    };

    std::vector<LSystemData> const lSystemData {
        {aStages, "A", aVisualiztion},
        {bStages, "B", bVisualiztion},
        {cStages, "C", cVisualiztion},
        {dStages, "D", dVisualiztion},
        {eStages, "E", eVisualiztion},
        {fStages, "F", fVisualiztion},
    };

    Engine::Application engine{ "Branching structures" };
    engine.Run(lSystemData);
    // TODO: Define L-Systems in a file
}
