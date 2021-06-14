
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>

#include <glew.h>
#include <glfw3.h>

#include <Windows.h>

#include <random>

#include "src/utils/ImGui/imgui.h"
#include "src/utils/ImGui/imgui_impl_glfw.h"
#include "src/utils/ImGui/imgui_impl_opengl3.h"

#include "src/Components/Sys.h"
#include "src/utils/debug.h"

#include "src/graphics/Renderables/texture.h"

#include "src/helpers.h"

#if 0
#define INCLUDE_ALL_GRAPHICS_COMPONENTS
#endif

#include "src/graphics/graphics_components.h"

#define VAR_NAME(v) (#v)

int main()
{
   using namespace legit_engine;
   using namespace graphics;
   using namespace components;
   using namespace shaders;
   using namespace buffers;
   using namespace renderables;

   int screenWidth = 800, screenHeight = 600;
   double mouseX = 0, mouseY = 0;
   const char* appName = "babbees first graphics engine";
   Window gameWindow("babbees first graphics engine", screenWidth, screenHeight);
   glClearColor(0.2f, 0.3f, 0.8f, 5.0f);

   mat4 ortho = mat4::orthographic(0.0, 0.5f, 0.0f, 0.5f, -1.0f, 1.0f);

   Shader shaderSys("src/graphics/shaders/textured.shader");
   shaderSys.Enable();
   shaderSys.setUniformMat4("pr_matrix", ortho);
   shaderSys.setUniformMat4("ml_matrix", mat4::translation(Vec3(0, 0.5, 0)));
   shaderSys.setUniform2f("light_pos", Vec2(0, 0));
   shaderSys.setUniform4f("colour", Vec4(1.0f, 0.5f, 0.0f, 1.0f));


   utils::DebugUtil debug;
   
   std::vector<Sprite*> sprites;
   const int NUM_SPRITES = 150;
   
   srand(0);
   for (float i = 0; i < 2.0f; i += 2.0f / NUM_SPRITES)
   {
      for (float j = 0; j < 2.0f; j += 2.0f / NUM_SPRITES)
      {
         sprites.push_back(new Sprite(i - 1.0f, j - 1.0f, 2.0f / NUM_SPRITES * 0.8f, 2.0f / NUM_SPRITES * 0.8f, Vec4(float_rand(0, 1), 0, float_rand(0, 1), float_rand(0, 1))));
      }
   } 
   
   LOG("Num Sprites: " << sprites.size());

   BatchRenderer2D draw;

   

   bootGui(&gameWindow);

   glActiveTexture(GL_TEXTURE0);
   renderables::Texture texture("src/res/sample2.jpg");
   renderables::Texture texture2("src/res/sample.png");
   texture.bind();

   bool navActive;
   ImVec4 clear_color = ImVec4(0, 0, 0, 0);
#if 1

   debug.timerStart();
   int count = 0;
   Vec2 positions = gameWindow.getMousePositionNormalized();

   // disable vsync
   glfwSwapInterval(0);

   while (!gameWindow.Closed()) 
   {
      bool windowBool = false, click_effect;
      static float intensity = 0.5f;
      gameWindow.Clear();
      draw.begin();
      debug.GLCheckErrors();
      float mod = 0.005f;
      sprites[0]->setPosition(positions.x, positions.y, 0);
      for ( Sprite* sprite: sprites)
      {
         //components::Vec3& position = sprite->getPositionM();
         //sprite->setPosition(position.x + mod, position.y, 0);
         draw.submit(sprite);
      }
      draw.end();
      draw.flush();
      
      
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

      gameWindow.Update();
      if (click_effect && gameWindow.MouseButtonsPressed(GLFW_MOUSE_BUTTON_1))
      {
         intensity *= 2.0f;
         shaderSys.setUniform1f("click", intensity);
         intensity /= 2.0f;
      }
      else
         shaderSys.setUniform1f("click", intensity);
      
      gameWindow.getWindowSize(screenWidth, screenHeight);
      positions = gameWindow.getMousePositionNormalized();

      shaderSys.setUniform2f("light_pos", positions);

      debug.update();
      count++;
   }

   debug.pollPerformance();
#endif


   return 0;
}