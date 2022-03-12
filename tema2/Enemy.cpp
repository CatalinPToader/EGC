#include "lab_m1/tema2/Enemy.h"

#define DIST_FROM_CENTER 10

using namespace std;
using namespace homework;

Enemy::Enemy(glm::vec3 center, float rad, MeshInfo *_info, homework::Collidable::Type _t) : SphereCollidable(center, rad, _info, _t)
{
    position = center;
    radius = rad;
    size = glm::vec3(rad * 2);

    alive = true;
    deathTime = numeric_limits<float>::infinity();

    time = 0;
    state = 2;

    origin = position;

    lastPos = getPosFromMoveFunc(0);
}

Enemy::Enemy() = default;

Enemy::~Enemy() = default;


// timeOfDamage - time (as defined from framework engine) when damage was taken
// amount - how much damage should the enemy take
void Enemy::damage(float timeOfDamage, int amount)
{
    if (alive)
    {
        hitsLeft -= amount;
        if (hitsLeft <= 0)
        {
            alive = false;
            deathTime = timeOfDamage;
        }
    }
}

// Returns time of death for the enemy entity
float Enemy::getDeathTime() const
{
    return deathTime;
}

// Returns position of entity
glm::vec3 Enemy::getPosition()
{
    return position;
}

// Returns whether the death aniamtion for the entity should be over
bool Enemy::animationDone(float currentTime) const
{
    return currentTime - deathTime > DEATH_ANIM_DUR;
}

// The function returns a ratio trending down towards the true ratio of HP left
// It's used for nice interpolation between colors representing health, a call occurs every frame
// Returns a ratio of HP left
float Enemy::getRatio()
{
    if ((float)hitsLeft / MAX_HP < ratio)
        ratio -= 0.01;

    return ratio;
}


// The enemies movement is based on a lemniscate which given an input t (time) returns a position on that figure
// t - time
// Returns the position p at time t, given by the move function
glm::vec3 Enemy::getPosFromMoveFunc(float t)
{
    glm::vec3 pos = glm::vec3(0);

    float n = (1 + sin(t) * sin(t));

    pos.x = DIST_FROM_CENTER * cos(t) / n;
    pos.z = DIST_FROM_CENTER * sin(t) * cos(t) / n;

    return pos + origin;
}

// Updates the enemy state and position
void Enemy::Update(float deltaTimeSeconds, glm::vec3 playerPos)
{
    glm::vec3 dir = glm::vec3(0);

    if (state == states::patrol)
    {
        time += deltaTimeSeconds;
        dir = getPosFromMoveFunc(time) - position;
        dir = glm::normalize(dir);
    }
    else if (state == states::follow)
    {
        dir = playerPos - position;
        dir.y = 0;
        dir = glm::normalize(dir);
    }
    else
    {
        dir = lastPos - position;
        dir = glm::normalize(dir);
        if (glm::length(position - lastPos) < 0.1)
        {
            state = states::patrol;
        }
    }

    if (alive)
        position += dir * deltaTimeSeconds * enemySpeed;

    if (glm::length(playerPos - origin) < 10 && state == states::patrol)
    {
        lastPos = position;
        state = states::follow;
    }
    else if (glm::length(playerPos - origin) > 10 && state == states::follow)
    {
        state = states::back;
    }

    SphereCollidable::moveAbs(position);
}