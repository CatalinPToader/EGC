#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"
#include "lab_m1/tema2/Collidable.h"

namespace homework
{
    class BoxCollidable : Collidable
    {
    public:
        BoxCollidable();
        BoxCollidable(glm::vec3 center, glm::vec3 scale, MeshInfo *_info, Type _t);
        ~BoxCollidable() override;
        bool collidesSphere(glm::vec3 center, float radius) override;
        bool collidesBox(glm::vec3 center, glm::vec3 sizeOther, float angleY) override;
        bool collidesBox(std::vector<glm::vec3> axisOther, std::vector<glm::vec3> pointsOther) override;
        glm::mat4 renderMatrix() override;
        void moveAbs(glm::vec3 newPosition) override;

        bool isInside(glm::vec3 point) override;

        Type t;
        Type getType() override;

        MeshInfo *info{};
        MeshInfo *getInfo() override;

    private:
        glm::vec3 position{};
        glm::vec3 size{};

        std::vector<glm::vec3> getPointsFromCenter();
    };
}