#ifndef D0L
#define D0L

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

    struct LSystem {
        std::string axiom;
        Rules rules;
        uint32_t stageCount;
    };

    auto GetLSystemFromUser() noexcept -> LSystem;

    auto GenerateStages(LSystem const&) noexcept -> Stages;

    auto PrintStages(Stages const&) noexcept -> void;

}

#endif// D0L
