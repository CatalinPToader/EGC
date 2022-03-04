#pragma once

#include "lab_m1/tema1/Collidable.h"


// Represents a Collidable entity with a box collider
namespace m1
{
    class BoxCollidable : public m1::Collidable
    {
    public:
        BoxCollidable();
        BoxCollidable(Type t, MeshInfo *info);
        ~BoxCollidable();

        glm::vec3 center;
        float sizeX, sizeY;
        Type t;
        float angle;
        float scaleX, scaleY;
        MeshInfo *info;
        float initialScaleX, initialScaleY;

        std::string getMeshName();
        MeshInfo *getMeshInfo();
        Type getType();

        void Init();

        void Update(float delta);

        float diagonal();

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
