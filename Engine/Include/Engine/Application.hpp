#ifndef APPLICATION_HPP
#define APPLICATION_HPP
// Project
#include "LSystem.hpp"
#include "Engine/GeometricPrimitives.hpp"
// Standard
#include <memory>
#include <string_view>

namespace LSystems
{
    struct VisualizationData final
    {
        float startRadians{};// Rotation for the first line
        float absRadians{};// The rotation by which + or - will turn
        float absRadiansAddend{};// Value of Rand(-absRadiansAdded, absRadiansAddend) gets added to absRadians

        float startLengthPx{ 1.f };// Gets scaled to the maximal size in FitLinesToScreen()
        bool divideLengthByStageIdx{};// Whether the length should be divided by stage index(useful for continuous segment shortening)
        float absLengthAddend{};// Same as for radians
        float lengthDividend{ 1.f };// Value by which to divide length every frame

        float widthPx{ 1.f };// Values < 1.f will be clamped to 1.f
        bool divideWidthByStageIdx{};// Same as for length
    };
    struct LSystemData final
    {
        Stages const& stages;
        std::string name;
        VisualizationData const& visualizationData;
    };
}

namespace LSystems::Engine {

    // The simplest SDL wrapper serving a single-only purpose of visualizing L-systems
    class Application final {
    public:
        explicit Application(std::string_view name);
        ~Application();
        Application(Application const&) noexcept = delete;
        Application(Application&&) noexcept = delete;
        Application& operator=(Application const&) noexcept = delete;
        Application& operator=(Application&&) noexcept = delete;

        // Draws the L-system and quits if Escape is pressed
        auto Run(std::vector<LSystemData> const&) const noexcept -> void;

    private:
        class Impl;
        std::unique_ptr<Impl> m_pImpl;
    };

}

#endif// APPLICATION_HPP
