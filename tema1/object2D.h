#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

// Series of functions for creating primitive 2D shapes

namespace object2D
{

    // Create square with given bottom left corner, length and color
    Mesh *CreateSquare(const std::string &name, glm::vec3 center, float length, glm::vec3 color, bool fill = false);
    Mesh *CreateSquareOutline(const std::string &name, glm::vec3 center, float length, float thickness, glm::vec3 color);
    Mesh *CreateRectangle(const std::string &name, glm::vec3 center, float sizeX, float sizeY, glm::vec3 color, bool fill = false);
    Mesh *CreateCircle(const std::string &name, glm::vec3 centerCircle, float radius, glm::vec3 color, bool fill = false);
    Mesh *CreateRing(const std::string &name, glm::vec3 centerCircle, float radius, float thickness, glm::vec3 color);
}
