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
        // The rotation by which + or - will turn
        float absRadians{};
        float startRadians{};
        float lengthFactor{ 1.f }; // multiplied on every [
        float startWidthPx{ 1.f };
        float widthFactor{ 1.f };  // multiplied on every [
    };
}

namespace LSystems::Engine {

#pragma region Application
    // The simplest SDL wrapper serving a single-only purpose of visualizing L-systems
    class Application final {
    public:
        explicit Application(std::string_view name, Vector2f windowDims);

        // Draws the L-system and quits if Escape is pressed
        auto Run(Stages const&, VisualizationData const&) const noexcept -> void;

    private:
        Vector2f m_windowDims;

        auto DrawLines(std::vector<Line> const&) const -> void;

        auto DrawLine(Line const&) const noexcept -> void;
        auto DrawCircle(Vector2f center, float radius) const noexcept -> void;

        // Creates lines out of L-System stage
        [[nodiscard]] auto GenerateLines(Stage const&, VisualizationData const&) const -> std::vector<Line>;

        // Centers and scales lines so the whole L-system fills the window
        auto FitLinesToScreen(std::vector<Line>& lines) const noexcept -> void;
    };


#pragma endregion Application

}

#endif// APPLICATION_HPP
