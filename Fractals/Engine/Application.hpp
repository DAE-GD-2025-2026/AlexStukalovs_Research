#ifndef APPLICATION_HPP
#define APPLICATION_HPP
// Standard
#include <string_view>

namespace LSystems::Engine {
    struct Vector2u{ uint32_t x, y; };

    // The simplest SDL wrapper serving a single-only purpose of visualizing L-systems
    class Application final {
    public:
        explicit Application(std::string_view name, Vector2u windowDims);

        // Draws the L-system and quits if Escape is pressed
        void Run(std::string_view lSystem) const noexcept;

    private:
        Vector2u m_windowDims;

        void DrawLSystem(std::string_view lSystem) const noexcept;

    };

}

#endif// APPLICATION_HPP
