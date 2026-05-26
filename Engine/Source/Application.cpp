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
        explicit Impl(std::string_view const name)
        {
            // Initializing SDL
            Utils::Check(SDL_InitSubSystem(SDL_INIT_VIDEO),
                "SDL_Init(SDL_INIT_VIDEO) Error"
            );

            // Initializing SDL_ttf
            Utils::Check(TTF_Init(),
                "TTF_Init() Error"
            );

            // Creating window and renderer
            SDL_Window* pSDLWindow{};
            SDL_Renderer* pSDLRenderer{};
            Utils::Check(
                SDL_CreateWindowAndRenderer(
                    name.data(),
                    static_cast<int>(m_windowDims.x), static_cast<int>(m_windowDims.y),
                    SDL_WINDOW_OPENGL,
                    &pSDLWindow, &pSDLRenderer
                    ),
                "Failed to initialize SDL window and renderer"
            );
            m_pSDLWindow = UniqueSDLWindow{ pSDLWindow };
            m_pSDLRenderer = UniqueSDLRenderer{ pSDLRenderer };

            // Loading the font
            m_pFont_primary = UniqueFont(TTF_OpenFont("Resources/Fonts/Akt/Akt.ttf", 24));
            assert(m_pFont_primary);
            m_pFont_secondary = UniqueFont(TTF_OpenFont("Resources/Fonts/Akt/Akt.ttf", 16));
            assert(m_pFont_secondary);
            // Creating text textures
            //// Horizontal arrow text
            m_pHorizontalArrowTextTexture = CreateTextTexture( "Press horizontal arrows to switch stages", m_pFont_secondary, m_textColor_secondary);
            assert(m_pHorizontalArrowTextTexture);
            m_horizontalArrowTextDst = { 10.f, m_windowDims.y - 30.f, static_cast<float>(m_pHorizontalArrowTextTexture->w), static_cast<float>(m_pHorizontalArrowTextTexture->h) };
            //// Vertical arrow text
            m_pVerticalArrowTextTexture = CreateTextTexture( "Press vertical arrows to switch L-systems", m_pFont_secondary, m_textColor_secondary);
            assert(m_pVerticalArrowTextTexture);
            m_verticalArrowTextDst = {10.f, 45.f, static_cast<float>(m_pVerticalArrowTextTexture->w), static_cast<float>(m_pVerticalArrowTextTexture->h)};
        }
        ~Impl() noexcept
        {
            // Must take place before TTF_Quit()
            m_pFont_primary.reset();
            m_pFont_secondary.reset();

            TTF_Quit();
            SDL_Quit();
        }
        Impl(Impl const&) noexcept = delete;
        Impl(Impl &&) noexcept = delete;
        Impl& operator=(Impl const&) noexcept = delete;
        Impl& operator=(Impl &&) noexcept = delete;

        auto Run(std::vector<LSystemData> const&) noexcept -> void;

    private:
        Vector2f const m_windowDims{ 1280.f, 720.f };

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
        using UniqueFont = std::unique_ptr<TTF_Font, decltype([](TTF_Font* pFont){ TTF_CloseFont(pFont); })>;
        UniqueFont m_pFont_primary{}, m_pFont_secondary{};
        SDL_Color m_textColor_primary{ 255, 255, 255, 255 },
            m_textColor_secondary{ 155, 155, 155, 255 };
        using UniqueTexture = std::unique_ptr<SDL_Texture, decltype([](SDL_Texture* pTexture){ SDL_DestroyTexture(pTexture); })>;
        UniqueTexture m_pHorizontalArrowTextTexture{}, m_pStageTextTexture{}, m_pVerticalArrowTextTexture{}, m_pNameTextTexture{};
        SDL_FRect m_horizontalArrowTextDst{}, m_stageTextDst{}, m_verticalArrowTextDst{}, m_nameTextDst{};

        [[nodiscard]] UniqueTexture CreateTextTexture(std::string_view const text, UniqueFont const& pFont, SDL_Color const& color) const
        {
            SDL_Surface* pSurface{ TTF_RenderText_Blended(pFont.get(), text.data(), 0, color) };
            assert(pSurface);
            auto pTexture{ UniqueTexture(SDL_CreateTextureFromSurface(m_pSDLRenderer.get(), pSurface)) };
            SDL_DestroySurface(pSurface);
            return pTexture;
        }
        void DrawText(UniqueTexture const& pTextTexture, SDL_FRect const& dst) const
        {
            SDL_RenderTexture(m_pSDLRenderer.get(), pTextTexture.get(), nullptr, &dst);
        }
        void UpdateStageText(uint32_t const stageIdx, uint32_t const stageCount)
        {
            m_pStageTextTexture = CreateTextTexture( std::format("Stage {}/{}", stageIdx, stageCount), m_pFont_primary, m_textColor_primary);
            assert(m_pStageTextTexture);
        }
        void UpdateLSystemText(uint32_t const lSystemIdx, uint32_t const lSystemCount, std::string_view const lSystemName)
        {
            m_pNameTextTexture = CreateTextTexture( std::format("L-system {}/{}: {}", lSystemIdx+1, lSystemCount, lSystemName), m_pFont_primary, m_textColor_primary);
            assert(m_pNameTextTexture);
            m_nameTextDst = { 10.f, 10.f, static_cast<float>(m_pNameTextTexture->w), static_cast<float>(m_pNameTextTexture->h) };
        }
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

LSystems::Engine::Application::Application(std::string_view const name)
    : m_pImpl{ std::make_unique<Impl>(name) } {}

LSystems::Engine::Application::~Application() = default;// External for pimpl to work

auto LSystems::Engine::Application::Run(std::vector<LSystemData> const& lsystemData) noexcept -> void
{
    m_pImpl->Run(lsystemData);
}

auto LSystems::Engine::Application::Impl::Run(std::vector<LSystemData> const& lsystemData) noexcept -> void{
    // L-systems
    uint32_t currentLsystemIdx{};// L-system that user observes
    UpdateLSystemText(currentLsystemIdx, lsystemData.size(), lsystemData.at(currentLsystemIdx).name);
    Stages currentStages{ lsystemData.at(currentLsystemIdx).stages };
    // Stages
    uint32_t currentStageIdx{};// Stage that user observes
    Stage const& currentStage{ lsystemData.at(currentStageIdx).stages.at(currentStageIdx) };
    UpdateStageText(currentStageIdx+1, currentStage.size());
    m_stageTextDst = { 10.f, m_windowDims.y - 60.f, static_cast<float>(m_pStageTextTexture->w), static_cast<float>(m_pStageTextTexture->h) };
    // Generating lines for all stages
    std::vector<std::vector<Line>> stageLines(currentStages.size());
    std::ranges::transform(currentStages, stageLines.begin(),
        [&](Stage const& stage) { return GenerateLines(stage, lsystemData.at(currentLsystemIdx).visualizationData); });

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
                            if (currentStageIdx > 0)
                            {
                                --currentStageIdx;
                                UpdateStageText(currentStageIdx+1, currentStages.size());
                            }
                            break;
                        case SDL_SCANCODE_RIGHT:
                            if (currentStageIdx < currentStages.size() - 1)
                            {
                                ++currentStageIdx;
                                UpdateStageText(currentStageIdx+1, currentStages.size());
                            }
                            break;
                        case SDL_SCANCODE_UP:
                            if (currentLsystemIdx < lsystemData.size() - 1)
                            {
                                ++currentLsystemIdx;

                                // Resetting the stage idx
                                currentStageIdx = 0;
                                UpdateStageText(currentStageIdx+1, currentStages.size());

                                currentStages = lsystemData.at(currentLsystemIdx).stages;
                                // Generating lines for all stages
                                stageLines.clear();
                                stageLines.resize(currentStages.size());
                                std::ranges::transform(currentStages, stageLines.begin(),
                                    [&](Stage const& stage) { return GenerateLines(stage, lsystemData.at(currentLsystemIdx).visualizationData); });

                                UpdateLSystemText(currentLsystemIdx, lsystemData.size(), lsystemData.at(currentLsystemIdx).name);
                            }
                            break;
                        case SDL_SCANCODE_DOWN:
                            if (currentLsystemIdx > 0)
                            {
                                --currentLsystemIdx;

                                // Resetting the stage idx
                                currentStageIdx = 0;
                                UpdateStageText(currentStageIdx+1, currentStages.size());

                                currentStages = lsystemData.at(currentLsystemIdx).stages;
                                // Generating lines for all stages
                                stageLines.clear();
                                stageLines.resize(currentStages.size());
                                std::ranges::transform(currentStages, stageLines.begin(),
                                    [&](Stage const& stage) { return GenerateLines(stage, lsystemData.at(currentLsystemIdx).visualizationData); });

                                UpdateLSystemText(currentLsystemIdx, lsystemData.size(), lsystemData.at(currentLsystemIdx).name);
                            }
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
        DrawLines(stageLines.at(currentStageIdx));

        // Text
        DrawText(m_pHorizontalArrowTextTexture, m_horizontalArrowTextDst);
        DrawText(m_pStageTextTexture, m_stageTextDst);
        DrawText(m_pVerticalArrowTextTexture, m_verticalArrowTextDst);
        DrawText(m_pNameTextTexture, m_nameTextDst);

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
    float constexpr padding{ 60.f };// px of margin on each side

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
