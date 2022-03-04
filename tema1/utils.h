#pragma once

#include "components/simple_scene.h"

#include <string>
#include <fstream>

// Utilities and structures that all classes can refer to

// Defines a viewport which changes based on the window
struct ViewportSpace
{
    ViewportSpace() : x(0), y(0), width(1), height(1) {}
    ViewportSpace(int x, int y, int width, int height)
        : x(x), y(y), width(width), height(height) {}
    int x;
    int y;
    int width;
    int height;
};

// Defines a logic space in which objects reside
struct LogicSpace
{
    LogicSpace() : x(0), y(0), width(1), height(1) {}
    LogicSpace(float x, float y, float width, float height)
        : x(x), y(y), width(width), height(height) {}
    float x;
    float y;
    float width;
    float height;
};

// All types of meshes in the game
enum MeshType
{
    NONE,
    SQUARE,
    RECTANGLE,
    CIRCLE,
    RING
};

// A structure detailing the information of a mesh
struct MeshInfo
{
    MeshInfo() : meshName(""), center(glm::vec3(0, 0, 0)), color(glm::vec3(0, 0, 0)),
                 fill(true), t(NONE), width(0), height(0), radius(0), thickness(0), length(0) {}
    std::string meshName;
    glm::vec3 center;
    glm::vec3 color;
    bool fill;
    MeshType t;

    // 0 for unused, fill as needed by each mesh type
    float width;
    float height;
    float radius;
    float thickness;
    float length;
};

void readVec3File(std::ifstream &fin, glm::vec3 *res);

glm::vec2 rotateCenteredNegAngle(float angle, glm::vec2 circle, glm::vec2 box);

std::vector<glm::vec2> pointsFromCenter(glm::vec2 center, float sizeX, float sizeY, float angle);

bool inRange(float val, float min, float max);