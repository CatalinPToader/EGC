#include "lab_m1/tema2/SphereCollidable.h"

#include <vector>

using namespace std;
using namespace homework;

SphereCollidable::SphereCollidable(glm::vec3 center, float rad, MeshInfo *_info, Type _t)
{
    position = center;
    size = glm::vec3(2 * rad);
    info = _info;
    t = _t;
    radius = rad;
}

SphereCollidable::SphereCollidable() = default;

SphereCollidable::~SphereCollidable() = default;

bool SphereCollidable::isInside(glm::vec3 point)
{
    return glm::length(point - position) < radius;
}

bool SphereCollidable::collidesSphere(glm::vec3 center, float otherRadius)
{
    return glm::length(center - position) < otherRadius + radius;
}

bool SphereCollidable::collidesBox(glm::vec3 center, glm::vec3 otherSize, float angleY)
{
    glm::vec3 localCenter = rotateToLocal(position, center, glm::vec3(0, 1, 0), angleY);

    glm::vec3 closest = glm::vec3(0);

    closest.x = max(center.x - otherSize.x / 2, min(localCenter.x, center.x + otherSize.x / 2));
    closest.y = max(center.y - otherSize.y / 2, min(localCenter.y, center.y + otherSize.y / 2));
    closest.z = max(center.z - otherSize.z / 2, min(localCenter.z, center.z + otherSize.z / 2));

    if (glm::length(localCenter - closest) < radius)
        return true;

    return false;
}

bool SphereCollidable::collidesBox(std::vector<glm::vec3> axisOther, std::vector<glm::vec3> pointsOther)
{
    return false;
}

glm::mat4 SphereCollidable::renderMatrix()
{
    glm::mat4 mat = glm::mat4(1);
    mat = glm::translate(mat, position);
    mat = glm::scale(mat, size);

    return mat;
}

void SphereCollidable::moveAbs(glm::vec3 newPosition) {
    this->position = newPosition;
}

homework::Collidable::Type SphereCollidable::getType()
{
    return t;
}

homework::MeshInfo *SphereCollidable::getInfo()
{
    return info;
}