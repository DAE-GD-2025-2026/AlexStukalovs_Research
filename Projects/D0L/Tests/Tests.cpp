// Project
#include "LSystem.hpp"
// Standard
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

using namespace LSystems;
using Catch::Matchers::ContainsSubstring;

TEST_CASE("RemoveSpaces removes all whitespace", "[parser]")
{
    SECTION("No spaces in string")
    {
        auto result = RemoveSpaces("abc");
        REQUIRE(result == "abc");
    }

    SECTION("Spaces throughout string")
    {
        auto result = RemoveSpaces(" a b c ");
        REQUIRE(result == "abc");
    }

    SECTION("Tabs and newlines")
    {
        auto result = RemoveSpaces("a\tb\nc");
        REQUIRE(result == "abc");
    }

    SECTION("Empty string")
    {
        auto result = RemoveSpaces("");
        REQUIRE(result.empty());
    }
}

TEST_CASE("ParseRuleLine with valid input", "[parser]")
{
    SECTION("Simple rule")
    {
        auto result = ParseRuleLine("a->b");
        REQUIRE(result.has_value());
        REQUIRE(result->first == 'a');
        REQUIRE(result->second == "b");
    }

    SECTION("Rule with spaces")
    {
        auto result = ParseRuleLine(" a -> bc ");
        REQUIRE(result.has_value());
        REQUIRE(result->first == 'a');
        REQUIRE(result->second == "bc");
    }

    SECTION("Rule with multi-character successor")
    {
        auto result = ParseRuleLine("x->xyz");
        REQUIRE(result.has_value());
        REQUIRE(result->first == 'x');
        REQUIRE(result->second == "xyz");
    }

    SECTION("Multiple rules with different predecessors")
    {
        auto r1 = ParseRuleLine("a->ab");
        auto r2 = ParseRuleLine("b->a");

        REQUIRE(r1.has_value());
        REQUIRE(r2.has_value());
        REQUIRE(r1->first == 'a');
        REQUIRE(r1->second == "ab");
        REQUIRE(r2->first == 'b');
        REQUIRE(r2->second == "a");
    }
}

TEST_CASE("ParseRuleLine with invalid input", "[parser]")
{
    SECTION("Empty string")
    {
        auto result = ParseRuleLine("");
        REQUIRE_FALSE(result.has_value());
        REQUIRE_THAT(result.error(), ContainsSubstring("empty"));
    }

    SECTION("Missing arrow")
    {
        auto result = ParseRuleLine("ab");
        REQUIRE_FALSE(result.has_value());
        REQUIRE_THAT(result.error(), ContainsSubstring("arrow"));
    }

    SECTION("Non-letter predecessor")
    {
        auto result = ParseRuleLine("1->ab");
        REQUIRE_FALSE(result.has_value());
        REQUIRE_THAT(result.error(), ContainsSubstring("not a letter"));
    }

    SECTION("Dash as predecessor")
    {
        auto result = ParseRuleLine("-->ab");
        REQUIRE_FALSE(result.has_value());
        REQUIRE_THAT(result.error(), ContainsSubstring("not a letter"));
    }

    SECTION("Missing successor")
    {
        auto result = ParseRuleLine("a->");
        REQUIRE_FALSE(result.has_value());
        REQUIRE_THAT(result.error(), ContainsSubstring("No successor"));
    }

    SECTION("Only arrow, no successor")
    {
        auto result = ParseRuleLine("a-> ");
        REQUIRE_FALSE(result.has_value());
        REQUIRE_THAT(result.error(), ContainsSubstring("No successor"));
    }

    SECTION("Multiple arrows")
    {
        auto result = ParseRuleLine("a->b->c");
        REQUIRE(result.has_value());
        // Should parse as: predecessor='a', successor="b->c"
        REQUIRE(result->second == "b->c");
    }
}

TEST_CASE("ParseRuleLine edge cases", "[parser]")
{
    SECTION("Single character rule")
    {
        auto result = ParseRuleLine("a->x");
        REQUIRE(result.has_value());
        REQUIRE(result->second == "x");
    }

    SECTION("Rule with only spaces")
    {
        auto result = ParseRuleLine("   ");
        REQUIRE_FALSE(result.has_value());
    }

    SECTION("Very long successor")
    {
        auto result = ParseRuleLine("a->abcdefghijklmnopqrstuvwxyz");
        REQUIRE(result.has_value());
        REQUIRE(result->second == "abcdefghijklmnopqrstuvwxyz");
    }
}

// Test for Fibonacci L-system rules
TEST_CASE("Fibonacci L-system rules parsing", "[integration]")
{
    SECTION("Parse both Fibonacci rules")
    {
        auto rule_a = ParseRuleLine("a->ab");
        auto rule_b = ParseRuleLine("b->a");

        REQUIRE(rule_a.has_value());
        REQUIRE(rule_b.has_value());

        // Store in map like the program does
        std::unordered_map<char, std::string> rules;
        rules.insert_or_assign(rule_a->first, rule_a->second);
        rules.insert_or_assign(rule_b->first, rule_b->second);

        REQUIRE(rules.size() == 2);
        REQUIRE(rules['a'] == "ab");
        REQUIRE(rules['b'] == "a");
    }
}
