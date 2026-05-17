#ifndef D0L
#define D0L

// Standard
#include <vector>
#include <expected>
#include <print>
#include <ranges>
#include <string>
#include <unordered_map>
#include <cstdint>

namespace LSystems
{
    using Rule = std::pair<char, std::string>;
    using Rules = std::unordered_map<char, std::string>;
    using Stages = std::vector<std::string>;
    using ErrorMessage = std::string;

    auto GetAxiomFromUser() noexcept -> std::string;

    auto RemoveSpaces(std::string_view) noexcept -> std::string;

    auto ParseRuleLine(std::string_view) noexcept -> std::expected<Rule, ErrorMessage>;

    auto GetRulesFromUser() noexcept -> Rules;

    auto GetStageCountFromUser() noexcept -> uint32_t;

    auto GenerateStages(std::string_view axiom, Rules const&, uint32_t stageCount) noexcept -> Stages;

    auto PrintStages(Stages const&) noexcept -> void;

}

#endif// D0L
