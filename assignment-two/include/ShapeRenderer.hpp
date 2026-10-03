#pragma once

#include "ShapeData.hpp"

#include <SFML/Graphics.hpp>

void drawShape(
    sf::RenderWindow &window,
    const sf::Font &font,
    const ShapeData &shape);