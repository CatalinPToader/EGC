#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"
#include "lab_m1/tema2/utils.h"

namespace homework
{
    class Collidable
    {
    public:
        enum Type
        {
            ENEMY,
            WALL,
            BULLET
        };

        Collidable()= default;
        virtual ~Collidable()= default;
        virtual bool isInside(glm::vec3 point) = 0;
        virtual bool collidesSphere(glm::vec3 center, float radius) = 0;
        virtual bool collidesBox(glm::vec3 center, glm::vec3 size, float angleY) = 0;
        virtual bool collidesBox(std::vector<glm::vec3> axisOther, std::vector<glm::vec3> pointsOther) = 0;
        virtual glm::mat4 renderMatrix() = 0;
        virtual void moveAbs(glm::vec3 position) = 0;

        Type t{};
        virtual Type getType() = 0;

        MeshInfo *info{};
        virtual MeshInfo *getInfo() = 0;
    };
}