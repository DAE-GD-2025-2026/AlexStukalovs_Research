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
        Stages stages;
        float const lineLengthPx;
        float const lineWidthPx{5u};
    };



#pragma region Application
    // The simplest SDL wrapper serving a single-only purpose of visualizing L-systems
    class Application final {
    public:
        explicit Application(std::string_view name, Vector2f windowDims);

        // Draws the L-system and quits if Escape is pressed
        auto Run(VisualizationData const&) const noexcept -> void;

    private:
        Vector2f m_windowDims;

        auto DrawLSystem(VisualizationData const&, uint32_t stageIdx) const -> void;

        auto DrawLine(Vector2f p1, Vector2f p2) const noexcept -> void;
        auto DrawCircle(Vector2f center, float radius) const noexcept -> void;
        auto DrawLinesFromPoints(std::vector<Vector2f> const&) const noexcept -> void;

        // Creates points to draw lines inbetween
        auto GeneratePoints(VisualizationData const&, uint32_t stageIdx) const -> std::vector<Vector2f>;

        // Puts the points in a way that the shape they make is centered
        auto CenterPoints(std::vector<Vector2f>& points) const noexcept -> void;
    };


#pragma endregion Application

}

#endif// APPLICATION_HPP
