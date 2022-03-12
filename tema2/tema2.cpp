#include "lab_m1/tema2/tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <ctime>

#include "lab_m1/tema2/BoxCollidable.h"
#include "lab_m1/tema2/Enemy.h"

using namespace std;
using namespace m1;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Tema2::Tema2()
= default;

Tema2::~Tema2()
= default;

void Tema2::genMaze()
{
    int startX = rand() % (MAZE_MAX - MAZE_MIN) + MAZE_MIN;
    int startY = rand() % (MAZE_MAX - MAZE_MIN) + MAZE_MIN;

    std::vector<pos2D> directions;
    int i = -2;
    directions.emplace_back(i, 0);
    directions.emplace_back(-i, 0);
    directions.emplace_back(0, i);
    directions.emplace_back(0, -i);

    std::vector<pos2D> cells;

    cells.emplace_back(startX, startY);
    maze[startX][startY] = 2;

    startPos = glm::vec3(startX * CELL_SZ, 0, startY * CELL_SZ);

    int chance = 4;

    while (!cells.empty())
    {
        auto c = cells.back();
        cells.pop_back();

        int pick = rand() % 4;

        for (i = 0; i < 4; i++)
        {
            auto d = directions[pick];
            pick = (pick + 1) % 4;
            int newX = c.x + d.x;
            int newY = c.y + d.y;
            if (newX >= MAZE_MIN && newX < MAZE_MAX && newY >= MAZE_MIN && newY < MAZE_MAX && maze[newX][newY] == 0)
            {
                int val = (rand() % chance == 0) ? 3 : 1;
                maze[newX][newY] = val;
                maze[c.x + d.x / 2][c.y + d.y / 2] = 1;
                cells.emplace_back(newX, newY);
            }
        }
    }

    bool exit = false;

    for (i= 0; i < MAZE_MAX + 1 && !exit; i++)
    {
        if (i == 0)
        {
            for (int j = 0; j < MAZE_MAX + 1 && !exit; j++)
            {
                if (maze[i + 1][j] != 0 && (abs(i + 1 - startX) + abs(j - startY) > 4))
                {
                    maze[i][j] = 4;
                    exit = true;
                }
            }
        }
        else if (i == MAZE_MAX)
        {
            for (int j = 0; j < MAZE_MAX + 1 && !exit; j++)
            {
                if (maze[i - 1][j] != 0 && (abs(i - 1 - startX) + abs(j - startY) > 4))
                {
                    maze[i][j] = 4;
                    exit = true;
                }
            }
        }
        else
        {
            int j = 0;
            if (maze[i][j + 1] != 0 && (abs(i - startX) + abs(j + 1 - startY) > 4))
            {
                maze[i][j] = 4;
                exit = true;
            }
            j = MAZE_MAX;
            if (maze[i][j - 1] != 0 && !exit && (abs(i - startX) + abs(j - 1 - startY) > 4))
            {
                maze[i][j] = 4;
                exit = true;
            }
        }
    }
}

void Tema2::Init()
{
    window->DisablePointer();
    srand(time(nullptr));

    genMaze();

    renderCameraTarget = true;

    camera = new homework::Camera();
    camera->Set(startPos, glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    {
        Mesh *mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh *mesh = new Mesh("sphere");
        // mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "models"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a simple quad
    {
        vector<glm::vec3> vertices{
            glm::vec3(0.5f, 0.5f, 0.0f),   // top right
            glm::vec3(0.5f, -0.5f, 0.0f),  // bottom right
            glm::vec3(-0.5f, -0.5f, 0.0f), // bottom left
            glm::vec3(-0.5f, 0.5f, 0.0f),  // top left
        };

        vector<glm::vec3> normals{
            glm::vec3(0, 1, 1),
            glm::vec3(1, 0, 1),
            glm::vec3(1, 0, 0),
            glm::vec3(0, 1, 0)};

        vector<glm::vec2> textureCoords{
            glm::vec2(0.0f, 0.0f),
            glm::vec2(0.0f, 1.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec2(1.0f, 0.0f)

        };

        vector<unsigned int> indices =
            {
                0, 1, 3,
                1, 2, 3};

        Mesh *mesh = new Mesh("square");
        mesh->InitFromData(vertices, normals, textureCoords, indices);
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        auto *texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "ground.jpg").c_str(), GL_REPEAT);
        mapTextures["ground"] = texture;
    }

    {
        auto *texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "default.png").c_str(), GL_REPEAT);
        mapTextures["default"] = texture;
    }

    {
        auto *texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "crate.jpg").c_str(), GL_CLAMP_TO_EDGE);
        mapTextures["crate"] = texture;
    }

    {
        auto *texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "noise.png").c_str(), GL_CLAMP_TO_EDGE);
        noise = texture;
    }

    {
        auto *texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "textures", "shirt.png").c_str(), GL_CLAMP_TO_EDGE);
        mapTextures["shirt"] = texture;
    }

    {
        auto *texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "textures", "skin.png").c_str(), GL_CLAMP_TO_EDGE);
        mapTextures["skin"] = texture;
    }

    {
        auto *texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "textures", "pants.png").c_str(), GL_CLAMP_TO_EDGE);
        mapTextures["pants"] = texture;
    }

    {
        auto *texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "textures", "crosshair.png").c_str(), GL_CLAMP_TO_EDGE);
        mapTextures["crosshair"] = texture;
    }

    {
        auto *texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "textures", "bullet.png").c_str(), GL_CLAMP_TO_EDGE);
        mapTextures["bullet"] = texture;
    }

    {
        auto *texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "textures", "health_wireframe.png").c_str(), GL_CLAMP_TO_EDGE);
        mapTextures["health_wireframe"] = texture;
    }

    {
        auto *texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "textures", "health.png").c_str(), GL_CLAMP_TO_EDGE);
        mapTextures["health"] = texture;
    }

    {
        auto *texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "textures", "time_wireframe.png").c_str(), GL_CLAMP_TO_EDGE);
        mapTextures["time_wireframe"] = texture;
    }

    {
        auto *texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "textures", "hourglass.png").c_str(), GL_CLAMP_TO_EDGE);
        mapTextures["hourglass"] = texture;
    }

    {
        auto *shader = new Shader("TexShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "TextureVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "TextureFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        auto *shader = new Shader("ArmShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "ArmAnimationVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "TextureFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        auto *shader = new Shader("EnemyShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "EnemyAnimationVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "EnemyFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    fov = 60;
    zNear = 0.01f;
    zFar = 200.f;

    left = -8;
    right = -left;
    bottom = -4.5;
    top = -bottom;
    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, zNear, zFar);

    angleY = glm::pi<float>() / 2 + glm::pi<float>() * camera->forward.y / 2;
    lastShot = 0;

    auto *info1 = new homework::MeshInfo("box", "TexShader", "ground");
    auto *info2 = new homework::MeshInfo("box", "TexShader", "crate");
    auto *info3 = new homework::MeshInfo("box", "TexShader", "default");
    auto *enemy = new homework::MeshInfo("sphere", "EnemyShader", "ground");

    for (int i = 0; i < MAZE_MAX + 1; i++)
    {
        for (int j = 0; j < MAZE_MAX + 1; j++)
        {
            int floor = maze[i][j] != 0;
            glm::vec3 pos = glm::vec3(i * CELL_SZ, -floor * (CELL_SZ / 2), j * CELL_SZ);
            glm::vec3 scale = glm::vec3(CELL_SZ, 1, CELL_SZ);

            if (floor)
            {
                homework::BoxCollidable *b;
                if (maze[i][j] == 4)
                {
                    b = new homework::BoxCollidable(pos, scale, info3, homework::Collidable::WALL);
                }
                else
                    b = new homework::BoxCollidable(pos, scale, info1, homework::Collidable::WALL);
                walls.push_back((homework::Collidable *)b);

                if (maze[i][j] == 3)
                {
                    enemies.push_back(new homework::Enemy(pos + glm::vec3(0, 1.5, 0), 0.5, enemy, homework::Collidable::ENEMY));
                }
                else if (maze[i][j] == 4)
                {
                }
            }
            else
            {
                scale.y = CELL_SZ;
                auto *b = new homework::BoxCollidable(pos, scale, info2, homework::Collidable::WALL);
                walls.push_back((homework::Collidable *)b);
            }
        }
    }

    p = homework::Player(startPos, walls);

    startPos.y = 0.5 + p.getHeightFeetToCenter() - (float)CELL_SZ / 2;
    p.moveAbs(startPos);

    lost = false;

    logicSpace = homework::LogicSpace(0, 0, 4,4);
}

glm::mat3 Tema2::VisualizationTransf2DUnif(const homework::LogicSpace &logicSpace, const homework::ViewportSpace &viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}

void Tema2::SetViewportArea(const homework::ViewportSpace &viewSpace,
                            glm::vec3 colorColor)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetPosition(glm::vec3(0, 0, 50));
    GetSceneCamera()->SetRotation(glm::vec3(0, 0, 0));
    GetCameraInput()->SetActive(false);
    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}

void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::RenderPlayer()
{
    if (renderCameraTarget)
    {
        camera->TranslateForward(-camera->distanceToTarget);
        glm::mat4 modelMatrix = p.bodyMatrix();
        RenderMesh(meshes["box"], shaders["TexShader"], modelMatrix, mapTextures["shirt"]);
        modelMatrix = p.armMatrix(true);
        RenderMesh(meshes["box"], shaders["ArmShader"], modelMatrix, mapTextures["shirt"]);
        modelMatrix = p.armMatrix(false);
        RenderMesh(meshes["box"], shaders["ArmShader"], modelMatrix, mapTextures["shirt"]);
        modelMatrix = p.handMatrix(true);
        RenderMesh(meshes["box"], shaders["ArmShader"], modelMatrix, mapTextures["skin"]);
        modelMatrix = p.handMatrix(false);
        RenderMesh(meshes["box"], shaders["ArmShader"], modelMatrix, mapTextures["skin"]);
        modelMatrix = p.legMatrix(true);
        RenderMesh(meshes["box"], shaders["TexShader"], modelMatrix, mapTextures["pants"]);
        modelMatrix = p.legMatrix(false);
        RenderMesh(meshes["box"], shaders["TexShader"], modelMatrix, mapTextures["pants"]);
        modelMatrix = p.headMatrix();
        RenderMesh(meshes["box"], shaders["TexShader"], modelMatrix, mapTextures["skin"]);
    }
}

void Tema2::DrawHud(glm::mat3 visMatrix)
{
    glm::mat3 modelMatrix;
    if (!renderCameraTarget)
    {
        modelMatrix = visMatrix * homework::transform2D::Translate(logicSpace.width / 2, logicSpace.height / 2) * homework::transform2D::Scale(.1f, .1f);
        RenderMesh2D(meshes["square"], modelMatrix, shaders["TexShader"], mapTextures["crosshair"]);
    }

    float sizeX = 1.f;
    float sizeY = .2f;
    modelMatrix = visMatrix * homework::transform2D::Translate(logicSpace.x, logicSpace.height - sizeY);
    modelMatrix *= homework::transform2D::Translate(-sizeX / 2, 0);
    modelMatrix *= homework::transform2D::Scale(sizeX * p.getHealth() / 10, sizeY);
    modelMatrix *= homework::transform2D::Translate(sizeX / 2, 0);

    RenderMesh2D(meshes["square"], modelMatrix, shaders["TexShader"], mapTextures["health"]);

    modelMatrix = visMatrix * homework::transform2D::Translate(logicSpace.x, logicSpace.height - sizeY) * homework::transform2D::Scale(sizeX, sizeY);
    RenderMesh2D(meshes["square"], modelMatrix, shaders["TexShader"], mapTextures["health_wireframe"]);

    modelMatrix = visMatrix * homework::transform2D::Translate(logicSpace.width, logicSpace.height - sizeY);
    modelMatrix *= homework::transform2D::Translate(sizeX / 2, 0);
    modelMatrix *= homework::transform2D::Scale(sizeX * (glm::clamp(PLAYTIME + 3 - Engine::GetElapsedTime(), 0.0, (double)PLAYTIME) / PLAYTIME), sizeY);
    modelMatrix *= homework::transform2D::Translate(-sizeX / 2, 0);
    modelMatrix = glm::translate(glm::mat4(modelMatrix), glm::vec3(0, 0, 0.1));

    RenderMesh2D(meshes["square"], modelMatrix, shaders["TexShader"], mapTextures["bullet"]);

    modelMatrix = visMatrix * homework::transform2D::Translate(logicSpace.width, logicSpace.height - sizeY) * homework::transform2D::Scale(sizeX, sizeY);
    RenderMesh2D(meshes["square"], modelMatrix, shaders["TexShader"], mapTextures["time_wireframe"]);

    float sizeXIcon = .2f;
    float sizeYIcon = .2f;

    modelMatrix = visMatrix * homework::transform2D::Translate(logicSpace.width - sizeX / 2 - sizeXIcon, logicSpace.height - sizeY) * homework::transform2D::Scale(sizeXIcon, sizeYIcon);
    RenderMesh2D(meshes["square"], modelMatrix, shaders["TexShader"], mapTextures["hourglass"]);
}

void Tema2::Update(float deltaTimeSeconds)
{
    if (p.won)
    {
        cout << "\n\nYOU WON!!!\n\n";
        Tema2::Exit();
    }

    if (p.getHealth() <= 0 || Engine::GetElapsedTime() > PLAYTIME + 3)
        lost = true;

    glm::vec3 oZ = camera->forward;
    oZ = glm::normalize(glm::vec3(oZ.x, 0, oZ.z));

    float dot = glm::dot(oZ, glm::vec3(0, 0, 1.f));

    float angle = acos(dot);

    if (oZ.x < 0)
        angle = -angle;

    p.Update(angle, deltaTimeSeconds);
    camera->position = p.getCameraPosition();

    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw - the left half of the window
    homework::ViewportSpace viewSpace = homework::ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0.05));

    // Compute the 2D visualization matrix
    glm::mat3 visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

    if (!lost)
    {
        DrawHud(visMatrix);
        RenderPlayer();
    }

    UpdateEnemies(deltaTimeSeconds);

    UpdateBullets(deltaTimeSeconds);

    RenderWalls();
}

void Tema2::UpdateEnemies(float deltaTimeSeconds) {
    for (auto it = enemies.begin(); it != enemies.end();)
    {
        homework::Enemy *e = *it;

        e->Update(deltaTimeSeconds, p.getPosition());

        if (e->collidesBox(p.getCenterCollisionBox(), p.getSizeCollisionBox(), p.rotY) && e->alive)
        {
            p.damage(Engine::GetElapsedTime());
        }

        if (e->animationDone(Engine::GetElapsedTime()))
            enemies.erase(it);
        else
            it++;

        homework::MeshInfo *info = e->getInfo();
        RenderMesh(meshes[info->getName()], shaders[info->getShader()], e->renderMatrix(), e->getPosition(), e->getDeathTime(), e->getRatio());
    }
}

void Tema2::UpdateBullets(float deltaTimeSeconds) {
    for (auto it = bullets.begin(); it != bullets.end();)
    {
        homework::Bullet *b = *it;

        b->Update(deltaTimeSeconds);

        bool hit = false;

        for (auto wit = walls.begin(); wit != walls.end() && !hit; wit++)
        {
            auto w = *wit;

            if (w->collidesSphere(b->getPosition(), b->radius))
                hit = true;
        }

        for (auto eit = enemies.begin(); eit != enemies.end() && !hit; eit++)
        {
            auto e = *eit;

            if (e->collidesSphere(b->getPosition(), b->radius))
            {
                hit = true;
                e->damage(Engine::GetElapsedTime(), 1);
            }
        }

        if (b->despawned() || hit)
            bullets.erase(it);
        else
            it++;

        homework::MeshInfo *info = b->getInfo();
        RenderMesh(meshes[info->getName()], shaders[info->getShader()], b->renderMatrix(), mapTextures[info->getTexture()]);
    }
}

void Tema2::RenderWalls() {
    for (auto wall : walls)
    {
        homework::MeshInfo *info = wall->getInfo();

        RenderMesh(meshes[info->getName()], shaders[info->getShader()], wall->renderMatrix(), mapTextures[info->getTexture()]);
    }
}

void Tema2::FrameEnd()
{
    // DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
    window->DisablePointer();
}

void Tema2::RenderMesh2D(Mesh *mesh, const glm::mat3 &modelMatrix, Shader *shader, Texture2D *text)
{
    if (!mesh || !shader || !shader->program)
        return;

    glm::mat3 mm = modelMatrix;
    glm::mat4 model = glm::mat4(
        mm[0][0], mm[0][1], mm[0][2], 0.f,
        mm[1][0], mm[1][1], mm[1][2], 0.f,
        0.f, 0.f, mm[2][2], 0.f,
        mm[2][0], mm[2][1], 0.f, 1.f);

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(model));

    glUniform3fv(glGetUniformLocation(shader->program, "p_pos"), 1, glm::value_ptr(p.getPosition()));

    glUniform1i(glGetUniformLocation(shader->program, "hasTex"),
                text != nullptr);
    glUniform1i(glGetUniformLocation(shader->program, "hud"), true);

    glUniform1i(glGetUniformLocation(shader->program, "shouldAnimate"), p.isStill());
    glUniform1f(glGetUniformLocation(shader->program, "timeSinceStop"), p.timeSinceStopped());

    if (text)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, text->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    }
    // mesh->Render();

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, nullptr);
}

void Tema2::RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, Texture2D *text)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform3fv(glGetUniformLocation(shader->program, "p_pos"), 1, glm::value_ptr(p.getPosition()));

    glUniform1i(glGetUniformLocation(shader->program, "hasTex"),
                text != nullptr);
    glUniform1i(glGetUniformLocation(shader->program, "hud"), false);

    glUniform1i(glGetUniformLocation(shader->program, "shouldAnimate"), p.isStill());
    glUniform1f(glGetUniformLocation(shader->program, "timeSinceStop"), p.timeSinceStopped());

    if (text)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, text->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    }

    // mesh->Render();

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, nullptr);
}

void Tema2::RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, glm::vec3 pos, float deathTime, float hpRatio)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glUniform1f(glGetUniformLocation(shader->program, "animationTime"), Engine::GetElapsedTime() - deathTime);
    glUniform1f(glGetUniformLocation(shader->program, "mixVal"), hpRatio);
    glUniform1f(glGetUniformLocation(shader->program, "playerDist"), glm::length(pos - p.getPosition()));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, noise->GetTextureID());
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);

    // mesh->Render();

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, nullptr);
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    if (!lost)
    {
        float playerSpeed = 0.75f;

        glm::vec3 forward = glm::normalize(glm::vec3(camera->forward.x, 0, camera->forward.z));
        glm::vec3 right = glm::normalize(glm::vec3(camera->right.x, 0, camera->right.z));

        if (window->KeyHold(GLFW_KEY_W))
        {
            p.applyVelocity(forward * playerSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_A))
        {
            p.applyVelocity(right * -playerSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_S))
        {
            p.applyVelocity(forward * -playerSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_D))
        {
            p.applyVelocity(right * playerSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_SPACE) && p.isOnGround())
        {
            p.applyVelocity(glm::vec3(0, 0.2, 0));
        }
    }
}

void Tema2::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_LEFT_CONTROL)
    {
        renderCameraTarget = !renderCameraTarget;
    }
}

void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (!renderCameraTarget)
        {
            renderCameraTarget = false;
            angleY += -sensivityOX * deltaY;
            if (angleY > 0.1 && angleY < glm::pi<float>() - 0.1)
                camera->RotateFirstPerson_OX(sensivityOX * -deltaY);
            else
                angleY -= -sensivityOX * deltaY;

            camera->RotateFirstPerson_OY(sensivityOY * -deltaX);
        }
        else
        {
            angleY += -sensivityOX * deltaY;
            bool collision = false;

            if (angleY > 0.1 && angleY < glm::pi<float>() - 0.1)
            {

                camera->RotateThirdPerson_OX(-sensivityOX * deltaY);

                for (auto it = walls.begin(); it != walls.end() && !collision; it++)
                {
                    auto w = *it;
                    if (w->isInside(camera->position))
                        collision = true;
                }

                if (collision)
                {
                    camera->RotateThirdPerson_OX(sensivityOX * deltaY);
                    angleY -= -sensivityOX * deltaY;
                }
            }
            else
                angleY -= -sensivityOX * deltaY;

            camera->RotateThirdPerson_OY(-sensivityOY * deltaX);

            collision = false;
            for (auto it = walls.begin(); it != walls.end() && !collision; it++)
            {
                auto w = *it;
                if (w->isInside(camera->position))
                    collision = true;
            }

            if (collision)
                camera->RotateThirdPerson_OY(sensivityOY * deltaX);
        }
    }
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (!renderCameraTarget && Engine::GetElapsedTime() - lastShot > SHOOTING_COOLDOWN && !lost)
    {
        lastShot = Engine::GetElapsedTime();
        bullets.push_back(new homework::Bullet(p.getCameraPosition() + camera->forward * 0.5f, 0.05f, bullet, homework::Collidable::BULLET, camera->forward));
    }
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
