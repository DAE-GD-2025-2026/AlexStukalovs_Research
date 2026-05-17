// Project
#include "D0L.hpp"
// Standard
#include <iostream>
#include <cctype>// std::isalpha

auto LSystems::GetAxiomFromUser() noexcept -> std::string
{
    // NOTE: I won't complicate the code with validation,
    // it's purpose if demonstrating the L-systems, not being fool-proof.
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

void LSystems::PrintStages(std::string_view axiom, Rules rules, uint32_t stageCount) noexcept
{

}
