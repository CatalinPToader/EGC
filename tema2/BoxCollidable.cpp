#include "lab_m1/tema2/BoxCollidable.h"

#include <vector>

using namespace std;
using namespace homework;

BoxCollidable::BoxCollidable(glm::vec3 center, glm::vec3 scale, MeshInfo *_info, Type _t)
{
    position = center;
    size = scale;
    info = _info;
    t = _t;
}

BoxCollidable::BoxCollidable() = default;

BoxCollidable::~BoxCollidable() = default;


// Returns vector of corners for box collider
std::vector<glm::vec3> BoxCollidable::getPointsFromCenter()
{
    float pair1[2] = {size.x / 2, -size.x / 2};
    float pair2[2] = {size.y / 2, -size.y / 2};
    float pair3[2] = {size.z / 2, -size.z / 2};

    std::vector<glm::vec3> points;
    for (float i : pair1)
        for (float j : pair2)
            for (float k : pair3)
                points.push_back(position + glm::vec3(i, j, k));

    return points;
}

// Returns whether a point is inside the box collider
bool BoxCollidable::isInside(glm::vec3 point)
{
    auto pointsThis = getPointsFromCenter();

    // Vector of points to create the 3 axis required for checking point projection
    std::vector<glm::vec3> pointsForAxis;

    pointsForAxis.push_back(position + glm::vec3(size.x / 2, size.y / 2, size.z / 2));
    pointsForAxis.push_back(position + glm::vec3(-size.x / 2, size.y / 2, size.z / 2));
    pointsForAxis.push_back(position + glm::vec3(size.x / 2, -size.y / 2, size.z / 2));
    pointsForAxis.push_back(position + glm::vec3(size.x / 2, size.y / 2, -size.z / 2));

    float minA, maxA, minB, maxB;
    glm::vec2 res;


    auto axisThis = getAxis(pointsForAxis);
    vector<glm::vec3> pointsOther;
    pointsOther.push_back(point);

    // Projects point on axis and checks if it's inside bounds of box
    for (auto axis : axisThis)
    {
        res = returnMinMaxOnAxis(axis, pointsThis);
        minA = res.x;
        maxA = res.y;

        res = returnMinMaxOnAxis(axis, pointsOther);
        minB = res.x;
        maxB = res.y;

        bool check = inRange(minA, minB, maxB) || inRange(minB, minA, maxA);

        // If any axis fails the check, that means the point is outside the body
        if (!check)
            return false;
    }

    return true;
}

// Returns whether a sphere defined by center and radius collides with the box
bool BoxCollidable::collidesSphere(glm::vec3 center, float radius)
{
    glm::vec3 closest = glm::vec3(0);

    // For each world axis returns the closest point in the box to the center of sphere
    closest.x = max(position.x - size.x / 2, min(center.x, position.x + size.x / 2));
    closest.y = max(position.y - size.y / 2, min(center.y, position.y + size.y / 2));
    closest.z = max(position.z - size.z / 2, min(center.z, position.z + size.z / 2));

    // If the closest point is closer to the sphere center than the radius of the sphere, there's an intersection
    if (glm::length(center - closest) < radius)
        return true;

    return false;
}

// This version of the OBB/OBB collision was not needed, but Sphere/OBB uses it and it needed a definition inside the BoxCollider
bool BoxCollidable::collidesBox(glm::vec3 center, glm::vec3 sizeOther, float angleY)
{
    return false;
}

// axisOther - vector of local axis of the other OBB
// pointsOther - vector of corners of the other OBB
// Returns whether a collision between this OBB and the given OBB occurs
bool BoxCollidable::collidesBox(std::vector<glm::vec3> axisOther, std::vector<glm::vec3> pointsOther)
{
    auto pointsThis = getPointsFromCenter();
    bool check;

    float minA, maxA, minB, maxB;
    glm::vec2 res;

    // Checks for each provided local axis of other object
    for (auto axis : axisOther)
    {

        // Projects this OBB's corners on the axis, saves minimum and maximum projection
        res = returnMinMaxOnAxis(axis, pointsThis);
        minA = res.x;
        maxA = res.y;

        res = returnMinMaxOnAxis(axis, pointsOther);
        minB = res.x;
        maxB = res.y;

        // Checks whether the minimum of A is inside the projection range of B and other way
        check = inRange(minA, minB, maxB) || inRange(minB, minA, maxA);

        // If any axis fails the check (there's a gap) then the objects aren't colliding
        if (!check)
            return false;
    }

    // This project only uses OBB's that are world axis aligned, so the following code uses world axis as defined by the OBB
    std::vector<glm::vec3> pointsForAxis;

    pointsForAxis.push_back(position + glm::vec3(size.x / 2, size.y / 2, size.z / 2));
    pointsForAxis.push_back(position + glm::vec3(-size.x / 2, size.y / 2, size.z / 2));
    pointsForAxis.push_back(position + glm::vec3(size.x / 2, -size.y / 2, size.z / 2));
    pointsForAxis.push_back(position + glm::vec3(size.x / 2, size.y / 2, -size.z / 2));

    auto axisThis = getAxis(pointsForAxis);

    for (auto axis : axisThis)
    {
        res = returnMinMaxOnAxis(axis, pointsThis);
        minA = res.x;
        maxA = res.y;

        res = returnMinMaxOnAxis(axis, pointsOther);
        minB = res.x;
        maxB = res.y;

        check = inRange(minA, minB, maxB) || inRange(minB, minA, maxA);

        if (!check)
            return false;
    }

    return true;
}

// Returns a render matrix for OpenGL
glm::mat4 BoxCollidable::renderMatrix()
{
    glm::mat4 mat = glm::mat4(1);
    mat = glm::translate(mat, position);
    mat = glm::scale(mat, size);

    return mat;
}

// Moves the collider to a new position
void BoxCollidable::moveAbs(glm::vec3 newPosition) {
    position = newPosition;
}

homework::Collidable::Type BoxCollidable::getType()
{
    return t;
}

homework::MeshInfo *BoxCollidable::getInfo()
{
    return info;
}