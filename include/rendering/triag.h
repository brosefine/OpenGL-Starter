#pragma once

#include <glm/glm.hpp>

#include <vector>

// positions
std::vector<glm::vec3> triagPositions {
    {-1.0f, -1.0f, 0.0f},
    {0.f, 1.0f, 0.0f},
    {1.0f, -1.0f, 0.0f},
};

// texCoords
std::vector<glm::vec2> triagUVs {
    {0.0f, 0.0f},
    {0.5f, 1.0f},
    {1.0f, 0.0f},
};

std::vector<unsigned int> triagIndices{
    0, 1, 2
};