// Project
#include "Application.hpp"
#include "Utils.hpp"
// Third-party
#include "SDL3/SDL.h"
// Standard
#include <cmath>

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

void DrawLine(LSystems::Vector2f const p1, LSystems::Vector2f const p2) noexcept
{
    SDL_RenderLine(g_pSDLRenderer,
        p1.x,
        p1.y,
        p2.x,
        p2.y
    );
}

void LSystems::Engine::Application::DrawLSystem(VisualizationData const& data) noexcept {
    // Setting white color
    SDL_SetRenderDrawColor(g_pSDLRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    float radians{};
    Vector2f prevPointPx{ data.startingPointPx };
    for (char const c : data.stage)
    {
        switch (c)
        {
        case 'F':
        {
            // Drawing a line in the current direction
            Vector2f const newPointPx{
                prevPointPx + Vector2f{
                    std::cosf(radians) * data.lineLengthPx,
                    std::sinf(radians) * data.lineLengthPx,
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
        default: ;
        }
    }

}
#pragma endregion Application
