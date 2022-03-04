#include "lab_m1/tema1/BoxCollidable.h"
#include "lab_m1/tema1/transform2D.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;

BoxCollidable::BoxCollidable()
{
}

BoxCollidable::BoxCollidable(Type t, MeshInfo *info)
{
    this->info = info;
    this->center = info->center;

    if (info->t == MeshType::RECTANGLE)
    {
        sizeX = info->width;
        sizeY = info->height;
    }
    else if (info->t == MeshType::SQUARE)
        sizeX = sizeY = info->length;
    else
        cerr << "Wrong mesh for box collidable";
    this->t = t;

    scaleX = scaleY = 1;
    angle = 0;
}

BoxCollidable::~BoxCollidable()
{
}

std::string BoxCollidable::getMeshName()
{
    return info->meshName;
}

MeshInfo *BoxCollidable::getMeshInfo()
{
    return info;
}

Collidable::Type BoxCollidable::getType() {
    return this->t;
}

void BoxCollidable::Init()
{
}

void BoxCollidable::Update(float delta)
{
}

// Calculates the size of the diagonal for fast collision checks
float BoxCollidable::diagonal()
{
    float adjX = sizeX * scaleX;
    float adjY = sizeY * scaleY;
    return sqrt(adjX * adjX + adjY * adjY);
}

// Checks collision with a circle by rotating the circle to the be in the same axis system as the box
bool BoxCollidable::collidesCircle(glm::vec2 cCenter, float radius)
{
    glm::vec2 circle = rotateCenteredNegAngle(angle, cCenter, glm::vec2(center));

    glm::vec2 diff = circle - glm::vec2(center.x, center.y);
    if (glm::length(diff) > diagonal() / 2 + radius)
        return false;

    glm::vec2 half(sizeX * scaleX / 2, sizeY * scaleY / 2);

    // Clamps the value of the distance between the circle center and the box center to the box sides
    glm::vec2 clamped = glm::clamp(diff, -half, half);
    // Adds the center to the clamped value, giving us the closest point between the circle and the box
    glm::vec2 close = clamped + glm::vec2(center);

    // Simple check if the closest point is within the radius of the circle (meaning there's intersection)
    diff = close - circle;

    return glm::length(diff) < radius;
}


// Given an axis (as a mathemathical vector) and a vector of points
// returns the minimum and maximum projection on the axis (as a magnitude along the axis vector)
glm::vec2 returnMinMaxOnAxis(glm::vec2 axis, vector<glm::vec2> points)
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

// Checks for box on box collision by using SAT (Separating Axis Theorem)
bool BoxCollidable::collidesBox(glm::vec2 center, float sizeX, float sizeY, float angle)
{
    bool firstAxis = false;
    bool secondAxis = false;
    bool thirdAxis = false;
    bool fourthAxis = false;

    // Get the corners (extremes) of each box
    vector<glm::vec2> pointsA = pointsFromCenter(this->center, (this->sizeX * scaleX), (this->sizeY * scaleY), this->angle);
    vector<glm::vec2> pointsB = pointsFromCenter(center, sizeX, sizeY, angle);

    // Create an axis from the points of A
    glm::vec2 axis = glm::vec2(-pointsA[1].y + pointsA[0].y, pointsA[1].x - pointsA[0].x);

    axis = glm::normalize(axis);

    float minA, maxA, minB, maxB;
    glm::vec2 res;

    res = returnMinMaxOnAxis(axis, pointsA);
    minA = res.x;
    maxA = res.y;

    res = returnMinMaxOnAxis(axis, pointsB);
    minB = res.x;
    maxB = res.y;

    // Checks if the minimum projection of one body is between the minimum and maximum projection of the other
    // This will only return true in case of a collision
    firstAxis = inRange(minA, minB, maxB) || inRange(minB, minA, maxA);

    // This repeats another 3 times for a total of 4 axis (2 from box A and 2 from box B)
    // This assures that there's no situation in which we have false positives or negatives

    axis = glm::vec2(-pointsA[2].y + pointsA[1].y, pointsA[2].x - pointsA[1].x);

    axis = glm::normalize(axis);

    res = returnMinMaxOnAxis(axis, pointsA);
    minA = res.x;
    maxA = res.y;

    res = returnMinMaxOnAxis(axis, pointsB);
    minB = res.x;
    maxB = res.y;

    secondAxis = inRange(minA, minB, maxB) || inRange(minB, minA, maxA);

    axis = glm::vec2(-pointsA[3].y + pointsA[2].y, pointsA[3].x - pointsA[2].x);

    axis = glm::normalize(axis);

    res = returnMinMaxOnAxis(axis, pointsA);
    minA = res.x;
    maxA = res.y;

    res = returnMinMaxOnAxis(axis, pointsB);
    minB = res.x;
    maxB = res.y;

    thirdAxis = inRange(minA, minB, maxB) || inRange(minB, minA, maxA);

    axis = glm::vec2(-pointsA[0].y + pointsA[3].y, pointsA[0].x - pointsA[3].x);

    axis = glm::normalize(axis);

    res = returnMinMaxOnAxis(axis, pointsA);
    minA = res.x;
    maxA = res.y;

    res = returnMinMaxOnAxis(axis, pointsB);
    minB = res.x;
    maxB = res.y;

    fourthAxis = inRange(minA, minB, maxB) || inRange(minB, minA, maxA);

    return firstAxis && secondAxis && thirdAxis && fourthAxis;
}

// Render matrix for OpenGL
glm::mat3 BoxCollidable::renderMatrix(glm::mat3 visMatrix)
{
    glm::mat3 mat = visMatrix;
    mat *= transform2D::Translate(center.x, center.y);
    mat *= transform2D::Rotate(RADIANS(angle));
    mat *= transform2D::Scale(scaleX, scaleY);
    return mat;
}

// Move, Rotate and Scale functions
// Abs sets the absolute values while Rel changes the current value by the given one

void BoxCollidable::moveAbs(float x, float y)
{
    center.x = x;
    center.y = y;
}

void BoxCollidable::moveRel(float x, float y)
{
    center.x += x;
    center.y += y;
}

void BoxCollidable::rotateAbs(float angle)
{
    this->angle = angle;
}

void BoxCollidable::rotateRel(float ang)
{
    angle += ang;
}

void BoxCollidable::scaleAbs(float x, float y)
{
    scaleX = initialScaleX = x;
    scaleY = initialScaleY = y;
}

void BoxCollidable::scaleRel(float x, float y)
{
    scaleX += x * initialScaleX;
    scaleY += y * initialScaleY;
}

// Checks if a point is inside the box

bool BoxCollidable::isInside(float x, float y)
{
    // First it rotates the point to the same axis system as the box
    glm::vec2 rotate = rotateCenteredNegAngle(angle, glm::vec2(x, y), glm::vec2(center));

    if (rotate.x > center.x - sizeX * scaleX / 2 && rotate.x < center.x + sizeX * scaleX / 2)
    {
        if (rotate.y > center.y - sizeY * scaleY / 2 && rotate.y < center.y + sizeY * scaleY / 2)
            return true;
        return false;
    }
    return false;
}

// Aligns values to be integers to eliminate rounding errors in editor movement
void BoxCollidable::alignPosition()
{
    center.x = (int)center.x;
    center.y = (int)center.y;
}

// Aligns rotation in multiple of 5 degrees.
void BoxCollidable::alignRotation()
{
    angle = (int)angle;

    if ((int)angle % 5 < 3)
        angle = (angle / 5) * 5;
    else
        angle = ((angle / 5) + 1) * 5;
}