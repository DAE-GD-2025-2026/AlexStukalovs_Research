#ifndef UTILS_HPP
#define UTILS_HPP
// Third-party
#include "SDL3/SDL.h"
// Standard
#include <format>
#include <stdexcept>
#include <string_view>
#include <random>
#include <numbers>

namespace LSystems::Engine::Utils
{
    inline auto ThrowSDLError(std::string_view const message) -> void
    {
        throw std::runtime_error(std::format("{}: {}", message, SDL_GetError()));
    }

    inline auto Check(bool const result, std::string_view const message) -> void
    {
        if (!result) ThrowSDLError(message);
    }

    constexpr auto ToRadians(float const degrees) noexcept -> float {
        return degrees * std::numbers::pi_v<float> / 180.f;
    }

    inline float GetRandFloatInRange(float const min, float const max) {
        static std::mt19937 rng{std::random_device{}()};
        std::uniform_real_distribution dist(min, max);
        return dist(rng);
    }

}

#endif// UTILS_HPP
