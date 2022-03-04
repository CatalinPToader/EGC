#include "lab_m1/tema1/utils.h"


// Read a vec3 from an input stream (reads the brackets and the 3 values inside)
void readVec3File(std::ifstream &fin, glm::vec3 *res)
{
    char s, e;
    fin >> s >> res->x >> res->y >> res->z >> e;
}

// Rotate a point around a center by the negative of the angle
// This helps bring objects to the same axis system
glm::vec2 rotateCenteredNegAngle(float angle, glm::vec2 point, glm::vec2 center)
{
    glm::vec2 res = glm::vec2(0, 0);
    float rad = RADIANS(-angle);
    res.x = glm::cos(rad) * (point.x - center.x) - glm::sin(rad) * (point.y - center.y) + center.x;
    res.y = glm::sin(rad) * (point.x - center.x) + glm::cos(rad) * (point.y - center.y) + center.y;

    return res;
}

// Rotate a point around a center by a positive radian amount
// This helps bring objects to the same axis system
glm::vec2 rotateCenteredPosRad(float rad, glm::vec2 point, glm::vec2 center)
{
    glm::vec2 res = glm::vec2(0, 0);
    res.x = glm::cos(rad) * (point.x - center.x) - glm::sin(rad) * (point.y - center.y) + center.x;
    res.y = glm::sin(rad) * (point.x - center.x) + glm::cos(rad) * (point.y - center.y) + center.y;

    return res;
}

// Returns the extremes of a square/rectangle from its center and sizes
std::vector<glm::vec2> pointsFromCenter(glm::vec2 center, float sizeX, float sizeY, float angle)
{
    std::vector<glm::vec2> points;

    glm::vec2 tr = glm::vec2(center.x + sizeX / 2, center.y + sizeY / 2);
    tr = rotateCenteredPosRad(RADIANS(angle), tr, center);

    points.push_back(tr);

    glm::vec2 tl = glm::vec2(center.x - sizeX / 2, center.y + sizeY / 2);
    tl = rotateCenteredPosRad(RADIANS(angle), tl, center);

    points.push_back(tl);

    glm::vec2 bl = glm::vec2(center.x - sizeX / 2, center.y - sizeY / 2);
    bl = rotateCenteredPosRad(RADIANS(angle), bl, center);

    points.push_back(bl);

    glm::vec2 br = glm::vec2(center.x + sizeX / 2, center.y - sizeY / 2);
    br = rotateCenteredPosRad(RADIANS(angle), br, center);

    points.push_back(br);

    return points;
}

// Checks if a value is between 2 other values
bool inRange(float val, float min, float max) {
    return val > min && val < max;
}