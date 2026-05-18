#include "D0L.hpp"
#include <print>

int main()
{
    std::println("Fractal visualizer");
    auto const axiom{ LSystems::GetAxiomFromUser() };
    auto const rules{ LSystems::GetRulesFromUser() };
    auto const stageCount{ LSystems::GetStageCountFromUser() };
    auto const stages{ LSystems::GenerateStages(axiom, rules, stageCount) };



    return 0;
}