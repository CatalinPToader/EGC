#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

Mesh *object2D::CreateSquare(
    const std::string &name,
    glm::vec3 centerShape,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 center = centerShape;

    std::vector<VertexFormat> vertices =
        {
            VertexFormat(center - glm::vec3(length / 2, length / 2, 0), color),
            VertexFormat(center - glm::vec3(-length / 2, length / 2, 0), color),
            VertexFormat(center + glm::vec3(length / 2, length / 2, 0), color),
            VertexFormat(center + glm::vec3(-length / 2, length / 2, 0), color)};

    Mesh *square = new Mesh(name);
    std::vector<unsigned int> indices = {0, 1, 2, 3};

    if (!fill)
    {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else
    {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

// Creates a square outline by using 8 points
Mesh *object2D::CreateSquareOutline(
    const std::string &name,
    glm::vec3 centerShape,
    float length,
    float thickness,
    glm::vec3 color)
{
    glm::vec3 center = centerShape;

    std::vector<VertexFormat> vertices =
        {
            VertexFormat(center + glm::vec3(length / 2, length / 2, 0), color),
            VertexFormat(center + glm::vec3(length / 2 - thickness, length / 2 - thickness, 0), color),

            VertexFormat(center + glm::vec3(-length / 2, length / 2, 0), color),
            VertexFormat(center + glm::vec3(-length / 2 + thickness, length / 2 - thickness, 0), color),

            VertexFormat(center + glm::vec3(-length / 2, -length / 2, 0), color),
            VertexFormat(center + glm::vec3(-length / 2 + thickness, -length / 2 + thickness, 0), color),

            VertexFormat(center + glm::vec3(length / 2, -length / 2, 0), color),
            VertexFormat(center + glm::vec3(length / 2 - thickness, -length / 2 + thickness, 0), color),
        };

    Mesh *square = new Mesh(name);
    std::vector<unsigned int> indices = {0, 1, 2, 3, 4, 5, 6, 7, 0, 1};

    square->SetDrawMode(GL_TRIANGLE_STRIP);

    square->InitFromData(vertices, indices);
    return square;
}

Mesh *object2D::CreateRectangle(
    const std::string &name,
    glm::vec3 centerShape,
    float sizeX,
    float sizeY,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 center = centerShape;

    std::vector<VertexFormat> vertices =
        {
            VertexFormat(center - glm::vec3(sizeX / 2, sizeY / 2, 0), color),
            VertexFormat(center - glm::vec3(-sizeX / 2, sizeY / 2, 0), color),
            VertexFormat(center + glm::vec3(sizeX / 2, sizeY / 2, 0), color),
            VertexFormat(center + glm::vec3(-sizeX / 2, sizeY / 2, 0), color)};

    Mesh *rect = new Mesh(name);
    std::vector<unsigned int> indices = {0, 1, 2, 3};

    if (!fill)
    {
        rect->SetDrawMode(GL_LINE_LOOP);
    }
    else
    {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    rect->InitFromData(vertices, indices);
    return rect;
}

Mesh *object2D::CreateCircle(
    const std::string &name,
    glm::vec3 centerCircle,
    float radius,
    glm::vec3 color,
    bool fill)
{
    int points = 36;

    glm::vec3 center = centerCircle;

    std::vector<VertexFormat> vertices;

    vertices.push_back(VertexFormat(center, color));

    for (int i = 0; i < points; i++)
    {
        float angle = RADIANS(360 * i / points);

        glm::vec3 pos = glm::vec3(radius * glm::cos(angle), radius * glm::sin(angle), 0) + center;

        VertexFormat form = VertexFormat(pos, color);

        vertices.push_back(form);
    }

    Mesh *circle = new Mesh(name);
    std::vector<unsigned int> indices;
    for (int i = 0; i <= points; i++)
        indices.push_back(i);
    indices.push_back(1);

    if (!fill)
    {
        circle->SetDrawMode(GL_LINE_LOOP);
        indices.erase(indices.begin());
    }
    else
    {
        circle->SetDrawMode(GL_TRIANGLE_FAN);
    }

    circle->InitFromData(vertices, indices);
    return circle;
}

// Creates a ring by using 72 points (36 for outside circle, 36 for inside circle)
Mesh *object2D::CreateRing(
    const std::string &name,
    glm::vec3 centerCircle,
    float radius,
    float thickness,
    glm::vec3 color)
{
    int points = 36;

    glm::vec3 center = centerCircle;

    std::vector<VertexFormat> vertices;

    for (int i = 0; i < points; i++)
    {
        float angle = RADIANS(360 * i / points);

        glm::vec3 pos = glm::vec3(radius * glm::cos(angle), radius * glm::sin(angle), 0) + center;

        VertexFormat form = VertexFormat(pos, color);

        vertices.push_back(form);

        pos = glm::vec3((radius - thickness) * glm::cos(angle), (radius - thickness) * glm::sin(angle), 0) + center;
        form = VertexFormat(pos, color);

        vertices.push_back(form);
    }

    Mesh *circle = new Mesh(name);
    std::vector<unsigned int> indices;
    for (int i = 0; i <= 2 * points - 1; i++)
        indices.push_back(i);
    indices.push_back(0);
    indices.push_back(1);
    circle->SetDrawMode(GL_TRIANGLE_STRIP);

    circle->InitFromData(vertices, indices);
    return circle;
}