// Project
#include "D0L.hpp"
// Standard
#include <print>

int main()
{
    std::println("D0L generator");
    auto const lsystem{
        LSystems::GetD0LFromUser()
    };
    auto const stages{ LSystems::GenerateStages(lsystem) };
    LSystems::PrintStages(stages);

    return 0;
}