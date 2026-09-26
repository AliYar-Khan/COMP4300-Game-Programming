#include "Config.hpp"

#include <cstdint>
#include <fstream>
#include <iostream>

bool loadConfig(
    const std::string &filename,
    unsigned int &windowWidth,
    unsigned int &windowHeight,
    std::vector<ShapeData> &shapes)
{
    std::ifstream file(filename);

    if (!file)
    {
        std::cerr << "Failed to open config file: "
                  << filename << '\n';

        return false;
    }

    std::string type;

    while (file >> type)
    {
        if (type == "Window")
        {
            file >> windowWidth >> windowHeight;
        }
        else if (type == "Circle")
        {
            ShapeData shape;

            shape.type = ShapeData::Type::Circle;

            int r;
            int g;
            int b;

            file >> shape.name
                 >> shape.position.x
                 >> shape.position.y
                 >> shape.velocity.x
                 >> shape.velocity.y
                 >> r
                 >> g
                 >> b
                 >> shape.size1;

            shape.color = sf::Color(
                static_cast<std::uint8_t>(r),
                static_cast<std::uint8_t>(g),
                static_cast<std::uint8_t>(b));

            shape.size2 = 0.0f;

            shapes.push_back(shape);
        }
        else if (type == "Rectangle")
        {
            ShapeData shape;

            shape.type = ShapeData::Type::Rectangle;

            int r;
            int g;
            int b;

            file >> shape.name
                 >> shape.position.x
                 >> shape.position.y
                 >> shape.velocity.x
                 >> shape.velocity.y
                 >> r
                 >> g
                 >> b
                 >> shape.size1
                 >> shape.size2;

            shape.color = sf::Color(
                static_cast<std::uint8_t>(r),
                static_cast<std::uint8_t>(g),
                static_cast<std::uint8_t>(b));

            shapes.push_back(shape);
        }
        else
        {
            std::cerr << "Unknown config entry: "
                      << type << '\n';
        }
    }

    return true;
}