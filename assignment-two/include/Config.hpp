#pragma once

#include "ShapeData.hpp"

#include <string>
#include <vector>

bool loadConfig(
    const std::string &filename,
    unsigned int &windowWidth,
    unsigned int &windowHeight,
    std::vector<ShapeData> &shapes);