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
        // The rotation by which + or - will turn
        float startRadians{};
        float absRadians{};
        float absRadiansAddend{};

        float startLengthPx{ 1.f };
        bool divideLengthByStage{};
        float absLengthAddend{};
        float lengthDividend{ 1.f };

        float widthPx{ 1.f };
        bool divideWidthByStage{};
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
