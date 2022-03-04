#pragma once

#include "lab_m1/tema1/Collidable.h"
#include "lab_m1/tema1/BoxCollidable.h"
#include "lab_m1/tema1/player.h"

// Represents an Enemy entity which is a Box Collider
namespace m1
{
    class Enemy : public m1::BoxCollidable
    {
    private:
        typedef BoxCollidable super;

    public:
        Enemy();
        Enemy(Collidable::Type t, MeshInfo *info, float health, float speed, float size); 
        ~Enemy();

        float health;
        float speed;
        float size;
        float maxHealth;

        float defaultSize = 100;

        void Init();

        void Update(float delta, Player* target, std::vector<Collidable *> walls);

        glm::mat3 getBodyMatrix(glm::mat3 visMatrix);
        glm::mat3 getLeftArmMatrix(glm::mat3 visMatrix);
        glm::mat3 getRightArmMatrix(glm::mat3 visMatrix);
    };
}
