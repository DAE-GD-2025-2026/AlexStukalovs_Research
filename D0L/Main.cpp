// Project
#include "D0L.hpp"

int main()
{
    std::println("D0L generator");
    auto const axiom{ LSystems::GetAxiomFromUser() };
    auto const rules{ LSystems::GetRulesFromUser() };
    auto const stageCount{ LSystems::GetStageCountFromUser() };
    LSystems::GenerateStages(axiom, rules, stageCount);

    return 0;
}