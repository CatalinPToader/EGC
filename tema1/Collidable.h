#pragma once

#include "components/simple_scene.h"
#include "utils.h"

// Represents a collidable
// This is a class with no implementation for inheritance purposes

namespace m1
{
    class Collidable
    {
    public:
        enum Type
        {
            ENEMY,
            WALL,
            OBSTACLE,
            BULLET
        };

        Collidable(){};
        ~Collidable(){};
        virtual bool collidesCircle(glm::vec2 center, float radius) = 0;
        virtual bool collidesBox(glm::vec2 center, float sizeX, float sizeY, float angle) = 0;
        virtual glm::mat3 renderMatrix(glm::mat3 visMatrix) = 0;
        virtual void moveAbs(float x, float y) = 0;
        virtual void moveRel(float x, float y) = 0;
        virtual void rotateAbs(float angle) = 0;
        virtual void rotateRel(float angle) = 0;

        virtual void scaleAbs(float x, float y) = 0;
        virtual void scaleRel(float x, float y) = 0;
        Type t;
        MeshInfo *info;

        virtual std::string getMeshName() = 0;
        virtual MeshInfo *getMeshInfo() = 0;
        virtual Type getType() = 0;
        virtual bool isInside(float x, float y) = 0;
        virtual void alignPosition() = 0;
        virtual void alignRotation() = 0;
    };
}
