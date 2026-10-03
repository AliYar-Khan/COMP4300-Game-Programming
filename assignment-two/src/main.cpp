#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>

#include "Config.hpp"
#include "EditorUI.hpp"
#include "ShapeRenderer.hpp"

#include <iostream>
#include <vector>


int main()
{
    unsigned int windowWidth = 800;
    unsigned int windowHeight = 600;

    std::vector<ShapeData> shapes;

    int selectedShape = 0;

    if (!loadConfig(
            "config/config.txt",
            windowWidth,
            windowHeight,
            shapes))
    {
        return 1;
    }


    sf::RenderWindow window(
        sf::VideoMode({windowWidth, windowHeight}),
        "SFML Shapes");

    sf::Font font;

    if (!font.openFromFile("./assets/fonts/tech.ttf"))
    {
        std::cerr << "Failed to load font\n";
        return 1;
    }

    ImGui::SFML::Init(window);

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Time deltaTime = clock.restart();

        ImGui::SFML::Update(window, deltaTime);

        while (const std::optional event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear(sf::Color::Black);

        for (auto &shape : shapes)
        {
            if (!shape.draw)
            {
                continue;
            }

            shape.position += shape.velocity * deltaTime.asSeconds();

            if (shape.type == ShapeData::Type::Circle)
            {
                const float radius = shape.size1;
                const float diameter = radius * 2.0f;

                // Left / right
                if (shape.position.x <= 0.0f)
                {
                    shape.position.x = 0.0f;
                    shape.velocity.x = -shape.velocity.x;
                }
                else if (shape.position.x + diameter >= windowWidth)
                {
                    shape.position.x = windowWidth - diameter;
                    shape.velocity.x = -shape.velocity.x;
                }

                // Top / bottom
                if (shape.position.y <= 0.0f)
                {
                    shape.position.y = 0.0f;
                    shape.velocity.y = -shape.velocity.y;
                }
                else if (shape.position.y + diameter >= windowHeight)
                {
                    shape.position.y = windowHeight - diameter;
                    shape.velocity.y = -shape.velocity.y;
                }

                sf::CircleShape circle(radius);
                circle.setPosition(shape.position);
                circle.setFillColor(shape.color);
                window.draw(circle);

                sf::Text text(font, shape.name, 16);
                text.setFillColor(sf::Color::White);

                sf::FloatRect textBounds = text.getLocalBounds();

                text.setOrigin(
                    textBounds.position + textBounds.size / 2.0f);

                text.setPosition(
                    shape.position + sf::Vector2f(radius, radius));

                window.draw(text);
            }
            else if (shape.type == ShapeData::Type::Rectangle)
            {
                const float width = shape.size1;
                const float height = shape.size2;

                // Left / right
                if (shape.position.x <= 0.0f)
                {
                    shape.position.x = 0.0f;
                    shape.velocity.x = -shape.velocity.x;
                }
                else if (shape.position.x + width >= windowWidth)
                {
                    shape.position.x = windowWidth - width;
                    shape.velocity.x = -shape.velocity.x;
                }

                // Top / bottom
                if (shape.position.y <= 0.0f)
                {
                    shape.position.y = 0.0f;
                    shape.velocity.y = -shape.velocity.y;
                }
                else if (shape.position.y + height >= windowHeight)
                {
                    shape.position.y = windowHeight - height;
                    shape.velocity.y = -shape.velocity.y;
                }
                sf::RectangleShape rectangle({width, height});
                rectangle.setPosition(shape.position);
                rectangle.setFillColor(shape.color);
                window.draw(rectangle);

                sf::Text text(font, shape.name, 16);
                text.setFillColor(sf::Color::White);

                sf::FloatRect textBounds = text.getLocalBounds();

                text.setOrigin(
                    textBounds.position + textBounds.size / 2.0f);

                text.setPosition(
                    shape.position + sf::Vector2f(width / 2.0f, height / 2.0f));

                window.draw(text);
            }
            
            drawShape(window, font, shape);
        }

         drawEditorUI(
            shapes,
            selectedShape);

        ImGui::SFML::Render(window);

        window.display();
    }
    ImGui::SFML::Shutdown();

    return 0;
}
