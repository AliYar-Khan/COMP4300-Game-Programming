#pragma once

#include <SFML/Graphics.hpp>
#include <string>

struct ShapeData
{
    enum class Type
    {
        Circle,
        Rectangle
    };

    Type type;

    std::string name;

    sf::Vector2f position;
    sf::Vector2f velocity;

    sf::Color color;

    float size1 = 0.0f;
    float size2 = 0.0f;

    bool draw = true;
};