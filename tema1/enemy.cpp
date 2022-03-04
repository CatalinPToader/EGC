#include "lab_m1/tema1/enemy.h"
#include "lab_m1/tema1/transform2D.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;

Enemy::Enemy()
{
}

Enemy::Enemy(Collidable::Type t, MeshInfo *info, float health, float speed, float size) : super(t, info)
{
    this->health = health;
    this->speed = speed;
    this->size = size;

    maxHealth = health;

    super::scaleAbs(size * defaultSize / sizeX, size * 100 / sizeY);
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
}

// Updates the enemy every frame
void Enemy::Update(float delta, Player *target, std::vector<Collidable *> walls)
{
    // If the player is dead, stop updating
    if (target->dead)
        return;

    float oldAngle = super::angle;

    // Rotate to face the player
    float angle = atan2(center.y - target->playerY, center.x - target->playerX);
    angle -= RADIANS(180);

    // Call the super function to rotate
    super::rotateAbs(DEGREES(angle));

    // Check for collision with all walls after rotation, cancel it if we are colliding
    for (auto w : walls)
    {
        if (w->getType() == Type::WALL)
            if (w->collidesBox(glm::vec2(center), size * defaultSize, size * defaultSize, DEGREES(angle)))
            {
                super::rotateAbs(oldAngle);
                break;
            }
    }

    // Move towards the direction we are facing with constant speed
    float dirX = cos(angle);
    float dirY = sin(angle);

    // Call the super function for movement
    super::moveRel(delta * speed * dirX, delta * speed * dirY);

    // Check for collision with all walls after movement, cancel it if we are colliding
    for (auto w : walls)
    {
        if (w->getType() == Type::WALL)
            if (w->collidesBox(glm::vec2(center), size * defaultSize, size * defaultSize, DEGREES(angle)))
            {
                super::moveRel(-delta * speed * dirX, -delta * speed * dirY);
                break;
            }
    }
}

// Returns the render matrix for the body of the enemy
glm::mat3 Enemy::getBodyMatrix(glm::mat3 visMatrix)
{
    return super::renderMatrix(visMatrix);
}

// Returns render matrix for the left arm
glm::mat3 Enemy::getLeftArmMatrix(glm::mat3 visMatrix)
{
    float dist = defaultSize * size / 2;

    float xScale = size * defaultSize / sizeX, yScale = size * 100 / sizeY;
    glm::mat3 modelMatrix = getBodyMatrix(visMatrix);
    modelMatrix *= transform2D::Scale(1 / xScale, 1 / yScale);
    modelMatrix *= transform2D::Translate(dist * 5 / 6, -dist * 5 / 6);
    modelMatrix *= transform2D::Scale(0.5f, 0.5f);
    modelMatrix *= transform2D::Scale(xScale, yScale);

    return modelMatrix;
}

// Returns render matrix for the right arm
glm::mat3 Enemy::getRightArmMatrix(glm::mat3 visMatrix)
{
    float dist = defaultSize * size / 2;
    float xScale = size * defaultSize / sizeX, yScale = size * 100 / sizeY;
    glm::mat3 modelMatrix = getBodyMatrix(visMatrix);
    modelMatrix *= transform2D::Scale(1 / xScale, 1 / yScale);
    modelMatrix *= transform2D::Translate(dist * 5 / 6, dist * 5 / 6);
    modelMatrix *= transform2D::Scale(0.5f, 0.5f);
    modelMatrix *= transform2D::Scale(xScale, yScale);

    return modelMatrix;
}