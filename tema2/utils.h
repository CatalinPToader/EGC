#pragma once

#include "components/simple_scene.h"
#include <string>
#include <utility>
#include <vector>

namespace homework
{
    struct MeshInfo
    {
        std::string name;
        std::string shaderName;
        std::string textureName;

        MeshInfo(std::string _name, std::string _shader, std::string _texture) : name(std::move(_name)), shaderName(std::move(_shader)), textureName(std::move(_texture)) {}
        std::string getName() const { return name; }
        std::string getShader() const { return shaderName; }
        std::string getTexture() const { return textureName; }
    };
    struct ViewportSpace
    {
        ViewportSpace() : x(0), y(0), width(1), height(1) {}
        ViewportSpace(int x, int y, int width, int height)
            : x(x), y(y), width(width), height(height) {}
        int x;
        int y;
        int width;
        int height;
    };

    struct LogicSpace
    {
        LogicSpace() : x(0), y(0), width(1), height(1) {}
        LogicSpace(float x, float y, float width, float height)
            : x(x), y(y), width(width), height(height) {}
        float x;
        float y;
        float width;
        float height;
    };

    bool inRange(float val, float min, float max);

    namespace transform2D
    {
        // Translate matrix
        inline glm::mat3 Translate(float translateX, float translateY)
        {
            return glm::transpose(glm::mat3(1, 0, translateX,
                                            0, 1, translateY,
                                            0, 0, 1));
        }

        // Scale matrix
        inline glm::mat3 Scale(float scaleX, float scaleY)
        {
            return {scaleX, 0, 0,
                             0, scaleY, 0,
                             0, 0, 1};
        }

        // Rotate matrix
        inline glm::mat3 Rotate(float radians)
        {
            return glm::transpose(
                glm::mat3(glm::cos(radians), -glm::sin(radians), 0,
                          glm::sin(radians), glm::cos(radians), 0,
                          0, 0, 1));
        }
    }
}

glm::vec3 rotateToLocal(glm::vec3 point, glm::vec3 centerLocal, glm::vec3 axis, float angle);

std::vector<glm::vec3> getAxis(std::vector<glm::vec3> points);

glm::vec2 returnMinMaxOnAxis(glm::vec3 axis, const std::vector<glm::vec3>& points);
