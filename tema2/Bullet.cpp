#include "lab_m1/tema2/Bullet.h"


using namespace std;
using namespace homework;

Bullet::Bullet(glm::vec3 center, float rad, MeshInfo *_info, homework::Collidable::Type _t, glm::vec3 dir) : SphereCollidable(center, rad, _info, _t)
{
    this->dir = dir;
    position = center;
    radius = rad;
    size = glm::vec3(rad * 2);

    time = 0;
}

Bullet::Bullet() = default;

Bullet::~Bullet() = default;

// deltaTime - time between frames
void Bullet::Update(float deltaTime) {
    time += deltaTime;

    // Updates position of parent class where collisions are handled
    position += dir * deltaTime * BULLET_SPEED;
    SphereCollidable::position = position;
}

// Returns whether the bullet should be despawned (by exceeding it's lifetime)
bool Bullet::despawned() const {
    return time > LIFETIME;
}

// Returns the bullet's position
glm::vec3 Bullet::getPosition() {
    return position;
}