#include "lab_m1/tema1/projectile.h"
#include "lab_m1/tema1/transform2D.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;

Projectile::Projectile()
{
}
Projectile::Projectile(glm::vec3 center, float radius, Type t, std::string mesh, glm::vec2 dir, float speed, float lifespan)
{
    this->center = center;
    this->radius = radius;
    this->t = t;
    this->mesh = mesh;
    this->dir = dir;
    this->speed = speed;
    this->lifespan = lifespan;
}

Projectile::~Projectile()
{
}

std::string Projectile::getMeshName()
{
    return mesh;
}

void Projectile::Init() {}

// Update the projectile each frame
void Projectile::Update(float delta)
{
    // Move projectile with a set speed
    center.x += speed * delta * dir.x;
    center.y += speed * delta * dir.y;

    // Decrease it's lifespan
    lifespan -= delta;
}

glm::mat3 Projectile::renderMatrix(glm::mat3 visMatrix) {
    glm::mat3 mat = visMatrix;
    mat *= transform2D::Translate(center.x, center.y);
    return mat;
}

void Projectile::moveAbs(float x, float y) {
    center.x = x;
    center.y = y;
}

void Projectile::moveRel(float x, float y) {
    center.x += x;
    center.y += y;
}

