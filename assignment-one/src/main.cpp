#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <imgui_stdlib.h>

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

    float size1;
    float size2;

    bool draw = true;
};

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

            file >> shape.name >> shape.position.x >> shape.position.y >> shape.velocity.x >> shape.velocity.y >> r >> g >> b >> shape.size1;

            shape.color = sf::Color(
                static_cast<std::uint8_t>(r),
                static_cast<std::uint8_t>(g),
                static_cast<std::uint8_t>(b));

            shape.size2 = 0.f;

            shapes.push_back(shape);
        }
        else if (type == "Rectangle")
        {
            ShapeData shape;

            shape.type = ShapeData::Type::Rectangle;

            int r;
            int g;
            int b;

            file >> shape.name >> shape.position.x >> shape.position.y >> shape.velocity.x >> shape.velocity.y >> r >> g >> b >> shape.size1 >> shape.size2;

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

    std::cout << "Window: "
              << windowWidth
              << "x"
              << windowHeight
              << '\n';

    std::cout << "Shapes loaded: "
              << shapes.size()
              << '\n';

    for (const auto &shape : shapes)
    {
        std::cout << "  "
                  << shape.name
                  << '\n';
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
        }

        ImGui::Begin("Shape Properties");

        if (!shapes.empty())
        {
            ShapeData &shape = shapes[selectedShape];

            std::vector<const char *> shapeNames;

            for (const auto &s : shapes)
            {
                shapeNames.push_back(s.name.c_str());
            }

            ImGui::SetNextItemWidth(150.0f);

            if (ImGui::Combo(
                    "Shape",
                    &selectedShape,
                    shapeNames.data(),
                    static_cast<int>(shapeNames.size())))
            {
                // selectedShape is automatically updated
            };

            ImGui::Checkbox("Draw", &shape.draw);

            if (shape.type == ShapeData::Type::Circle)
            {
                ImGui::SetNextItemWidth(150.0f);

                ImGui::DragFloat(
                    "Scale",
                    &shape.size1,
                    1.0f,
                    1.0f,
                    500.0f,
                    "%.3f");
            }
            else if (shape.type == ShapeData::Type::Rectangle)
            {
                ImGui::SetNextItemWidth(150.0f);

                ImGui::DragFloat(
                    "Width",
                    &shape.size1,
                    1.0f,
                    1.0f,
                    800.0f,
                    "%.3f");

                ImGui::SetNextItemWidth(150.0f);

                ImGui::DragFloat(
                    "Height",
                    &shape.size2,
                    1.0f,
                    1.0f,
                    600.0f,
                    "%.3f");
            }

            // Velocity
            ImGui::SetNextItemWidth(150.0f);

            ImGui::DragFloat2(
                "Velocity",
                &shape.velocity.x,
                1.0f,
                -1000.0f,
                1000.0f,
                "%.3f"
            );

            // Color
            float color[3] = {
                shape.color.r / 255.0f,
                shape.color.g / 255.0f,
                shape.color.b / 255.0f};

            ImGui::SetNextItemWidth(150.0f);

            if (ImGui::ColorEdit3("Color", color))
            {
                shape.color = sf::Color(
                    static_cast<std::uint8_t>(color[0] * 255.0f),
                    static_cast<std::uint8_t>(color[1] * 255.0f),
                    static_cast<std::uint8_t>(color[2] * 255.0f));
            }

            ImGui::SetNextItemWidth(150.0f);
            ImGui::InputText("Name", &shape.name);
        }

        ImGui::End();

        ImGui::SFML::Render(window);

        window.display();
    }
    ImGui::SFML::Shutdown();

    return 0;
}
