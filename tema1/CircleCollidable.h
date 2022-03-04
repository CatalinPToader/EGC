#pragma once

#include "lab_m1/tema1/Collidable.h"

// Represents a Collidable entity with a circle collider
namespace m1
{
    class CircleCollidable : public m1::Collidable
    {
    public:
        CircleCollidable();
        CircleCollidable(Type t, MeshInfo *info);
        ~CircleCollidable();

        glm::vec3 center;
        float radius;
        Type t;
        float angle;
        float scaleX, scaleY;
        MeshInfo *info;

        std::string getMeshName();
        MeshInfo *getMeshInfo();
        Type getType();

        void Init();

        void Update(float delta);

        bool collidesCircle(glm::vec2 center, float radius);
        bool collidesBox(glm::vec2 center, float sizeX, float sizeY, float angle);

        glm::mat3 renderMatrix(glm::mat3 visMatrix);

        void moveAbs(float x, float y);
        void moveRel(float x, float y);

        void rotateAbs(float angle);
        void rotateRel(float angle);

        void scaleAbs(float x, float y);
        void scaleRel(float x, float y);

        bool isInside(float x, float y);

        void alignPosition();
        void alignRotation();
    };
}
