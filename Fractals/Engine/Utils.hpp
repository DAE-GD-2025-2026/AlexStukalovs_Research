#ifndef UTILS_HPP
#define UTILS_HPP
// Third-party
#include "SDL3/SDL.h"
// Standard
#include <format>
#include <stdexcept>
#include <string_view>

namespace LSystems::Engine::Utils
{
    inline void ThrowSDLError(std::string_view const message)
    {
        throw std::runtime_error(std::format("{}: {}", message, SDL_GetError()));
    }

    inline void Check(bool const result, std::string_view const message)
    {
        if (!result) ThrowSDLError(message);
    }

}

#endif// UTILS_HPP
