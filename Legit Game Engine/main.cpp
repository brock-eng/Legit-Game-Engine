#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include "src/core/application.h"

#define LOG(x) (std::cout << x << std::endl)

#define FR (float_rand(-1, 1))

float float_rand(float b, float e)
{
   return ((float)rand() / RAND_MAX) * (e - b) + b;
}


void bootGui(legit_engine::graphics::Window* window)
{
   IMGUI_CHECKVERSION();
   ImGui::CreateContext();
   ImGuiIO& io = ImGui::GetIO(); (void)io;
   ImGui::StyleColorsDark();
   ImGui_ImplGlfw_InitForOpenGL(window->getWindowPointer(), true);
   ImGui_ImplOpenGL3_Init();
   ImGui::GetStyle().ScaleAllSizes(2.0f);
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
      int screenWidth = 0, screenHeight = 0;

      

      void OnUserCreate() override
      {
         const int NUM_SPRITES = 20;

         srand(0);
         int countTest = 0;

         Texture texture1("src/res/hero.jpg");
         Texture texture2("src/res/pepesprites.png");
         Texture texture3("src/res/booba.jpg");

         getScreenSize(screenWidth, screenHeight);
         float width = (float)screenWidth, height = (float)screenHeight;

         for (float i = 0; i < 1.0f; i += 1.0 / NUM_SPRITES)
         {
            for (float j = 0; j < 1.0f; j += 1.0f / NUM_SPRITES)
            {
               switch (countTest)
               {
               case 0: sprites.push_back(new Sprite(i * width, j * height, width / NUM_SPRITES, height / NUM_SPRITES, texture1, m_Shader));
                  break;
               case 1: sprites.push_back(new Sprite(i * width, j * height, width / NUM_SPRITES, height / NUM_SPRITES, texture2, m_Shader));
                  break;
               case 2: sprites.push_back(new Sprite(i * width, j * height, width / NUM_SPRITES, height / NUM_SPRITES, Vec4{ 0.5f, 0.5f, 0, 0 }, m_Shader));
                  countTest -= 3;
                  break;
               }
               countTest++;
            }
         }

         LOG("Num Sprites: " << sprites.size());

         sprites[0]->setUV(0, 0.75, 0.25, 0.25);

         bootGui(m_Window);
         
      }

      void OnUserUpdate() override
      {

         bool windowBool = false, click_effect;
         static float intensity = 0.5f;
         components::Vec2 positions = getMousePositionNormalized();
         m_Window->Clear();
         m_Renderer->begin();
         m_DebugAPI->GLCheckErrors();
         float mod = 0.005f;
         sprites[0]->setPosition(positions.x, positions.y, 0);
         for (Sprite* sprite : sprites)
         {
            m_Renderer->submitSprite(sprite);
         }
         m_Renderer->end();
         m_Renderer->flush();


         { // Gui shit
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::Begin("Debug Menu", 0, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::SliderFloat("Lighting Intensity", &intensity, 0.0f, 1.0f);
            ImGui::ColorEdit3("Background", (float*)&clear_color);
            ImGui::Checkbox("Click Effect", &click_effect);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::SetWindowFontScale(1.5f);
            ImGui::End();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
         }

         glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);

         m_Window->Update();
         if (click_effect && m_Window->MouseButtonsPressed(GLFW_MOUSE_BUTTON_1))
         {
            intensity *= 2.0f;
            m_Shader->setUniform1f("click", intensity);
            intensity /= 2.0f;
         }
         else
            m_Shader->setUniform1f("click", intensity);

         m_Window->getWindowSize(screenWidth, screenHeight);
         positions = m_Window->getMousePositionNormalized();

         m_Shader->setUniform2f("light_pos", Vec2(2.0f * positions.x / screenWidth - 1.0f, 2.0f * positions.y / screenHeight - 1.0f));

         m_DebugAPI->update();

         mat4 ortho = mat4::orthographic(0, screenWidth, 0, screenHeight, -1.0f, 1.0f);
         m_Shader->setUniformMat4("pr_matrix", ortho);
      }
};

int main()
{



   int screenWidth = 800, screenHeight = 600;
   double mouseX = 0, mouseY = 0;
   const char* appName = "babbees first graphics engine";

   testApplication game(appName, screenWidth, screenHeight);

   game.Start();



}


 
   

  