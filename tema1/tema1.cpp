#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/tema1/object2D.h"
#include "lab_m1/tema1/BoxCollidable.h"

#define EDIT_MODE false
#define MAP_PATH_NAME "map"

using namespace std;
using namespace m1;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

// Only for fallback in case map loading failed.
void Tema1::createWalls()
{
    float width = 150;
    float height = 3000;

    std::string meshName = "box1";
    glm::vec3 centerBox = glm::vec3(0, 0, 0);
    glm::vec3 color = glm::vec3(0.8f, 0.8f, 0.8f);

    Mesh *box1 = object2D::CreateRectangle(meshName, centerBox, width, height, color, true);
    AddMeshToList(box1);

    MeshInfo *m = new MeshInfo();
    m->meshName = meshName;
    m->center = centerBox;
    m->color = color;
    m->width = width;
    m->height = height;
    m->fill = true;
    m->t = MeshType::RECTANGLE;

    wall = m;

    BoxCollidable *b = new BoxCollidable(Collidable::OBSTACLE, m);
    b->moveAbs(-width / 2, height / 2);

    walls.push_back(b);

    b = new BoxCollidable(Collidable::OBSTACLE, m);
    b->moveAbs(height * 2 / 3 - width / 2, height / 2);

    walls.push_back(b);

    b = new BoxCollidable(Collidable::OBSTACLE, m);
    b->moveAbs(height / 3 - width / 2, -width / 2);
    b->rotateAbs(90);
    b->scaleAbs(1, 2.15f / 3);

    walls.push_back(b);

    b = new BoxCollidable(Collidable::OBSTACLE, m);
    b->moveAbs(height / 3 - width / 2, height + width / 2);
    b->rotateAbs(90);
    b->scaleAbs(1, 2.15f / 3);

    walls.push_back(b);

    b = new BoxCollidable(Collidable::OBSTACLE, m);
    b->moveAbs(1000, 200);
    b->rotateAbs(90);
    b->scaleAbs(1.f, .5f / 3);

    walls.push_back(b);

    b = new BoxCollidable(Collidable::OBSTACLE, m);
    b->moveAbs(700, 120);
    b->rotateAbs(-45);
    b->scaleAbs(1.f, .5f / 3);

    walls.push_back(b);
}

// Initialize game
void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, 1 * (float)resolution.x, 0, 1 * (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    srand(1);

    score = 0;

    logicSpace.x = 0;
    logicSpace.y = 0;
    logicSpace.width = 900;
    logicSpace.height = 900;

    glm::vec3 centerP = glm::vec3(0, 0, -0.1);

    float px = 1000, py = 1500, pr = 50;
    p = new Player(window, px, py, pr, centerP, 0);

    glm::vec3 centerHigh = p->centerHigh;
    float squareSide = 100;
    float width = 20 * squareSide;

    if (!readFromFile())
        createWalls();

    // Create meshes
    Mesh *projectile = object2D::CreateCircle("projectile", centerHigh, 10, glm::vec3(0, 0, 0), true);
    Mesh *floor = object2D::CreateSquare("floor", centerP, squareSide, glm::vec3(0.33f, 0.33f, 0.33f), true);

    Mesh *bodyEnemy = object2D::CreateSquare("bodyEnemy", centerHigh, squareSide, glm::vec3(0.99f, 0.10f, 0), true);
    Mesh *outlineBodyEnemy = object2D::CreateSquareOutline("outlineBodyEnemy", centerHigh, squareSide, squareSide / 20, glm::vec3(0, 0, 0));
    Mesh *armEnemy = object2D::CreateSquare("armEnemy", centerHigh, squareSide, glm::vec3(0.99f, 0.10f, 0), true);
    Mesh *outlineArmEnemy = object2D::CreateSquareOutline("outlineArmEnemy", centerHigh, squareSide, squareSide / 10, glm::vec3(0, 0, 0));

    // Create MeshInfo for enemy
    enemy = new MeshInfo();

    enemy->center = centerP;
    enemy->color = glm::vec3(0.99f, 0.20f, 0);
    enemy->fill = true;
    enemy->length = squareSide;
    enemy->t = MeshType::SQUARE;
    enemy->meshName = "bodyEnemy";

    Mesh *healthArea = object2D::CreateRectangle("healthArea", glm::vec3(0, 0, 1), 300, 50, glm::vec3(1, 0, 0));
    Mesh *healthBar = object2D::CreateRectangle("healthBar", glm::vec3(0, 0, 1), 300, 50, glm::vec3(1, 0, 0), true);

    Mesh *bodyPlayer = object2D::CreateCircle("bodyPlayer", centerP, pr, glm::vec3(0.996f, 0.470f, 0.031f), true);
    Mesh *armPlayer = object2D::CreateCircle("armPlayer", centerHigh, pr, glm::vec3(0.996f, 0.470f, 0.031f), true);
    Mesh *bodyRingPlayer = object2D::CreateRing("bodyRingPlayer", centerP, pr, pr / 10, glm::vec3(0, 0, 0));
    Mesh *armRingPlayer = object2D::CreateRing("armRingPlayer", centerHigh, pr, pr / 5, glm::vec3(0, 0, 0));

    Mesh *selected = object2D::CreateRectangle("selected", glm::vec3(0, 0, 0), 150, 3000, glm::vec3(1, 0, 0), false);

    // Add all meshes to the mesh list
    AddMeshToList(selected);

    AddMeshToList(healthArea);
    AddMeshToList(healthBar);

    AddMeshToList(bodyEnemy);
    AddMeshToList(armEnemy);
    AddMeshToList(outlineBodyEnemy);
    AddMeshToList(outlineArmEnemy);

    AddMeshToList(projectile);
    AddMeshToList(floor);
    AddMeshToList(bodyPlayer);
    AddMeshToList(armPlayer);
    AddMeshToList(bodyRingPlayer);
    AddMeshToList(armRingPlayer);

    // Editor values
    lastWall = nullptr;
    showSelection = false;
    scaleOnX = true;
}

// Transform logic space into view space
glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace &logicSpace, const ViewportSpace &viewSpace)
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

void Tema1::SetViewportArea(const ViewportSpace &viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}

void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

// Renders an enemy
void Tema1::enemyRender(Enemy *e, glm::mat3 visMatrix)
{
    modelMatrix = e->getLeftArmMatrix(visMatrix);
    RenderMesh2D(meshes["outlineArmEnemy"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["armEnemy"], shaders["VertexColor"], modelMatrix);

    modelMatrix = e->getRightArmMatrix(visMatrix);
    RenderMesh2D(meshes["outlineArmEnemy"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["armEnemy"], shaders["VertexColor"], modelMatrix);

    modelMatrix = e->getBodyMatrix(visMatrix);
    RenderMesh2D(meshes["outlineBodyEnemy"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["bodyEnemy"], shaders["VertexColor"], modelMatrix);
}

// Renders the player
void Tema1::playerRender(glm::mat3 visMatrix)
{
    modelMatrix = p->getBodyMatrix(visMatrix);
    RenderMesh2D(meshes["bodyRingPlayer"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["bodyPlayer"], shaders["VertexColor"], modelMatrix);

    modelMatrix = p->getLeftArmMatrix(visMatrix);
    RenderMesh2D(meshes["armRingPlayer"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["armPlayer"], shaders["VertexColor"], modelMatrix);

    modelMatrix = p->getRightArmMatrix(visMatrix);
    RenderMesh2D(meshes["armRingPlayer"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["armPlayer"], shaders["VertexColor"], modelMatrix);
}

// Draws the scene
void Tema1::DrawScene(glm::mat3 visMatrix, float delta)
{

    // Non-edit mode requires player, enemies, bullets and GUI be rendered
    if (!EDIT_MODE)
    {
        playerRender(visMatrix);
        for (auto e : enemies)
            enemyRender(e, visMatrix);
        DrawBullets(visMatrix, delta);
        DrawHealthbar(visMatrix, delta);
    }
    else
    {
        // If we have to show the selected wall, render the selection mesh
        if (showSelection && lastWall != nullptr)
        {
            modelMatrix = lastWall->renderMatrix(visMatrix);
            RenderMesh2D(meshes["selected"], shaders["VertexColor"], modelMatrix);
        }
    }

    // Walls get rendered in both modes
    for (Collidable *b : walls)
    {
        modelMatrix = b->renderMatrix(visMatrix);
        RenderMesh2D(meshes[b->getMeshName()], shaders["VertexColor"], modelMatrix);
    }

    // The floor also gets rendered in both modes
    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(1000, 1500);
    modelMatrix *= transform2D::Scale(20, 30);
    RenderMesh2D(meshes["floor"], shaders["VertexColor"], modelMatrix);
}

// Draws and updates all bullets (Also checks for collisions and bullets expiring)
// Should be broken down further
void Tema1::DrawBullets(glm::mat3 visMatrix, float delta)
{
    for (auto it = projectiles.begin(); it != projectiles.end();)
    {
        auto p = *it;
        p->Update(delta);
        modelMatrix = p->renderMatrix(visMatrix);
        RenderMesh2D(meshes[p->getMeshName()], shaders["VertexColor"], modelMatrix);

        bool collision = false;

        // Check if we hit any walls
        for (auto it = walls.begin(); it != walls.end() && !collision; it++)
        {
            Collidable *b = *it;
            if (b->collidesCircle(glm::vec2(p->center), p->radius))
                collision = true;
        }

        // If not, check for enemy collisions
        if (!collision)
        {
            for (auto it = enemies.begin(); it != enemies.end() && !collision; it++)
            {
                Enemy *e = *it;
                if (e->collidesCircle(glm::vec2(p->center), p->radius))
                {
                    collision = true;
                    e->health -= projectileDamage;

                    // Check if enemy should die from the bullet
                    if (e->health < 0)
                    {
                        score++;
                        enemies.erase(it);
                    }
                }
            }
        }

        // If bullet lifespan is up or collided, remove it
        if (p->lifespan < 0 || collision)
            projectiles.erase(it);
        else
            it++;
    }
}

// Draws the HealthBar UI
void Tema1::DrawHealthbar(glm::mat3 visMatrix, float delta)
{

    // This entire code section is my attempt at doing an inverse transformation from viewspace to logic space
    float x = logicSpace.x + (viewSpace.width - 350);
    float y = logicSpace.y + (viewSpace.height - 50);

    glm::ivec2 res = window->GetResolution();

    float sx, sy, smin, tx, ty;

    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;

    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    float xLogic = (res.x * 13 / 16 - tx) / smin;
    float yLogic = (res.y * 15 / 16 - ty) / smin;
    // It was a successful attempt as the healthbar stays in the top right, regardless of window size

    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(xLogic, yLogic);
    modelMatrix *= transform2D::Scale(1.5, 1.5);
    modelMatrix *= transform2D::Scale(smin, smin);
    RenderMesh2D(meshes["healthArea"], shaders["VertexColor"], modelMatrix);
    modelMatrix *= transform2D::Translate(150, 0);
    if (!p->dead)
        modelMatrix *= transform2D::Scale(p->health / p->maxHealth, 1);
    else
        modelMatrix *= transform2D::Scale(0, 1);
    modelMatrix *= transform2D::Translate(-150, 0);
    RenderMesh2D(meshes["healthBar"], shaders["VertexColor"], modelMatrix);
}

// Handles enemy spawning by spawning them in waves
void Tema1::EnemySpawner(float deltaTimeSeconds)
{
    timeToWave -= deltaTimeSeconds;

    // Only allows maxEnemies on the map at all times
    // Spawns the missing enemies in waves
    if (timeToWave < 0 && enemies.size() < maxEnemies)
    {
        int sz = enemies.size();

        for (int i = sz; i < maxEnemies; i++)
        {
            // Randomization regarding the enemy stats for more variation
            float health = rand() % (int)(maxEnemyHealth - minEnemyHealth) + minEnemyHealth;
            float speed = rand() % (int)(maxEnemySpeed - minEnemySpeed) + minEnemySpeed;
            float size = health / ((minEnemyHealth + maxEnemyHealth) / 2);
            speed *= 1 / size;

            Enemy *e = new Enemy(Collidable::Type::ENEMY, enemy, health, speed, size);

            float x = rand() % 1650 + 100;
            float y = rand() % 2700 + 100;

            while (abs(x - p->playerX) < 100)
                x = rand() % 1650 + 100;
            while (abs(y - p->playerY) < 100)
                y = rand() % 2700 + 100;

            e->moveAbs(x, y);

            enemies.push_back(e);
        }

        timeToWave = maxTimeToWave;
    }
}

// Updates all enemies and checks for collision with player
void Tema1::EnemyUpdater(float deltaTimeSeconds)
{
    for (auto it = enemies.begin(); it != enemies.end();)
    {
        Enemy *e = *it;
        e->Update(deltaTimeSeconds, p, walls);

        // Enemies that touch the player are killed
        if (e->collidesCircle(glm::vec2(p->playerX, p->playerY), p->radius))
        {
            p->health -= 41;
            enemies.erase(it);
        }
        else
            it++;
    }
}

// Framework Update function called every frame
void Tema1::Update(float deltaTimeSeconds)
{
    cooldown -= deltaTimeSeconds;
    glm::ivec2 resolution = window->GetResolution();

    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0.5f), true);

    // Compute uniform 2D visualization matrix
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
    DrawScene(visMatrix, deltaTimeSeconds);

    EnemyUpdater(deltaTimeSeconds);
    EnemySpawner(deltaTimeSeconds);

    if (p->dead && score >= 0)
    {
        cout << "Score: " << score << endl
             << "Restart app!" << endl
             << flush;
        score = -1;
    }
}

void Tema1::FrameEnd()
{
}


// Editor function for moving, scaling and rotating objects
void Tema1::objectMove(float deltaTime, int mods)
{
    float adj = 1.0;

    if (mods & GLFW_MOD_SHIFT)
        adj *= 2;
    if (mods & GLFW_MOD_CONTROL)
        adj *= .5f;

    if (window->KeyHold(GLFW_KEY_W))
    {
        lastWall->moveRel(0, moveSpeed * deltaTime * adj);
    }
    if (window->KeyHold(GLFW_KEY_S))
    {
        lastWall->moveRel(0, -moveSpeed * deltaTime * adj);
    }
    if (window->KeyHold(GLFW_KEY_A))
    {
        lastWall->moveRel(-moveSpeed * deltaTime * adj, 0);
    }
    if (window->KeyHold(GLFW_KEY_D))
    {
        lastWall->moveRel(moveSpeed * deltaTime * adj, 0);
    }
    if (window->KeyHold(GLFW_KEY_EQUAL))
    {
        if (scaleOnX)
            lastWall->scaleRel(scaleSpeed * deltaTime * adj, 0);
        else
            lastWall->scaleRel(0, scaleSpeed * deltaTime * adj);
    }
    if (window->KeyHold(GLFW_KEY_MINUS))
    {
        if (scaleOnX)
            lastWall->scaleRel(-scaleSpeed * deltaTime * adj, 0);
        else
            lastWall->scaleRel(0, -scaleSpeed * deltaTime * adj);
    }
    if (window->KeyHold(GLFW_KEY_LEFT_BRACKET))
    {
        lastWall->rotateRel(rotation * deltaTime * adj);
    }
    if (window->KeyHold(GLFW_KEY_RIGHT_BRACKET))
    {
        lastWall->rotateRel(-rotation * deltaTime * adj);
    }
}

// Editor function for moving camera when an object is not selected
void Tema1::cameraMove(float deltaTime, int mods)
{
    float adj = 1.0;

    if (mods & GLFW_MOD_SHIFT)
        adj *= 2;

    if (window->KeyHold(GLFW_KEY_W))
    {
        logicSpace.y += cameraSpeed * deltaTime * adj;
    }
    if (window->KeyHold(GLFW_KEY_S))
    {
        logicSpace.y -= cameraSpeed * deltaTime * adj;
    }
    if (window->KeyHold(GLFW_KEY_D))
    {
        logicSpace.x += cameraSpeed * deltaTime * adj;
    }
    if (window->KeyHold(GLFW_KEY_A))
    {
        logicSpace.x -= cameraSpeed * deltaTime * adj;
    }
    if (window->KeyHold(GLFW_KEY_MINUS))
    {
        logicSpace.height += cameraSpeed * deltaTime * adj;
        logicSpace.width += cameraSpeed * deltaTime * adj;
    }
    if (window->KeyHold(GLFW_KEY_EQUAL))
    {
        logicSpace.height -= cameraSpeed * deltaTime * adj;
        logicSpace.width -= cameraSpeed * deltaTime * adj;
    }
}

// Framework function called on continous key presses
void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    if (!EDIT_MODE)
        p->playerMove(deltaTime, mods, walls, &logicSpace);
    else
    {
        if (!showSelection)
            cameraMove(deltaTime, mods);
        else if (lastWall != nullptr)
            objectMove(deltaTime, mods);
    }
}

// Framework function called on a keypress regardless of holding the key
void Tema1::OnKeyPress(int key, int mods)
{
    if (EDIT_MODE)
    {
        if (key == GLFW_KEY_INSERT)
            writeToFile();
        if (key == GLFW_KEY_HOME)
            readFromFile();
        if (key == GLFW_KEY_TAB)
            if (lastWall != nullptr)
                showSelection = !showSelection;

        if (showSelection)
        {
            if (key == GLFW_KEY_DELETE)
                if (lastWall != nullptr)
                {
                    auto it = std::find(walls.begin(), walls.end(), lastWall);
                    if (it != walls.end())
                    {
                        walls.erase(it);
                        lastWall = nullptr;
                    }
                }
            if (key == GLFW_KEY_ENTER)
                if (lastWall != nullptr)
                {
                    lastWall->alignPosition();
                    lastWall->alignRotation();
                }

            if (key == GLFW_KEY_BACKSLASH)
                scaleOnX = !scaleOnX;
        }
    }
}

// Framework function called on mouse movement
void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    glm::ivec2 resolution = window->GetResolution();

    // Updates player to face mouse
    float dX = mouseX - resolution.x / 2;
    float dY = (resolution.y - mouseY) - resolution.y / 2;
    p->angle = DEGREES(glm::atan2(dY, dX));
}

// Framework function called on mouse buttons presses
void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Handles shooting for playing mode
    if (cooldown < 0 && !EDIT_MODE && !p->dead)
    {
        glm::ivec2 resolution = window->GetResolution();

        // Since player is always facing mouse, get angle of mouse relative to center of screen for shooting direction
        float dX = mouseX - resolution.x / 2;
        float dY = (resolution.y - mouseY) - resolution.y / 2;

        float angle = glm::atan2(dY, dX);

        glm::vec2 dir = glm::vec2(glm::cos(angle), glm::sin(angle));

        glm::vec3 res = glm::vec3(0, 0, 0);
        res.x = glm::cos(angle) * (projectileSpawnDist + p->radius) + p->playerX;
        res.y = glm::sin(angle) * (projectileSpawnDist + p->radius) + p->playerY;

        Projectile *p = new Projectile(res, 10, Collidable::BULLET, "projectile", dir, projectileSpeed, projectileLifespan);
        projectiles.push_back(p);

        // Cooldown so that player can't spam bullets
        cooldown = maxCooldown;
    }
    else if (EDIT_MODE)
    {

        // Code snippet to inverse viewspace positions to logic space positions
        float sx, sy, smin, tx, ty;

        sx = viewSpace.width / logicSpace.width;
        sy = viewSpace.height / logicSpace.height;
        if (sx < sy)
            smin = sx;
        else
            smin = sy;

        tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
        ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

        float xLogic = (mouseX - tx) / smin;
        float yLogic = ((viewSpace.height - mouseY) - ty) / smin;

        /*
        cout << "Mouse: " << xLogic << ' ' << yLogic << endl
             << flush;
        */
        show = true;

        // Check if click was on a wall
        bool found = false;
        for (auto it = walls.begin(); it != walls.end() && !found; it++)
        {
            auto w = *it;
            if (w->isInside(xLogic, yLogic) == true)
            {
                lastWall = w;
                found = true;
            }
        }

        // If not, spawn new wall
        if (!found)
        {
            auto b = new BoxCollidable(Collidable::OBSTACLE, wall);
            b->moveAbs(xLogic, yLogic);
            b->scaleAbs(1.f, 1.f / 20);
            walls.push_back(b);

            lastWall = b;
        }
    }
}

// Editor function to write the map to a file.
// This makes it easy to create and edit maps.
void Tema1::writeToFile()
{
    ofstream fout;
    fout.open(MAP_PATH_NAME);
    vector<MeshInfo *> meshes;

    // Adds all unique meshes to a vector
    for (auto itWalls = walls.begin(); itWalls != walls.end(); itWalls++)
    {
        bool skip = false;
        for (auto itMesh = meshes.begin(); meshes.size() != 0 && itMesh != meshes.end() && !skip; itMesh++)
        {
            if ((*itWalls)->getMeshInfo() == *itMesh)
                skip = true;
        }
        if (!skip)
            meshes.push_back((*itWalls)->getMeshInfo());
    }

    // For each mesh, save it's information
    for (auto m : meshes)
    {
        fout << "mesh " << m->meshName << ' ' << m->center << ' ' << m->t << ' ';
        if (m->t == MeshType::RECTANGLE)
        {
            fout << m->width << ' ' << m->height << ' ' << m->color << ' ' << m->fill;
        }

        fout << endl;


        // Then save the information of all the wall that have that mesh
        for (auto b : walls)
        {
            if (b->getMeshInfo() == m)
            {
                fout << "obj ";

                if (m->t == MeshType::RECTANGLE)
                {
                    BoxCollidable *box = (BoxCollidable *)b;
                    fout << box->t << ' ';
                    fout << box->center << ' ' << box->angle << ' ' << box->scaleX << ' ' << box->scaleY << endl;
                }
            }
        }
    }

    fout.close();
}

// Reads the map from a file for loading on game start
// Returns false on unsuccessful loading
bool Tema1::readFromFile()
{
    // Currently only handles Rectangle meshes used for walls, but it's easy to implement other mesh types

    ifstream fin(MAP_PATH_NAME);
    if (!fin.is_open())
        return false;
    else
    {
        MeshInfo *m = new MeshInfo();

        std::string typeRead;

        while (true)
        {
            fin >> typeRead;
            if (fin.eof())
                break;

            // If the current line contains a mesh, creates the mesh
            if (typeRead == "mesh")
            {
                int meshTypeRead;
                fin >> m->meshName;

                readVec3File(fin, &m->center);

                fin >> meshTypeRead;
                m->t = MeshType(meshTypeRead);

                if (m->t == MeshType::RECTANGLE)
                {
                    fin >> m->width >> m->height;

                    readVec3File(fin, &m->color);

                    fin >> m->fill;

                    Mesh *mesh1 = object2D::CreateRectangle(m->meshName, m->center, m->width, m->height, m->color, m->fill);
                    AddMeshToList(mesh1);

                    wall = m;
                }
            }
            else
            {   
                // Otherwise uses the last read mesh to create walls.
                if (m->t == MeshType::RECTANGLE)
                {
                    int CollidableType;
                    fin >> CollidableType;

                    auto b = new BoxCollidable(Collidable::Type(CollidableType), m);
                    auto newCenter = glm::vec3(0, 0, 0);

                    readVec3File(fin, &newCenter);
                    b->moveAbs(newCenter.x, newCenter.y);
                    float angle;
                    fin >> angle;
                    b->rotateAbs(angle);
                    float scaleX, scaleY;
                    fin >> scaleX >> scaleY;

                    b->scaleAbs(scaleX, scaleY);

                    walls.push_back(b);
                }
            }
        }

        return true;
    }
}