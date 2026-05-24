// Project
#include "D0L.hpp"
// Standard
#include <iostream>
#include <cctype>// std::isalpha
#include <ranges>
#include <print>

namespace LSystems {
    auto GetAxiomFromUser() noexcept -> std::string;

    auto RemoveSpaces(std::string_view) noexcept -> std::string;

    auto ParseRuleLine(std::string_view) noexcept -> std::expected<Rule, ErrorMessage>;

    auto GetRulesFromUser() noexcept -> Rules;

    auto GetStageCountFromUser() noexcept -> uint32_t;
}

auto LSystems::GetD0LFromUser() noexcept -> D0L {
    return D0L{ GetAxiomFromUser(), GetRulesFromUser(), GetStageCountFromUser() };
}

auto LSystems::GetAxiomFromUser() noexcept -> std::string
{
    std::string axiom;
    auto const isAxiomInvalid{
        [&]
        {
            if (axiom.empty())
            {
                std::println("Error: Axiom is empty");
                return true;
            }
            if (axiom.contains(' '))
            {
                std::println("Error: Spaces found in the axiom");
                return true;
            }

            return false;
        }
    };

    do
    {
        std::print("Enter the axiom: ");
        std::getline(std::cin, axiom);
    }
    while (isAxiomInvalid());

    return axiom;
}

auto LSystems::RemoveSpaces(std::string_view line) noexcept -> std::string
{
    return line
        | std::views::filter([](char const c) { return !std::isspace(c); })
        | std::ranges::to<std::string>();
}

auto LSystems::ParseRuleLine(std::string_view const lineView) noexcept -> std::expected<Rule, ErrorMessage>
{
    // Removing all the spaces from the line
    auto const line{RemoveSpaces(lineView)};

    if (line.empty())
        return std::unexpected("Input string is empty");

    // Getting the predecessor(single character, because we do not process the context)
    char const predecessor{line[0]};
    if (!std::isalpha(predecessor))
        return std::unexpected(std::format("Predecessor {} is not a letter", predecessor));
    // Removing the predecessor from the line
    std::string_view rest{line}; // The rest of the string after removing characters
    rest.remove_prefix(1);

    // Checking if there is an arrow
    if (!rest.starts_with("->"))
        return std::unexpected("Expected arrow \"->\"");
    // Removing the arrow from the line
    rest.remove_prefix(2);

    // Checking if the successor is there
    if (rest.empty())
        return std::unexpected("No successor provided");

    // All the rest should be a successor (note that spaces are already removed)
    return std::make_pair(predecessor, std::string{rest});
}

auto LSystems::GetRulesFromUser() noexcept -> std::unordered_map<char, std::string>
{
    std::println("Enter production rules (format: X -> Y, one per line, empty line to finish):");
    std::println("Enter rules (example: a -> ab):");
    std::unordered_map<char, std::string> rules;
    std::string line;
    while (true) // Finishes upon empty line
    {
        std::getline(std::cin, line);

        if (line.empty())
        {
            if (rules.empty())
            {
                std::println("Error: Need to have at least one rule");
                continue; // Asking again
            }
            break; // Done entering rules
        }

        // Parsing the rule
        if (auto const rule{ParseRuleLine(line)}; rule.has_value())
        {
            if (auto [_, inserted] = rules.insert_or_assign(rule->first, rule->second); !inserted)
            {
                std::println("Warning: Overrwriting rule for {}", rule->first);
            }
        }
        else
        {
            std::println("Error: {}", rule.error());
        }
    }

    return rules;
}

auto LSystems::GetStageCountFromUser() noexcept -> uint32_t
{
    uint32_t stageCount{};
    std::print("Enter stage count: ");
    std::cin >> stageCount;
    return stageCount;
}

auto LSystems::GenerateStages(D0L const& lsystem) noexcept -> Stages
{
    std::vector<std::string> stages;
    // NOTE: +1 for the axiom at the beginning
    stages.reserve(lsystem.stageCount + 1);
    stages.emplace_back(lsystem.axiom);

    std::string_view currentStage{ lsystem.axiom };
    std::string newStage;

    for ([[maybe_unused]] auto _ : std::views::iota(0u, lsystem.stageCount))
    {
        for (char character : currentStage)
        {
            // No rule -> adding the character
            if (!lsystem.rules.contains(character))
            {
                newStage.push_back(character);
                continue;
            }

            // Has rule -> adding the rule
            newStage.append_range(lsystem.rules.at(character));
        }

        // Saving the stage
        stages.push_back(std::move(newStage));
        // Setting the new stage as current one
        currentStage = stages.back();
        // Preparing to reuse the string for the new stage
        newStage.clear();
    }

    return stages;
}

auto LSystems::PrintStages(Stages const& stages) noexcept -> void
{
    for (auto&& [stageIdx, stage] : std::views::enumerate(stages))
    {
        std::println("Stage {}: {}", stageIdx + 1, stage);
    }
}
