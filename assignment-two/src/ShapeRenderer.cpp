#include "ShapeRenderer.hpp"

void drawShape(
    sf::RenderWindow &window,
    const sf::Font &font,
    const ShapeData &shape)
{
    if (shape.type == ShapeData::Type::Circle)
    {
        const float radius = shape.size1;

        sf::CircleShape circle(radius);

        circle.setPosition(shape.position);
        circle.setFillColor(shape.color);

        window.draw(circle);

        sf::Text text(font, shape.name, 16);
        text.setFillColor(sf::Color::White);

        sf::FloatRect bounds = text.getLocalBounds();

        text.setOrigin(
            bounds.position + bounds.size / 2.0f);

        text.setPosition(
            shape.position +
            sf::Vector2f(radius, radius));

        window.draw(text);
    }
    else if (shape.type == ShapeData::Type::Rectangle)
    {
        const float width = shape.size1;
        const float height = shape.size2;

        sf::RectangleShape rectangle({width, height});

        rectangle.setPosition(shape.position);
        rectangle.setFillColor(shape.color);

        window.draw(rectangle);

        sf::Text text(font, shape.name, 16);
        text.setFillColor(sf::Color::White);

        sf::FloatRect bounds = text.getLocalBounds();

        text.setOrigin(
            bounds.position + bounds.size / 2.0f);

        text.setPosition(
            shape.position +
            sf::Vector2f(width / 2.0f, height / 2.0f));

        window.draw(text);
    }
}