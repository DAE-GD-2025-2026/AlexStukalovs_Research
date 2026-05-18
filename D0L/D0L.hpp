#ifndef D0L_HPP
#define D0L_HPP

// Standard
#include <vector>
#include <expected>
#include <string>
#include <unordered_map>
#include <cstdint>

namespace LSystems
{
    using Rule = std::pair<char, std::string>;
    using Rules = std::unordered_map<char, std::string>;
    using Stages = std::vector<std::string>;
    using ErrorMessage = std::string;

    struct D0L final {
        std::string axiom;
        Rules rules;
        uint32_t stageCount;
    };

    auto GetD0LFromUser() noexcept -> D0L;

    auto GenerateStages(D0L const&) noexcept -> Stages;

    auto PrintStages(Stages const&) noexcept -> void;

}

#endif// D0L_HPP
