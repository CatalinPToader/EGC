#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema1/player.h"
#include "lab_m1/tema1/Collidable.h"
#include "lab_m1/tema1/utils.h"
#include "lab_m1/tema1/projectile.h"
#include "lab_m1/tema1/enemy.h"
#include <vector>


// Main file containing the game and most functions
namespace m1
{
   class Tema1 : public gfxc::SimpleScene
   {

   public:
      Tema1();
      ~Tema1();

      void Init() override;

   private:
      void FrameStart() override;
      void Update(float deltaTimeSeconds) override;
      void FrameEnd() override;

      void DrawScene(glm::mat3 visMatrix, float delta);
      void DrawBullets(glm::mat3 visMatrix, float delta);
      void DrawHealthbar(glm::mat3 visMatrix, float delta);

      void OnInputUpdate(float deltaTime, int mods) override;
      void OnKeyPress(int key, int mods) override;
      void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
      void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;

      void enemyRender(Enemy *e, glm::mat3 visMatrix);
      void playerRender(glm::mat3 visMatrix);

      void EnemyUpdater(float deltaTimeSeconds);
      void EnemySpawner(float deltaTimeSeconds);

      void cameraMove(float deltaTime, int mods);
      void playerMove(float delta, int mods);
      void objectMove(float deltaTime, int mods);

      void createWalls();
      glm::mat3 VisualizationTransf2DUnif(const LogicSpace &logicSpace, const ViewportSpace &viewSpace);

      void SetViewportArea(const ViewportSpace &viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

      void writeToFile();
      bool readFromFile();

   protected:
      float length;
      ViewportSpace viewSpace, viewSpaceHealth;
      LogicSpace logicSpace, logicSpaceHealth;
      glm::mat3 modelMatrix, visMatrix;

      float newCenterX, newCenterY;

      Player *p;

      std::vector<Collidable *> walls;
      std::vector<Projectile *> projectiles;
      std::vector<Enemy *> enemies;

      int score;

      float projectileSpawnDist = 50;
      float projectileSpeed = 700;
      float projectileLifespan = 1.5f;
      float projectileDamage = 151;
      float maxCooldown = 0.33;
      float cooldown = maxCooldown;

      float maxTimeToWave = 10;
      float timeToWave = 0;
      int maxEnemies = 10;

      bool show = true;

      float minEnemyHealth = 100;
      float maxEnemyHealth = 200;
      float minEnemySpeed = 100;
      float maxEnemySpeed = 200;

      float cameraSpeed = 250;
      float moveSpeed = 100;
      float scaleSpeed = 1;
      float rotation = 30;

      MeshInfo *wall;
      MeshInfo *enemy;
      Collidable *lastWall;
      int lastWallPos;

      bool showSelection;
      bool scaleOnX;
   };
} // namespace m1