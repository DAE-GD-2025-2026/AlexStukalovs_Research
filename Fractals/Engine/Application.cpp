// Project
#include "Application.hpp"
#include "Utils.hpp"
// Third-party
#include "SDL3/SDL.h"
// Standard
#include <cmath>
#include <format>
#include <ranges>

static SDL_Window* g_pSDLWindow{};
static SDL_Renderer* g_pSDLRenderer{};

#pragma region Application
LSystems::Engine::Application::Application(std::string_view const name, Vector2f const windowDims)
    : m_windowDims{ windowDims }
{
    // Initializing SDL
    Utils::Check(SDL_InitSubSystem(SDL_INIT_VIDEO),
        "SDL_Init(SDL_INIT_VIDEO) Error"
    );

    // Creating window and renderer
    Utils::Check(
        SDL_CreateWindowAndRenderer(
            name.data(),
            static_cast<int>(windowDims.x), static_cast<int>(windowDims.y),
            SDL_WINDOW_OPENGL,
            &g_pSDLWindow, &g_pSDLRenderer
            ),
        "Failed to initialize SDL window and renderer"
    );
}

void LSystems::Engine::Application::Run(VisualizationData const& data) noexcept {
    while (true) {

        // Processing events

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT: return;
                default:;
            }
        }

        // Rendering

        // Clearing the background with black
        SDL_SetRenderDrawColor(g_pSDLRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(g_pSDLRenderer);

        // Drawing the L-System
        DrawLSystem(data);

        // Showing the new frame
        SDL_RenderPresent(g_pSDLRenderer);
    }
}

void LSystems::Engine::Application::DrawLSystem(VisualizationData const& data) {
    // Setting white color
    SDL_SetRenderDrawColor(g_pSDLRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    float radians{}, lineLengthPx{ data.startingLineLengthPx };
    Vector2f prevPointPx{ data.startingPointPx };
    DrawCircle(prevPointPx, 10.f);
    for (std::string_view const stage : data.stages)
    {
        // Making the line length shorter
        // How?
        // I need to divide the current line length by the amount of lines that
        // are collinear with the previous line. How?
        // I cannot just count all the F's

        // Drawing the stage
        for (char const character : stage)
        {
            switch (character)
            {
            case 'F':
                {
                    // Drawing a line in the current direction
                    Vector2f const newPointPx{
                        prevPointPx + Vector2f{
                            std::cosf(radians) * data.startingLineLengthPx,
                            std::sinf(radians) * data.startingLineLengthPx,
                        }
                    };
                    DrawLine(
                        prevPointPx, newPointPx
                    );
                    prevPointPx = newPointPx;
                    break;
                }
            case '+':
                // Rotating 90 degrees
                radians += 0.5f * std::numbers::pi;
                break;
            case '-':
                radians -= 0.5f * std::numbers::pi;
                break;
            default:
                throw std::logic_error{std::format("Invalid character {}", character)};
            }
        }
    }

}

// NOTE: The origin is bottom left
void LSystems::Engine::Application::DrawLine(Vector2f const p1, Vector2f const p2) noexcept
{
    SDL_RenderLine(g_pSDLRenderer,
        p1.x,
        m_windowDims.y - p1.y,
        p2.x,
        m_windowDims.y - p2.y
    );
}

void LSystems::Engine::Application::DrawCircle(Vector2f const center, float const radius) noexcept
{
    // Thanks, SDL, for not having a default circle-drawing function

    float constexpr pi{ std::numbers::pi_v<_Float16> };

    int32_t const pointCount{
        std::max(
            static_cast<int32_t>(2.f * pi * radius),// Circumference in px(point per px)
            8// Making small circles not degenerate to 1 or 2 segments
        )
    };

    for (int32_t const pointIdx : std::ranges::views::iota(0, pointCount)) {
        // Calculating 2 subsequent points on the circumference
        float const p1{ 2.f * pi * static_cast<float>(pointIdx) / static_cast<float>(pointCount) },
            p2{ 2.f * pi * static_cast<float>(pointIdx + 1) / static_cast<float>(pointCount) };

        // Drawing the circumference segment
        DrawLine(
            {center.x + radius * cosf(p1), center.y + radius * sinf(p1)},
            {center.x + radius * cosf(p2), center.y + radius * sinf(p2)}
        );
    }
}

#pragma endregion Application
