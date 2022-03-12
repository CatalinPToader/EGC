#pragma once

#define DEATH_ANIM_DUR 5
#define MAX_HP 3

#include "utils/glm_utils.h"
#include "utils/math_utils.h"
#include "lab_m1/tema2/SphereCollidable.h"

// Represents an Enemy entity with a sphere collider

namespace homework
{
    class Enemy : public SphereCollidable
    {
    public:
        Enemy(glm::vec3 center, float rad, MeshInfo *_info, homework::Collidable::Type _t);
        Enemy();
        ~Enemy() override;

        void Update(float deltaTimeSeconds, glm::vec3 playerPos);

        glm::vec3 getPosFromMoveFunc(float time);

        void damage(float timeOfDamage, int amount);
        float getDeathTime() const;
        bool animationDone(float currentTime) const;

        glm::vec3 getPosition();

        float getRatio();
        bool alive{};
    private:
        glm::vec3 origin{};
        glm::vec3 lastPos{};
        glm::vec3 position{};
        glm::vec3 size{};
        float radius{};
        float deathTime{};

        float time{};

        // 0 - Patrol
        // 1 - Pursuit
        // 2 - Return
        int state{};

        float ratio = 1;
        int hitsLeft = 3;

        float enemySpeed = 5.f;

        enum states {patrol = 0, follow = 1, back = 2};
    };
}