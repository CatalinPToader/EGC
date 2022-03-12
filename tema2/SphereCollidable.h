#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"
#include "lab_m1/tema2/Collidable.h"

namespace homework
{
    class SphereCollidable : Collidable
    {
    public:
        SphereCollidable(glm::vec3 center, float rad, MeshInfo *_info, Type _t);
        SphereCollidable();
        ~SphereCollidable() override;
        bool isInside(glm::vec3 point) override;
        bool collidesSphere(glm::vec3 center, float otherRadius) override;
        bool collidesBox(glm::vec3 center, glm::vec3 otherSize, float angleY) override;
        bool collidesBox(std::vector<glm::vec3> axisOther, std::vector<glm::vec3> pointsOther) override;
        glm::mat4 renderMatrix() override;
        void moveAbs(glm::vec3 newPosition) override;

        Type t;
        Type getType() override;

        MeshInfo *info{};
        MeshInfo *getInfo() override;
        glm::vec3 position{};
        glm::vec3 size{};
        float radius{};
    };
}