// Project
#include "Application.hpp"
#include "Utils.hpp"
// Third-party
#include "SDL3/SDL.h"
#include "SDL3/SDL_video.h"
#include "SDL3_ttf/SDL_ttf.h"
// Standard
#include <memory>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <format>
#include <ranges>
#include <stack>

namespace LSystems::Engine
{
    class Application::Impl final
    {
    public:
        explicit Impl(std::string_view const name, Vector2f const windowDims)
            : m_windowDims{ windowDims }
        {
            // Initializing SDL
            Utils::Check(SDL_InitSubSystem(SDL_INIT_VIDEO),
                "SDL_Init(SDL_INIT_VIDEO) Error"
            );

            // Initializing SDL_ttf
            Utils::Check(TTF_Init(),
                "TTF_Init() Error"
            );
            // Loading the font
            m_pFont = TTF_OpenFont("Resources/Fonts/Akt/Akt.ttf", 24);

            // Creating window and renderer
            SDL_Window* pSDLWindow{};
            SDL_Renderer* pSDLRenderer{};
            Utils::Check(
                SDL_CreateWindowAndRenderer(
                    name.data(),
                    static_cast<int>(windowDims.x), static_cast<int>(windowDims.y),
                    SDL_WINDOW_OPENGL,
                    &pSDLWindow, &pSDLRenderer
                    ),
                "Failed to initialize SDL window and renderer"
            );
            m_pSDLWindow = UniqueSDLWindow{ pSDLWindow };
            m_pSDLRenderer = UniqueSDLRenderer{ pSDLRenderer };
        }

        auto Run(Stages const&, VisualizationData const&) const noexcept -> void;

    private:
        Vector2f m_windowDims;

#pragma region SDL_Structs
        using UniqueSDLWindow = std::unique_ptr<SDL_Window,
            decltype([](SDL_Window* pSDLWindow){ SDL_DestroyWindow(pSDLWindow); })
        >;
        UniqueSDLWindow m_pSDLWindow{};

        using UniqueSDLRenderer = std::unique_ptr<SDL_Renderer,
            decltype([](SDL_Renderer* pSDLRenderer){ if (pSDLRenderer) SDL_DestroyRenderer(pSDLRenderer); })
        >;
        UniqueSDLRenderer m_pSDLRenderer{};
#pragma endregion SDL_Structs

#pragma region Text
        // Text
        TTF_Font* m_pFont{};
        SDL_Color g_primaryTextColor{ 255, 255, 255, 255 };
        SDL_Color g_secondaryTextColor{ 155, 155, 155, 255 };
        SDL_Texture* g_pPressArrowsTexture{}, * g_pStageTexture{}, * g_pPressNumberTexture{}, *g_pNameTexture{};
#pragma endregion Text

#pragma region Drawing
        auto DrawLines(std::vector<Line> const&) const -> void;
        auto DrawLine(Line const&) const noexcept -> void;
        auto DrawCircle(Vector2f center, float radius) const noexcept -> void;
#pragma endregion Drawing

        // Creates lines out of L-System stage
        [[nodiscard]] auto GenerateLines(Stage const&, VisualizationData const&) const -> std::vector<Line>;

        // Centers and scales lines so the whole L-system fills the window
        auto FitLinesToScreen(std::vector<Line>& lines) const noexcept -> void;
    };
}

LSystems::Engine::Application::Application(std::string_view const name, Vector2f const windowDims)
    : m_pImpl{ std::make_unique<Impl>(name, windowDims) } {}

LSystems::Engine::Application::~Application() = default;// External for pimpl to work

auto LSystems::Engine::Application::Run(Stages const& stages, VisualizationData const& data) const noexcept -> void
{
    m_pImpl->Run(stages, data);
}

auto LSystems::Engine::Application::Impl::Run(Stages const& stages, VisualizationData const& data) const noexcept -> void{
    uint32_t selectedStageIdx{};// Stage that user observes

    // Generating lines for all stages
    std::vector<std::vector<Line>> stageLines(stages.size());
    std::ranges::transform(stages, stageLines.begin(),
        [&](Stage const& stage) { return GenerateLines(stage, data); });

    // Running the app loop
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
                            if (selectedStageIdx > 0) --selectedStageIdx;
                            break;
                        case SDL_SCANCODE_RIGHT:
                            if (selectedStageIdx < stages.size() - 1) ++selectedStageIdx;
                            break;
                        default:;
                    }
                    break;
                default:;
            }
        }

        // Rendering

        // Clearing the background with black
        SDL_SetRenderDrawColor(m_pSDLRenderer.get(), 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(m_pSDLRenderer.get());

        // Drawing the L-System
        DrawLines(stageLines.at(selectedStageIdx));

        // Text



        // Showing the new frame
        SDL_RenderPresent(m_pSDLRenderer.get());
    }
}

auto LSystems::Engine::Application::Impl::DrawLines(std::vector<Line> const& lines) const -> void
{
    SDL_SetRenderDrawColor(m_pSDLRenderer.get(), 255, 255, 255, SDL_ALPHA_OPAQUE);// Setting white color
    for (Line const& line : lines) DrawLine(line);
}

struct State final
{
    LSystems::Vector2f point{};
    float radians{},
        lengthPx{},// from 1 to 0, where 1 is the longest it can get
        widthPx{ 1.f };
    uint32_t stateIdx{};
};

auto LSystems::Engine::Application::Impl::GenerateLines(Stage const& stage, VisualizationData const& data) const -> std::vector<Line>
{
    // Allocating point history
    std::vector<Line> lines;
    lines.reserve(std::ranges::count(stage, 'F'));

    // Allocating state stack
    std::stack<State> savedStates;
    State currentState{{}, data.startRadians, data.startLengthPx, data.widthPx};

    // Generating the points
    for (char const character : stage)
    {
        switch (character)
        {
        case 'F':
        {
            // Randomizing length around the base length for this stage
            float segmentLength{ currentState.lengthPx };
            if (data.divideLengthByStage) segmentLength /= static_cast<float>(currentState.stateIdx + 1);
            if (data.absLengthAddend > 0.f) segmentLength += Utils::GetRandFloatInRange(-data.absLengthAddend, data.absLengthAddend);

            // Drawing a line in the current direction
            Vector2f const newPointPx {
                currentState.point + segmentLength * Vector2f {
                    std::cosf(currentState.radians),
                    std::sinf(currentState.radians),
                }
            };
            lines.emplace_back(currentState.point, newPointPx, currentState.widthPx);

            // Updating the state
            currentState.point = newPointPx;
            ++currentState.stateIdx;
            currentState.lengthPx /= data.lengthDividend;

            break;
        }
        case '+':// "Turning left"
            currentState.radians += Utils::GetRandFloatInRange(
                data.absRadians - data.absRadiansAddend,
                data.absRadians + data.absRadiansAddend
            );
            break;
        case '-':// "Turning right"
            currentState.radians -= Utils::GetRandFloatInRange(
                data.absRadians - data.absRadiansAddend,
                data.absRadians + data.absRadiansAddend
            );
            break;
        case '[':// Pushing the state to the stack
            savedStates.push(currentState);
            // Dividing base width by stage idx if requested
            if (data.divideWidthByStage)
                currentState.widthPx = std::max(1.f, currentState.widthPx / static_cast<float>(savedStates.size() + 1));
            break;
        case ']':// Popping the state from the stack
            if (savedStates.empty()) break;
            currentState = savedStates.top();
            savedStates.pop();
            break;
        default:
            // throw std::logic_error{std::format("Invalid character {}", character)};// It's ok!
            break;
        }
    }

    FitLinesToScreen(lines);

    return lines;
}

auto LSystems::Engine::Application::Impl::DrawLine(Line const& line) const noexcept -> void
{
    Vector2f const d{ line.p2.x - line.p1.x, line.p2.y - line.p1.y };
    float const length{ std::hypot(d.x, d.y) };
    if (length < 1e-6f) return;// Not drawing if the line is too short

    // Perpendicular unit vector
    Vector2f const n{ -d.y / length * line.widthPx * 0.5f, d.x / length * line.widthPx * 0.5f };

    // Four corners of the rectangle
    std::array<SDL_Vertex, 4> const vertices{
        SDL_Vertex{ { line.p1.x + n.x, m_windowDims.y - (line.p1.y + n.y) }, {255,255,255,255}, {0,0} },
        { { line.p1.x - n.x, m_windowDims.y - (line.p1.y - n.y) }, {255,255,255,255}, {0,0} },
        { { line.p2.x + n.x, m_windowDims.y - (line.p2.y + n.y) }, {255,255,255,255}, {0,0} },
        { { line.p2.x - n.x, m_windowDims.y - (line.p2.y - n.y) }, {255,255,255,255}, {0,0} }
    };
    std::array constexpr indices{ 0, 1, 2, 1, 2, 3 };
    SDL_RenderGeometry(m_pSDLRenderer.get(), nullptr, vertices.data(), 4, indices.data(), 6);
}

auto LSystems::Engine::Application::Impl::DrawCircle(Vector2f const center, float const radius) const noexcept -> void
{
    // Thanks, SDL, for not having a default circle-drawing function

    float constexpr pi{ std::numbers::pi_v<float> };

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
        DrawLine({
            {center.x + radius * cosf(p1), center.y + radius * sinf(p1)},
            {center.x + radius * cosf(p2), center.y + radius * sinf(p2)}
        });
    }
}

auto GetAABB(std::vector<LSystems::Line> const& lines) -> SDL_FRect
{
    if (lines.empty()) throw std::invalid_argument("Input array must not be empty");

    float minX{ lines.front().p1.x }, maxX{ minX },
        minY{ lines.front().p1.y }, maxY{ minY };

    for (auto const& [p1, p2, _] : lines)
    {
        for (auto const& [x, y] : { p1, p2 })
        {
            minX = std::min(minX, x);  maxX = std::max(maxX, x);
            minY = std::min(minY, y);  maxY = std::max(maxY, y);
        }
    }

    return SDL_FRect{ minX, minY, maxX - minX, maxY - minY };
}

auto LSystems::Engine::Application::Impl::FitLinesToScreen(std::vector<Line>& lines) const noexcept -> void
{
    if (lines.empty()) return;
    float constexpr padding{ 20.f };// px of margin on each side

    auto const [x, y, w, h]{ GetAABB(lines) };

    // Centering the AABB at the origin first
    Vector2f const aabbCenter{ x + 0.5f * w, y + 0.5f * h },
    windowCenter{ 0.5f * m_windowDims };

    // Scaling uniformly so the larger AABB dimension fills the window minus padding
    float const availableW{ m_windowDims.x - 2.f * padding };
    float const availableH{ m_windowDims.y - 2.f * padding };
    float const scale{ w > 0.f && h > 0.f
        ? std::min(availableW / w, availableH / h)
        : 1.f };

    std::ranges::transform(lines, lines.begin(), [&](Line const& line) -> Line {
        // Translating so AABB center is at window center, then scaling around window center
        auto fit{ [&](Vector2f const& p) -> Vector2f {
            return windowCenter + scale * (p - aabbCenter);
        }};
        return Line{ fit(line.p1), fit(line.p2), line.widthPx};
    });
}
