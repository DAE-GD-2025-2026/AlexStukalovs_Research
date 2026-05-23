#include "Vector2f.hpp"

LSystems::Vector2f LSystems::Vector2f::operator+=(Vector2f const rhs) noexcept
{
    this->x += rhs.x;
    this->y += rhs.y;
    return *this;
}

LSystems::Vector2f LSystems::Vector2f::operator-=(Vector2f const rhs) noexcept
{
    return *this += -rhs;
}

LSystems::Vector2f LSystems::operator-(Vector2f const rhs) noexcept
{
    return {-rhs.x, -rhs.y};
}

LSystems::Vector2f LSystems::operator+(Vector2f lhs, Vector2f const rhs) noexcept
{
    return lhs += rhs;
}

LSystems::Vector2f LSystems::operator-(Vector2f lhs, Vector2f rhs) noexcept
{
    return lhs -= rhs;
}

LSystems::Vector2f LSystems::operator*(float const scalar, Vector2f const rhs) noexcept
{
    return { rhs.x * scalar, rhs.y * scalar };
}
