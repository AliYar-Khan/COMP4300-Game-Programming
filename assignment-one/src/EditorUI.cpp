#include "EditorUI.hpp"

#include <cstdint>
#include <imgui.h>
#include <imgui_stdlib.h>

void drawEditorUI(
    std::vector<ShapeData> &shapes,
    int &selectedShape)
{
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

        ImGui::Combo(
            "Shape",
            &selectedShape,
            shapeNames.data(),
            static_cast<int>(shapeNames.size()));

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

        ImGui::SetNextItemWidth(150.0f);

        ImGui::DragFloat2(
            "Velocity",
            &shape.velocity.x,
            1.0f,
            -1000.0f,
            1000.0f,
            "%.3f");

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

        ImGui::InputText(
            "Name",
            &shape.name);
    }

    ImGui::End();
}