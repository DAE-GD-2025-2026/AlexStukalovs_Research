// Project
#include "Application.hpp"
#include "Utils.hpp"
// Third-party
#include "SDL3/SDL.h"
// Standard
#include <algorithm>
#include <cassert>
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

auto LSystems::Engine::Application::Run(VisualizationData const& data) const noexcept -> void{
    uint32_t stageIdx{};
    float lineLengthPx{ data.startingLineLengthPx };

    while (true) {

        // Processing events

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT: return;
                case SDL_EVENT_KEY_UP:
                    switch (event.key.scancode)
                    {
                        case SDL_SCANCODE_LEFT:
                        {
                            if (stageIdx == 0) break;

                            if ((--stageIdx %= data.stages.size()) == 0)
                                lineLengthPx = data.startingLineLengthPx;
                            else
                                lineLengthPx *= data.absLengthScaleFactor;
                            break;
                        }
                        case SDL_SCANCODE_RIGHT:
                        {
                            if (stageIdx == data.stages.size() - 1) break;

                            if ((++stageIdx %= data.stages.size()) == 0)
                                lineLengthPx = data.startingLineLengthPx;
                            else
                                lineLengthPx /= data.absLengthScaleFactor;
                            break;
                        }
                        default:;
                    }

                    break;
                default:;
            }
        }

        // Rendering

        // Clearing the background with black
        SDL_SetRenderDrawColor(g_pSDLRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(g_pSDLRenderer);

        // Drawing the L-System
        DrawStage(data.stages.at(stageIdx), lineLengthPx);

        // Showing the new frame
        SDL_RenderPresent(g_pSDLRenderer);
    }
}

auto LSystems::Engine::Application::DrawStage(std::string_view const stage, float const lineLengthPx) const -> void
{
    SDL_SetRenderDrawColor(g_pSDLRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);// Setting white color
    DrawLinesFromPoints(GeneratePoints(stage, lineLengthPx));
}

auto LSystems::Engine::Application::GeneratePoints(std::string_view const stage, float const lineLengthPx) const -> std::vector<Vector2f>
{
    // Generating the points
    float radians{};
    std::vector<Vector2f> points;
    points.reserve(std::ranges::count(stage, 'F'));
    points.emplace_back(Vector2f{});// Starting point = {0, 0}
    for (char const character : stage)
    {
        switch (character)
        {
        case 'F':
        {
            // Drawing a line in the current direction
            Vector2f const newPointPx {
                points.back() + Vector2f {
                    std::cosf(radians) * lineLengthPx,
                    std::sinf(radians) * lineLengthPx,
                }
            };
            points.emplace_back(newPointPx);
            break;
        }
        case '+':
            // Rotating 90 degrees, i.e., "turning left"
            radians += 0.5f * std::numbers::pi;
            break;
        case '-':
            // Rotating -90 degrees, i.e., "turning right"
            radians -= 0.5f * std::numbers::pi;
            break;
        default:
            throw std::logic_error{std::format("Invalid character {}", character)};
        }

    }

    CenterPoints(points);
    return points;
}

// NOTE: The origin is bottom left
auto LSystems::Engine::Application::DrawLine(Vector2f const p1, Vector2f const p2) const noexcept -> void
{
    SDL_RenderLine(g_pSDLRenderer,
        p1.x,
        m_windowDims.y - p1.y,
        p2.x,
        m_windowDims.y - p2.y
    );
}

auto LSystems::Engine::Application::DrawCircle(Vector2f const center, float const radius) const noexcept -> void
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

auto LSystems::Engine::Application::DrawLinesFromPoints(std::vector<Vector2f> const& points) const noexcept -> void
{
    for (uint32_t const pointIdx : std::ranges::views::iota(1u, points.size()))
        DrawLine(points[pointIdx - 1], points[pointIdx]);
}

auto GetAABB(std::vector<LSystems::Vector2f> const& points) -> SDL_FRect
{
    if (points.empty()) throw std::invalid_argument("Points is an empty array");

    auto const xExtremes{ std::ranges::minmax(points, {}, &LSystems::Vector2f::x) },
        yExtremes{ std::ranges::minmax(points, {}, &LSystems::Vector2f::y) };

    float const left{ xExtremes.min.x },
        top{ yExtremes.min.y },
        right{ xExtremes.max.x },
        bottom{ yExtremes.max.y };

    return SDL_FRect{ left, top, right - left, bottom - top };
}

auto LSystems::Engine::Application::CenterPoints(std::vector<Vector2f>& points) const noexcept -> void
{
    const auto [x, y, w, h]{ GetAABB(points) };
    Vector2f const aabbCenter{ x + 0.5f * w, y + 0.5f * h },
        offsetVector{ 0.5f * m_windowDims - aabbCenter };

    std::ranges::transform(points, points.begin(), [&offsetVector](Vector2f const& p)
    {
        return p + offsetVector;
    });
}

#pragma endregion Application
