#ifndef APPLICATION_HPP
#define APPLICATION_HPP
// Project
#include "LSystem.hpp"
#include "Engine/GeometricPrimitives.hpp"
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

        auto DrawLine(Line const&) const noexcept -> void;
        auto DrawCircle(Vector2f center, float radius) const noexcept -> void;

        // Creates lines out of L-System stage
        [[nodiscard]] auto GenerateLines(std::string_view stage, VisualizationData const&) const -> std::vector<Line>;

        // Puts the line points in a way that the shape they make is centered
        auto CenterLines(std::vector<Line>& lines) const noexcept -> void;
    };


#pragma endregion Application

}

#endif// APPLICATION_HPP
