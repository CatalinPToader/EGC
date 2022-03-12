#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema2/cam.h"
#include "lab_m1/tema2/Player.h"
#include "lab_m1/tema2/Collidable.h"
#include "lab_m1/tema2/Enemy.h"
#include "lab_m1/tema2/Bullet.h"

#define MAZE_MAX 17
#define MAZE_MIN 1
#define CELL_SZ 20
#define SHOOTING_COOLDOWN 0.5f
#define PLAYTIME 300

namespace m1
{
   class Tema2 : public gfxc::SimpleScene
   {

      struct pos2D
      {
         int x;
         int y;
         pos2D(int _x, int _y) : x(_x), y(_y) {}
      };

   public:
      Tema2();
      ~Tema2() override;

      void Init() override;

   private:
      void FrameStart() override;
      void Update(float deltaTimeSeconds) override;
      void FrameEnd() override;

      void RenderPlayer();
      void DrawHud(glm::mat3 visMatrix);
      void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, Texture2D *text = nullptr);
      void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, glm::vec3 pos = glm::vec3(0), float deathTime = 0, float hpRatio = 0);
      void RenderMesh2D(Mesh *mesh, const glm::mat3 &modelMatrix, Shader *shader, Texture2D *text);

      void OnInputUpdate(float deltaTime, int mods) override;
      void OnKeyPress(int key, int mods) override;
      void OnKeyRelease(int key, int mods) override;
      void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
      void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
      void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
      void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
      void OnWindowResize(int width, int height) override;

      static glm::mat3 VisualizationTransf2DUnif(const homework::LogicSpace &logicSpace, const homework::ViewportSpace &viewSpace);
      void SetViewportArea(const homework::ViewportSpace &viewSpace,
                           glm::vec3 colorColor);


      void genMaze();

   protected:
      std::unordered_map<std::string, Texture2D *> mapTextures;

      homework::Camera *camera{};
      glm::mat4 projectionMatrix{};
      bool renderCameraTarget{};

      float left{}, right{}, bottom{}, top{};
      float fov{};
      float zNear{}, zFar{};

      int maze[MAZE_MAX + 1][MAZE_MAX + 1]{};
      glm::vec3 startPos{};

      homework::Player p;

      float angleY{};
      float lastShot{};

      bool lost{};

      std::vector<homework::Collidable *> walls;
      std::vector<homework::Enemy *> enemies;
      std::vector<homework::Bullet *> bullets;

      homework::MeshInfo *bullet = new homework::MeshInfo("sphere", "TexShader", "bullet");
      Texture2D *noise{};

      homework::LogicSpace logicSpace;

       void RenderWalls();

       void UpdateBullets(float deltaTimeSeconds);

       void UpdateEnemies(float deltaTimeSeconds);
   };
} // namespace m1
