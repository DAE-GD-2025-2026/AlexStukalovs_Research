#ifndef APPLICATION_HPP
#define APPLICATION_HPP
// Project
#include "Vector2f.hpp"
// Standard
#include <cstdint>
#include <string_view>

namespace LSystems::Engine {
    struct VisualizationData final
    {
        std::string_view stage{};
        Vector2f const startingPointPx{};
        float const lineLengthPx{};
        float const lineWidthPx{5u};
    };

#pragma region Application
    // The simplest SDL wrapper serving a single-only purpose of visualizing L-systems
    class Application final {
    public:
        explicit Application(std::string_view name, Vector2f windowDims);

        // Draws the L-system and quits if Escape is pressed
        void Run(VisualizationData const&) noexcept;

    private:
        Vector2f m_windowDims;

        static void DrawLSystem(VisualizationData const&) noexcept;
    };
#pragma endregion Application

}

#endif// APPLICATION_HPP
