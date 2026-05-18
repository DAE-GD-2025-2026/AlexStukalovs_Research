// Project
#include "D0L.hpp"
#include "Fractals.hpp"
// Standard
#include <print>

int main()
{
    std::println("Fractal visualizer");
    LSystems::D0L const d0l{
        LSystems::GetD0LFromUser()
    };
    LSystems::Stages const stages{
        LSystems::GenerateStages(d0l)
    };

    LSystems::RunD0LVisualizer(stages.back());

    return 0;
}
