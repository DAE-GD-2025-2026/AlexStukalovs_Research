// Project
#include "Application.hpp"
#include "Utils.hpp"
// Third-party
#include "SDL3/SDL.h"

static SDL_Window* pSDLWindow{};
static SDL_Renderer* pSDLRenderer{};

LSystems::Engine::Application::Application(std::string_view const name, Vector2u const windowDims)
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
            &pSDLWindow, &pSDLRenderer
            ),
        "Failed to initialize SDL window and renderer"
    );
}

void LSystems::Engine::Application::Run(std::string_view const lSystem) const noexcept {
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
        SDL_SetRenderDrawColor(pSDLRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(pSDLRenderer);

        // Drawing the L-System
        DrawLSystem(lSystem);

        // Showing the new frame
        SDL_RenderPresent(pSDLRenderer);
    }
}

void LSystems::Engine::Application::DrawLSystem([[maybe_unused]] std::string_view lSystem) const noexcept {
    // Setting white color
    SDL_SetRenderDrawColor(pSDLRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    SDL_RenderLine(pSDLRenderer, 0.f, 0.f, m_windowDims.x, m_windowDims.y);
}
