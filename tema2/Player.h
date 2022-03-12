#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"
#include "lab_m1/tema2/Collidable.h"
#include <vector>

#define DAMAGE_COOLDOWN 0.5

// Represents a Player entity with a box collider


namespace homework
{
    class Player
    {
    public:
        Player();
        Player(glm::vec3 position, std::vector<homework::Collidable *> walls);
        ~Player();
        void Update(float angle, float deltaTime);
        glm::mat4 centerMatrix();
        glm::mat4 bodyMatrix();
        glm::mat4 headMatrix();
        glm::mat4 handMatrix(bool left);
        glm::mat4 armMatrix(bool left);
        glm::mat4 legMatrix(bool left);
        void moveAbs(glm::vec3 pos);
        void applyVelocity(glm::vec3 newVelocity);
        glm::vec3 getPosition();
        glm::vec3 getCameraPosition();
        bool isOnGround() const;
        bool isStill() const;

        void damage(float damageTime);

        float getHeightFeetToCenter() const;
        glm::vec3 getCenterCollisionBox() const;
        glm::vec3 getSizeCollisionBox() const;

        float timeSinceStopped() const;
        float getHealth() const;

        bool won = false;
        float rotY{};
    private:
        float health = 10;
        float lastHitTime = 0;

        glm::vec3 velocity{};
        float gravity = -0.01;
        float drag = 0.1;

        float displacement = 0.01;

        float chestH = 0.4, chestW = 0.36;
        float armH = 0.4 - 0.175, armW = 0.175;
        float legH = 0.4, legW = 0.175;
        float handL = 0.175;
        float headL = chestW / 2;

        float playerHeight = chestH + legH + headL + 2 * displacement;
        float playerWidth = armW + displacement + chestW + displacement + armW;
        float playerDepth = 0.175;

        bool onGround{};
        float time{};
        float stopTime{};

        glm::vec3 position{};

        std::vector<homework::Collidable *> walls;
        std::vector<glm::vec3> getLocalAxis();
        std::vector<glm::vec3> getPoints();
        std::vector<glm::vec3> getPointsForAxis();


        bool checkCollision();

        void applyDrag();
    };
}