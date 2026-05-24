#ifndef APPLICATION_HPP
#define APPLICATION_HPP
// Project
#include "D0L.hpp"
#include "Vector2f.hpp"
// Standard
#include <string_view>

namespace LSystems::Engine {
    struct VisualizationData final
    {
        Stages stages{};
        Vector2f const startingPointPx{};
        float const startingLineLengthPx{};
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

        void DrawLSystem(VisualizationData const&);

        void DrawLine(Vector2f p1, Vector2f p2) noexcept;
        void DrawCircle(Vector2f center, float radius) noexcept;
    };


#pragma endregion Application

}

#endif// APPLICATION_HPP
