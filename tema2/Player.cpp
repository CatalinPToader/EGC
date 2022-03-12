#include "lab_m1/tema2/Player.h"

#include <utility>
#include <vector>
#include <iostream>

using namespace std;
using namespace homework;

Player::Player() = default;

Player::Player(glm::vec3 position, std::vector<homework::Collidable *> walls)
{
    this->position = position;
    this->velocity = glm::vec3(0, 0, 0);
    onGround = true;
    this->walls = std::move(walls);

    rotY = 0;
    time = 0;
    stopTime = 0;
}

Player::~Player() = default;

std::vector<glm::vec3> Player::getPointsForAxis()
{
    vector<glm::vec3> pointsForAxis;

    glm::vec3 point = position + glm::vec3(chestW / 2, chestH / 2 + displacement + headL, playerDepth / 2);
    point = glm::rotate(glm::mat4(1), rotY, glm::vec3(0, 1, 0)) * glm::vec4(point, 1);
    pointsForAxis.push_back(point);

    point = position + glm::vec3(-chestW / 2, chestH / 2 + displacement + headL, playerDepth / 2);
    point = glm::rotate(glm::mat4(1), rotY, glm::vec3(0, 1, 0)) * glm::vec4(point, 1);
    pointsForAxis.push_back(point);

    point = position + glm::vec3(chestW / 2, -(chestH / 2 + displacement + legH), playerDepth / 2);
    point = glm::rotate(glm::mat4(1), rotY, glm::vec3(0, 1, 0)) * glm::vec4(point, 1);
    pointsForAxis.push_back(point);

    point = position + glm::vec3(chestW / 2, chestH / 2 + displacement + headL, -playerDepth / 2);
    point = glm::rotate(glm::mat4(1), rotY, glm::vec3(0, 1, 0)) * glm::vec4(point, 1);
    pointsForAxis.push_back(point);

    return pointsForAxis;
}

std::vector<glm::vec3> Player::getLocalAxis()
{
    return getAxis(getPointsForAxis());
}

std::vector<glm::vec3> Player::getPoints()
{
    float pair1[2] = {chestW / 2, -chestW / 2};
    float pair2[2] = {chestH / 2 + displacement + headL, -(chestH / 2 + displacement + legH)};
    float pair3[2] = {playerDepth / 2, -playerDepth / 2};

    std::vector<glm::vec3> points;
    for (float i : pair1)
        for (float j : pair2)
            for (float k : pair3)
                points.push_back(position + glm::vec3(i, j, k));

    return points;
}

void Player::Update(float angle, float deltaTime)
{
    time += deltaTime;
    if (!isStill())
        stopTime = time;

    rotY = angle;

    position.x += velocity.x;

    bool collision = checkCollision();
    if (collision)
        position.x -= velocity.x;

    position.z += velocity.z;

    collision = checkCollision();
    if (collision)
        position.z -= velocity.z;

    position.y += velocity.y;
    collision = checkCollision();

    if (collision)
    {
        position.y -= velocity.y;
        velocity.y = 0;
        onGround = true;
    }

    applyDrag();

    if (!onGround)
    {
        float magnitude = velocity.y > 0 ? 1 : 0.33;
        velocity.y += gravity * magnitude;
    }
}

void Player::applyDrag() {
    if (velocity.x != 0)
        velocity.x = (1 - drag) * velocity.x;
    if (velocity.z != 0)
        velocity.z = (1 - drag) * velocity.z;

    if (abs(velocity.x) < 1.f / 1000)
        velocity.x = 0;
    if (abs(velocity.z) < 1.f / 1000)
        velocity.z = 0;
}

bool Player::checkCollision() {
    bool collision = false;
    for (auto it = walls.begin(); it != walls.end() && !collision; it++)
    {
        Collidable *w = *it;
        if (w->collidesBox(getLocalAxis(), getPoints()))
        {
            collision = true;
            if (w->getInfo()->getTexture() == "default")
                won = true;
        }
    }
    return collision;
}

glm::mat4 Player::centerMatrix()
{
    glm::mat4 mat = glm::mat4(1);
    mat = glm::translate(mat, position);
    mat = glm::rotate(mat, rotY, glm::vec3(0, 1, 0));
    return mat;
}

glm::mat4 Player::bodyMatrix()
{
    glm::mat4 mat = centerMatrix();
    mat = glm::scale(mat, glm::vec3(chestW, chestH, playerDepth));
    return mat;
}

glm::mat4 Player::headMatrix()
{
    glm::mat4 mat = centerMatrix();
    mat = glm::translate(mat, glm::vec3(0, (chestH + headL) / 2 + displacement, 0));
    mat = glm::scale(mat, glm::vec3(headL, headL, headL));

    return mat;
}

glm::mat4 Player::handMatrix(bool left)
{
    glm::mat4 mat = centerMatrix();
    int sign = left ? 1 : -1;
    mat = glm::translate(mat, glm::vec3(sign * ((chestW + armW) / 2 + displacement), (handL - chestH) / 2 - displacement, 0));
    mat = glm::scale(mat, glm::vec3(handL, handL, playerDepth));

    return mat;
}

glm::mat4 Player::armMatrix(bool left)
{
    glm::mat4 mat = centerMatrix();
    int sign = left ? 1 : -1;
    mat = glm::translate(mat, glm::vec3(sign * ((chestW + armW) / 2 + displacement), handL / 2, 0));
    mat = glm::scale(mat, glm::vec3(armW, armH, playerDepth));

    return mat;
}

glm::mat4 Player::legMatrix(bool left)
{
    glm::mat4 mat = centerMatrix();
    int sign = left ? 1 : -1;
    if (velocity.x != 0 || velocity.z != 0)
    {
        glm::vec3 velocityFlat = glm::vec3(velocity.x, 0, velocity.z);
        float magnitude = log2(glm::length(velocityFlat * 10.f) + 1) * sin(time * 8);
        mat = glm::rotate(mat, sign * magnitude * RADIANS(30), glm::vec3(1, 0, 0));
    }

    mat = glm::translate(mat, glm::vec3(sign * ((chestW - legW) / 2), -((chestH + legH) / 2 + displacement), 0));
    mat = glm::scale(mat, glm::vec3(legW, legH, playerDepth));

    return mat;
}

void Player::moveAbs(glm::vec3 pos)
{
    position = pos;
}

glm::vec3 Player::getPosition()
{
    return position;
}

glm::vec3 Player::getCameraPosition()
{
    return position + glm::vec3(0, (chestH + headL) / 2, 0);
}

void Player::applyVelocity(glm::vec3 newVelocity)
{
    this->velocity += newVelocity;

    if (newVelocity.y > 0)
        onGround = false;
}

bool Player::isOnGround() const
{
    return onGround;
}

bool Player::isStill() const
{
    return velocity.x == 0 && velocity.y == 0 && velocity.z == 0;
}

float Player::getHeightFeetToCenter() const
{
    return legH + chestH / 2 + displacement;
}

float Player::timeSinceStopped() const
{
    return time - stopTime;
}

glm::vec3 Player::getCenterCollisionBox() const {
    return glm::vec3(position.x, position.y - getHeightFeetToCenter() + playerHeight / 2, position.z);
}

glm::vec3 Player::getSizeCollisionBox() const {
    return glm::vec3(playerWidth, playerHeight, playerDepth);
}

void Player::damage(float damageTime) {
    if (damageTime - lastHitTime > DAMAGE_COOLDOWN) {
        lastHitTime = damageTime;
        health -= 1;
    }
}

float Player::getHealth() const {
    return health;
}