#include "lab_m1/tema2/utils.h"

#include <iostream>

using namespace std;

glm::vec3 rotateToLocal(glm::vec3 centerSphere, glm::vec3 centerLocal, glm::vec3 axis, float angle) {
    glm::mat4 mat = glm::mat4(1);
    
    mat = glm::translate(mat, centerLocal);
    mat = glm::rotate(mat, -angle, axis);
    mat = glm::translate(mat, -centerLocal);
    glm::vec3 newCenter = mat * glm::vec4(centerSphere, 1);

    return newCenter;
}

std::vector<glm::vec3> getAxis(std::vector<glm::vec3> points) {
    glm::vec3 p1 = points[0];

    std::vector<glm::vec3> axis;
    for(auto p : points) {
        if (p == p1)
            continue;
        axis.push_back(p1 - p);
    }

    return axis;
}

glm::vec2 returnMinMaxOnAxis(glm::vec3 axis, const vector<glm::vec3>& points)
{
    float max, min;
    max = -std::numeric_limits<float>::max();
    min = -max;

    for (auto v : points)
    {
        float dot = glm::dot(axis, v);
        max = std::max(max, dot);
        min = std::min(min, dot);
    }

    return glm::vec2(min, max);
}

bool homework::inRange(float val, float min, float max) {
    return val > min && val < max;
}