#pragma once

#include "lab_m1/tema1/CircleCollidable.h"

// Represents a Projectile entity which is a Circle Collider
namespace m1
{
    class Projectile : public m1::CircleCollidable
    {
    public:
        Projectile();
        Projectile(glm::vec3 center, float radius, Type t, std::string mesh, glm::vec2 dir, float speed, float lifespan);
        ~Projectile();

        glm::vec3 center;
        float radius;
        Type t;
        std::string mesh;
        glm::vec2 dir;
        float speed;
        float lifespan;

        std::string getMeshName();

        void Init();

        void Update(float delta);

        glm::mat3 renderMatrix(glm::mat3 visMatrix);

        void moveAbs(float x, float y);
        void moveRel(float x, float y);
    };
}
