#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include "src/core/application.h"

#include "src/graphics/Renderables/texture.h"


#define LOG(x) (std::cout << x << std::endl)

#define FR (float_rand(-1, 1))

float float_rand(float b, float e)
{
   return ((float)rand() / RAND_MAX) * (e - b) + b;
}




using namespace legit_engine;
using namespace renderables;
using namespace components;

class testApplication : public Application
{
   public:
      testApplication(const char* name, unsigned int screenWidth, unsigned int screenHeight)
         : Application(name, screenWidth, screenHeight) {}

      ImVec4 clear_color = ImVec4(0, 0., 0.5, 0);
      std::vector<legit_engine::renderables::Sprite*> sprites;

      int screenHeight, screenWidth;

      std::vector<Texture> textures;

      Texture texture1 = Texture("src/res/hero.jpg");
      Texture texture2 = Texture("src/res/pepesprites.png");
      Texture texture3 = Texture("src/res/booba.jpg");

      Entity player = Entity(400, 400, 200, 600, 45.0f, texture1);
      Entity player2 = Entity(800, 400, 500, 600, 0.0f, texture2);

      bool OnUserCreate() override
      {
         const int NUM_SPRITES = 20;

         srand(0);
         int countTest = 0;
         getScreenSize(screenWidth, screenHeight);
         float width = (float)screenWidth, height = (float)screenHeight;

         sprites.clear();
         for (float i = 0; i < 1.0f; i += 1.0 / NUM_SPRITES)
         {
            for (float j = 0; j < 1.0f; j += 1.0f / NUM_SPRITES)
            {
               switch (countTest)
               {
               case 0: sprites.push_back(new Sprite(i * width, j * height, width / NUM_SPRITES * 0.9f, height / NUM_SPRITES * 0.9f, Vec4(0.5, 0.5, 0, 0), m_Shader));
                  break;
               case 1: sprites.push_back(new Sprite(i * width, j * height, width / NUM_SPRITES * 0.9f, height / NUM_SPRITES * 0.9f, Vec4(0.5, 0.1, 0, 0), m_Shader));
                  break;
               case 2: sprites.push_back(new Sprite(i * width, j * height, width / NUM_SPRITES * 0.9f, height / NUM_SPRITES * 0.9f, Vec4(0.1, 0.5, 0.8, 0), m_Shader));
                  countTest -= 3;
                  break;
               }
               countTest++;
            }
         }

         LOG("Num Sprites: " << sprites.size());

         sprites[0]->setUV(0, 0.75, 0.25, 0.25);
         Vec3 position = { 200, 200, 0 };
         float rotation = 45.0f;
         Vec2 size = { 200.0f, 200.0f };
         mat4 transform = mat4::translation(position)
            * mat4::rotation(rotation, { 0.0f, 0.0f, 1.0f })
            * mat4::scale({ size.x, size.y, 1.0f });

         mat4::translation(Vec3(200.0f, 0, 0));

         m_Shader->setUniformMat4("vw_matrix", mat4::translation(Vec3(1.0f, 0, 0)));

         std::cout << transform.toString() << std::endl;

         return true;
      }

      bool pause = false;
      bool OnUserUpdate() override
      {
         bool windowBool = false, click_effect;
         static float intensity = 0.5f;
         
         components::Vec2 positions = getMousePositionNormalized();

         getScreenSize(screenWidth, screenHeight);
         float width = (float)screenWidth, height = (float)screenHeight;
         int NUM_SPRITES = 20;

         player.setPosition(800, 500);

         for (Sprite* sprite : sprites)
         {
            m_Renderer->submitSprite(sprite);
         }
         m_Renderer->submitEntity(&player);
         m_Renderer->submitEntity(&player2);
         float rotation = player.getRotation();
         player2.modifyRotation(0.0f);
         m_Renderer->submitSprite(sprites[0]);
         {  // Gui shit
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::Begin("Debug Menu", 0, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::SliderFloat("Lighting Intensity", &intensity, 0.0f, 1.0f);
            ImGui::ColorEdit3("Background", (float*)&clear_color);
            ImGui::Checkbox("Click Effect", &click_effect);
            ImGui::Checkbox("Pause", &pause);
            while (pause)
            {

            }
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::InputFloat("Rotation", &rotation);
            ImGui::SetWindowFontScale(1.5f);
            ImGui::End();
         }
         player.modifyRotation(0.5f);
         glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);

         if (click_effect && m_Mouse[BUTTON_1].bHeld)
         {
            intensity *= 2.0f;
            m_Shader->setUniform1f("click", intensity);
            intensity /= 2.0f;
         }
         else
         m_Shader->setUniform1f("click", intensity);

         m_Shader->setUniformMat4("vw_matrix", mat4::identity());

         if (m_Keys[KEY_LEFT].bHeld)
            m_Shader->setUniformMat4("vw_matrix", mat4::translation(Vec3(-0.02f, 0, 0)));
         if (m_Keys[KEY_RIGHT].bHeld)
            m_Shader->setUniformMat4("vw_matrix", mat4::translation(Vec3(0.02f, 0, 0)));


         m_Window->getWindowSize(screenWidth, screenHeight);
         positions = m_Window->getMousePositionNormalized();

         m_Shader->setUniform2f("light_pos", Vec2(2.0f * positions.x / screenWidth - 1.0f, 2.0f * positions.y / screenHeight - 1.0f));

         m_DebugAPI->update();

         m_DebugAPI->GLCheckErrors();
         mat4 ortho = mat4::orthographic(0, screenWidth, 0, screenHeight, -1.0f, 1.0f);
         m_Shader->setUniformMat4("pr_matrix", ortho);


         return true;
      }
};

int main()
{
   int screenWidth = 2000, screenHeight = 1500;
   double mouseX = 0, mouseY = 0;
   const char* appName = "babbees first graphics engine";

   testApplication game(appName, screenWidth, screenHeight);

   game.Start();
}


 
   

  