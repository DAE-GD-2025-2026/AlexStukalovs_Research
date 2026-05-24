#ifndef APPLICATION_HPP
#define APPLICATION_HPP
// Project
#include "LSystem.hpp"
#include "Engine/Vector2f.hpp"
// Standard
#include <string_view>

namespace LSystems
{
    struct VisualizationData final
    {
        float const startingLineLengthPx;
        // Multiplier/divider for the line length when incrementing/decrementing stage idx
        float const absLengthScaleFactor;
        // The rotation by which + or - will turn
        float absRadians, startRadians{};
    };
}

namespace LSystems::Engine {

#pragma region Application
    // The simplest SDL wrapper serving a single-only purpose of visualizing L-systems
    class Application final {
    public:
        explicit Application(std::string_view name, Vector2f windowDims);

        // Draws the L-system and quits if Escape is pressed
        auto Run(Stages, VisualizationData const&) const noexcept -> void;

    private:
        Vector2f m_windowDims;

        auto DrawStage(std::string_view stage, VisualizationData const&) const -> void;

        auto DrawLine(Vector2f p1, Vector2f p2) const noexcept -> void;
        auto DrawCircle(Vector2f center, float radius) const noexcept -> void;
        auto DrawLinesFromPoints(std::vector<Vector2f> const&) const noexcept -> void;

        // Creates points to draw lines inbetween
        [[nodiscard]] auto GeneratePoints(std::string_view stage, VisualizationData const&) const -> std::vector<Vector2f>;

        // Puts the points in a way that the shape they make is centered
        auto CenterPoints(std::vector<Vector2f>& points) const noexcept -> void;
    };


#pragma endregion Application

}

#endif// APPLICATION_HPP
