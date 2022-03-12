#pragma once

#define LIFETIME 0.5
#define BULLET_SPEED 20.f

#include "utils/glm_utils.h"
#include "utils/math_utils.h"
#include "lab_m1/tema2/SphereCollidable.h"

// Represents a Bullet entity with a sphere collider

namespace homework
{
    class Bullet : public SphereCollidable
    {
    public:
        Bullet(glm::vec3 center, float rad, MeshInfo *_info, homework::Collidable::Type _t, glm::vec3 dir);
        Bullet();
        ~Bullet() override;

        void Update(float deltaTime);
        bool despawned() const;

        glm::vec3 getPosition();

        float radius{};
    private:
        glm::vec3 dir{};
        glm::vec3 position{};
        glm::vec3 size{};
        

        float time{};
    };
}