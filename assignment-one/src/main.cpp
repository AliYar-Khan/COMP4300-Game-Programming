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
            shape.position += shape.velocity * deltaTime.asSeconds();
            ;

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
            }
        }

        ImGui::Begin("Shapes");

        for (int i = 0; i < static_cast<int>(shapes.size()); ++i)
        {
            if (ImGui::Selectable(shapes[i].name.c_str(), selectedShape == i))
            {
                selectedShape = i;
            }
        }

        if (!shapes.empty())
        {
            ShapeData &shape = shapes[selectedShape];

            ImGui::Separator();

            char nameBuffer[256];

            if (shape.type == ShapeData::Type::Circle)
            {
                ImGui::DragFloat("Radius", &shape.size1, 1.0f, 1.0f, 500.0f);
            }
            else if (shape.type == ShapeData::Type::Rectangle)
            {
                ImGui::DragFloat("Width", &shape.size1, 1.0f, 1.0f, 800.0f);
                ImGui::DragFloat("Height", &shape.size2, 1.0f, 1.0f, 600.0f);
            }

            std::snprintf(
                nameBuffer,
                sizeof(nameBuffer),
                "%s",
                shape.name.c_str());

            if (ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer)))
            {
                shape.name = nameBuffer;
            }

            ImGui::InputFloat2(
                "Position",
                &shape.position.x);

            ImGui::InputFloat2(
                "Velocity",
                &shape.velocity.x);

            int color[3] = {
                shape.color.r,
                shape.color.g,
                shape.color.b};

            if (ImGui::InputInt3("Color", color))
            {
                color[0] = std::clamp(color[0], 0, 255);
                color[1] = std::clamp(color[1], 0, 255);
                color[2] = std::clamp(color[2], 0, 255);

                shape.color = sf::Color(
                    static_cast<std::uint8_t>(color[0]),
                    static_cast<std::uint8_t>(color[1]),
                    static_cast<std::uint8_t>(color[2]));
            }
        }

        ImGui::End();

        ImGui::SFML::Render(window);

        window.display();
    }
    ImGui::SFML::Shutdown();

    return 0;
}
