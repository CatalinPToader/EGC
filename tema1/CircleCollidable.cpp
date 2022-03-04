#include "lab_m1/tema1/CircleCollidable.h"
#include "lab_m1/tema1/transform2D.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;

CircleCollidable::CircleCollidable()
{
}

CircleCollidable::CircleCollidable(Type t, MeshInfo *info)
{
    this->info = info;
    this->center = info->center;
    this->radius = info->radius;
    this->t = t;

    scaleX = scaleY = 1;
    angle = 0;
}

CircleCollidable::~CircleCollidable()
{
}

std::string CircleCollidable::getMeshName()
{
    return info->meshName;
}

MeshInfo *CircleCollidable::getMeshInfo()
{
    return info;
}

Collidable::Type CircleCollidable::getType() {
    return this->t;
}

void CircleCollidable::Init()
{
}

void CircleCollidable::Update(float delta)
{
}


// Check if 2 circles collide by comparing distance between centers
bool CircleCollidable::collidesCircle(glm::vec2 cCenter, float radius)
{
    float dist = glm::length(glm::vec2(center) - cCenter);

    if (dist > this->radius + radius)
        return true;
    return false;
}

// Check if circle collides with a box
// Same function as BoxCollidable::collidesCircle but from the circle perspective
bool CircleCollidable::collidesBox(glm::vec2 bCenter, float sizeX, float sizeY, float angle)
{
    glm::vec2 circle = rotateCenteredNegAngle(angle, center, bCenter);

    float diag = sqrt(sizeX * sizeX + sizeY * sizeY);

    glm::vec2 diff = circle - glm::vec2(center.x, center.y);
    if (glm::length(diff) > diag/2+radius)
        return false;
    
    glm::vec2 half(sizeX * scaleX / 2, sizeY * scaleY / 2);

    glm::vec2 clamped = glm::clamp(diff, -half, half);
    glm::vec2 close = clamped + glm::vec2(center);

    diff = close - circle;

    return glm::length(diff) < radius;
    return false;
}

// Render matrix for OpenGL.
glm::mat3 CircleCollidable::renderMatrix(glm::mat3 visMatrix)
{
    glm::mat3 mat = visMatrix;
    mat *= transform2D::Translate(center.x, center.y);
    mat *= transform2D::Rotate(RADIANS(angle));
    mat *= transform2D::Scale(scaleX, scaleY);
    return mat;
}

// Move, Rotate and Scale functions
// Abs sets the absolute values while Rel changes the current value by the given one

void CircleCollidable::moveAbs(float x, float y)
{
    center.x = x;
    center.y = y;
}

void CircleCollidable::moveRel(float x, float y)
{
    center.x += x;
    center.y += y;
}

void CircleCollidable::rotateAbs(float angle)
{
    this->angle = angle;
}

void CircleCollidable::rotateRel(float ang)
{
    angle += ang;
}

void CircleCollidable::scaleAbs(float x, float y)
{
    scaleX = x;
    scaleY = y;
}

void CircleCollidable::scaleRel(float x, float y)
{
    scaleX += x;
    scaleY += y;
}


// Checks if a point is inside the circle by comparing distance to radius
bool CircleCollidable::isInside(float x, float y)
{
    float newX = x - center.x;
    float newY = y - center.y;
    if (sqrt(newX * newX + newY * newY) <= radius)
        return true;
    return false;
}

// Aligns rotation to the integer for editor movement
void CircleCollidable::alignPosition()
{
    center.x = (int)center.x;
    center.y = (int)center.y;
}

// Circles don't differ visually when rotated
void CircleCollidable::alignRotation()
{
}