#include "lab_m1/tema1/player.h"

#include <vector>
#include <iostream>

#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/tema1/object2D.h"
#include "lab_m1/tema1/Collidable.h"

using namespace std;
using namespace m1;

Player::Player()
{
}

Player::Player(WindowObject *w, float x, float y, float radius, glm::vec3 centerLow, float zCenterHigh)
{
    window = w;
    this->centerLow = centerLow;
    centerHigh = centerLow;
    centerHigh.z = zCenterHigh;

    this->radius = radius;
    playerX = x;
    playerY = y;

    speed = 200;
    health = maxHealth = 200;
    dead = false;
}

Player::~Player()
{
}

// Initializes the player with some standard values
void Player::Init()
{
    centerLow = glm::vec3(0, 0, -0.1);
    centerHigh = glm::vec3(0, 0, 0);
    radius = 50;

    playerX = 1000;
    playerY = 1500;
    speed = 200;
    angle = 0;
}

// Returns the render matrix for the body
glm::mat3 Player::getBodyMatrix(glm::mat3 visMatrix)
{
    // Checks if player died between frames
    if (health < 0 && !dead)
    {
        dead = true;
    }

    glm::mat3 modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(playerX, playerY);
    modelMatrix *= transform2D::Rotate(RADIANS(angle));

    // If player died, don't render the body anymore
    if (dead)
        modelMatrix *= transform2D::Scale(0, 0);

    return modelMatrix;
}

glm::mat3 Player::getLeftArmMatrix(glm::mat3 visMatrix)
{
    glm::mat3 modelMatrix = getBodyMatrix(visMatrix);
    modelMatrix *= transform2D::Translate(radius * 5 / 6, -radius * 3 / 5);
    modelMatrix *= transform2D::Scale(0.5f, 0.5f);

    return modelMatrix;
}

glm::mat3 Player::getRightArmMatrix(glm::mat3 visMatrix)
{
    glm::mat3 modelMatrix = getBodyMatrix(visMatrix);
    modelMatrix *= transform2D::Translate(radius * 5 / 6, radius * 3 / 5);
    modelMatrix *= transform2D::Scale(0.5f, 0.5f);

    return modelMatrix;
}

// Handles player movement
void Player::playerMove(float delta, int mods, vector<m1::Collidable *> v, LogicSpace *l)
{
    // If player died between calls, set as dead.
    // If player is dead, no movement.
    if (health < 0 && !dead)
    {
        dead = true;
        return;
    }
    else if (dead)
        return;

    // Adjustment for holding shift to run
    float adj = 1.0;

    if (mods & GLFW_MOD_SHIFT)
        adj *= 2;

    float change = speed * delta * adj;

    // Grabs inputs from window here instead of the main file because it eliminates a jitter effect when moving against objects (Poorly handled)
    if (window->KeyHold(GLFW_KEY_W))
    {
        bool collision = false;
        playerY += change;

        // For all walls check collision
        for (auto it = v.begin(); it != v.end() && !collision; it++)
            // Player is a circle so call the function for circle collision of wall
            if ((*it)->collidesCircle(glm::vec2(playerX, playerY), radius))
                collision = true;

        //  Revert movement if collision happened
        if (collision)
            playerY -= change;
    }
    if (window->KeyHold(GLFW_KEY_S))
    {
        bool collision = false;
        playerY -= change;
        for (auto it = v.begin(); it != v.end() && !collision; it++)
            if ((*it)->collidesCircle(glm::vec2(playerX, playerY), radius))
                collision = true;
        if (collision)
            playerY += change;
    }
    if (window->KeyHold(GLFW_KEY_D))
    {
        bool collision = false;
        playerX += change;
        for (auto it = v.begin(); it != v.end() && !collision; it++)
            if ((*it)->collidesCircle(glm::vec2(playerX, playerY), radius))
                collision = true;
        if (collision)
            playerX -= change;
    }
    if (window->KeyHold(GLFW_KEY_A))
    {
        bool collision = false;
        playerX -= change;
        for (auto it = v.begin(); it != v.end() && !collision; it++)
            if ((*it)->collidesCircle(glm::vec2(playerX, playerY), radius))
                collision = true;
        if (collision)
            playerX += change;
    }

    // Update the logic space position to keep camera on player.
    l->y = playerY - l->height / 2;
    l->x = playerX - l->width / 2;
}

// Move player to a specified position
void Player::updatePos(float x, float y)
{
    playerX = x;
    playerY = y;
}

void Player::updatePos(glm::vec2 v)
{
    playerX = v.x;
    playerY = v.y;
}