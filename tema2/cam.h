#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"

namespace homework
{
    class Camera
    {
    public:
        Camera()
        {
            position = glm::vec3(0, 2, 5);
            forward = glm::vec3(0, 0, -1);
            up = glm::vec3(0, 1, 0);
            right = glm::vec3(1, 0, 0);
            distanceToTarget = 1.5;
        }

        Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
        {
            Set(position, center, up);
        }

        ~Camera()
        = default;

        // Update camera
        void Set(const glm::vec3 &setPos, const glm::vec3 &center, const glm::vec3 &setUp)
        {
            this->position = setPos;
            forward = glm::normalize(center - setPos);
            right = glm::cross(forward, setUp);
            this->up = glm::cross(right, forward);
        }

        void MoveForward(float distance)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
            position += dir * distance;
        }

        void TranslateForward(float distance)
        {
            position += forward * distance;
        }

        void TranslateUpward(float distance)
        {
            position += up * distance;
        }

        void TranslateRight(float distance)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(right.x, 0, right.z));
            position += dir * distance;
        }

        void RotateFirstPerson_OX(float angle)
        {

            glm::mat4 rotation_mat = glm::rotate(glm::mat4(1), angle, right);

            glm::vec4 new_fwd = rotation_mat * glm::vec4(forward, 1);
            forward = glm::normalize(glm::vec3(new_fwd));

            up = glm::normalize(glm::cross(right, forward));
        }

        void RotateFirstPerson_OY(float angle)
        {
            glm::mat4 rotation_mat = glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0));

            glm::vec4 new_fwd = rotation_mat * glm::vec4(forward, 1);
            forward = glm::normalize(glm::vec3(new_fwd));

            glm::vec4 new_right = rotation_mat * glm::vec4(right, 1);
            right = glm::normalize(glm::vec3(new_right));

            up = glm::normalize(glm::cross(right, forward));
        }

        void RotateFirstPerson_OZ(float angle)
        {

            glm::mat4 rotation_mat = glm::rotate(glm::mat4(1), angle, forward);

            glm::vec4 new_right = glm::normalize(rotation_mat * glm::vec4(right, 1));
            right = glm::vec3(new_right);

            up = glm::normalize(glm::cross(right, forward));
        }

        void RotateThirdPerson_OX(float angle)
        {
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OX(angle);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OY(float angle)
        {
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OY(angle);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OZ(float angle)
        {
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OZ(angle);
            TranslateForward(-distanceToTarget);
        }

        glm::mat4 GetViewMatrix() const
        {
            // Returns the view matrix
            return glm::lookAt(position, position + forward, up);
        }

    public:
        float distanceToTarget{};
        glm::vec3 position{};
        glm::vec3 forward{};
        glm::vec3 right{};
        glm::vec3 up{};
    };
} // namespace implemented
