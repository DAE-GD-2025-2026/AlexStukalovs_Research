#ifndef D0L
#define D0L

// Standard
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
    using ErrorMessage = std::string;

    auto GetAxiomFromUser() noexcept -> std::string;

    auto RemoveSpaces(std::string_view) noexcept -> std::string;

    auto ParseRuleLine(std::string_view) noexcept -> std::expected<Rule, ErrorMessage>;

    auto GetRulesFromUser() noexcept -> Rules;

    auto GetStageCountFromUser() noexcept -> uint32_t;

    void PrintStages(std::string_view axiom, Rules rules, uint32_t stageCount) noexcept;

}

#endif// D0L
