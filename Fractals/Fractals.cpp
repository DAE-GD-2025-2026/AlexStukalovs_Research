// Project
#include "Engine/Application.hpp"
#include "Fractals.hpp"
#include "D0L.hpp"

void LSystems::RunD0LVisualizer(std::string_view const stage) noexcept {

    LSystems::Engine::Vector2u constexpr windowDims{ 720, 480 };
    LSystems::Engine::Application engine{ "Fractal visualizer", windowDims };
    engine.Run(stage);
}
