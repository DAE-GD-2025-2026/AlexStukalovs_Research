#ifndef VECTOR2F_HPP
#define VECTOR2F_HPP

namespace LSystems
{
    struct Vector2f final
    {
        float x{}, y{};
        [[nodiscard]] Vector2f operator+=(Vector2f rhs) noexcept;
        [[nodiscard]] Vector2f operator-=(Vector2f rhs) noexcept;
    };
    [[nodiscard]] Vector2f operator-(Vector2f rhs) noexcept;
    [[nodiscard]] Vector2f operator+(Vector2f lhs, Vector2f rhs) noexcept;
    [[nodiscard]] Vector2f operator-(Vector2f lhs, Vector2f rhs) noexcept;
    [[nodiscard]] Vector2f operator*(float scalar, Vector2f rhs) noexcept;

    struct Line final
    {
        Vector2f p1{}, p2{};
    };
}

#endif// VECTOR2F_HPP
