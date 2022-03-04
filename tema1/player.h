#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema1/Collidable.h"
#include "lab_m1/tema1/utils.h"

// Represents the Player
namespace m1
{
    class Player
    {
    public:
        Player();
        Player(WindowObject *w, float x, float y, float radius, glm::vec3 centerLow, float zCenterHigh);
        ~Player();

        float playerX, playerY;
        float radius;
        float speed;
        float angle;
        float maxHealth;
        float health;

        bool dead;

        glm::vec3 centerLow, centerHigh;

        WindowObject* window;

        void Init();

        void Update(float delta);

        void playerMove(float delta, int mods, std::vector<Collidable *> v, LogicSpace* l);

        glm::mat3 getBodyMatrix(glm::mat3 visMatrix);
        glm::mat3 getLeftArmMatrix(glm::mat3 visMatrix);
        glm::mat3 getRightArmMatrix(glm::mat3 visMatrix);

        void updatePos(float x, float y);
        void updatePos(glm::vec2 v);
    };
}