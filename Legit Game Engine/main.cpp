
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
   Window gameWindow(appName, screenWidth, screenHeight);
   BatchRenderer2D draw;
   utils::DebugUtil debug;
   bootGui(&gameWindow);
   glClearColor(1, 1, 0, 1);
   float xL = 0;
   float xR =  screenWidth;
   float yB = 0;
   float yT =  screenHeight;

   mat4 ortho = mat4::orthographic(xL, xR, yB, yT, -1.0f, 1.0f);
   // mat4 ortho = mat4::orthographic(0, (float)screenWidth, (float)screenHeight, 0.0f, -1.0f, 1.0f);
   mat4 identity(1.0f);
   mat4 defaultMatrix;

   Vec4 positionTest = { 2, 2, 0, 0 };

   std::cout << ortho.toString();
   std::cout << identity.toString();
   std::cout << defaultMatrix.toString();
   std::cout << positionTest.multiply(ortho) << std::endl;
   std::cout << positionTest.multiply(mat4::translation(Vec3(0, 10.0f, 0))) << std::endl;

   Shader shaderSys("src/graphics/shaders/textured.shader");
   shaderSys.Enable();
   shaderSys.setUniformMat4("pr_matrix", ortho);
   shaderSys.setUniformMat4("ml_matrix", mat4::translation(Vec3(0, 10.0f, 0)));
   shaderSys.setUniform2f("light_pos", Vec2(0, 0));
   shaderSys.setUniform4f("colour", Vec4(1.0f, 0.5f, 0.0f, 1.0f));


   
   std::vector<Sprite*> sprites;
   const int NUM_SPRITES = 4;
   srand(0);
   int countTest = 0;
   
   for (float i = 0; i < 2.0f; i += 2.0f / NUM_SPRITES)
   {
      for (float j = 0; j < 2.0f; j += 2.0f / NUM_SPRITES)
      {
         switch (countTest)
         {
         case 0: sprites.push_back(new Sprite(i/2.0f * screenWidth , (j - 1.0f) * screenHeight + 200, (float) screenWidth / NUM_SPRITES * 1.5f, (float) screenHeight / NUM_SPRITES * 1.5f + 200, Vec4(float_rand(0, 1), 0, float_rand(0, 1), float_rand(0, 1)), &shaderSys, 2));
            break;
         case 1: sprites.push_back(new Sprite(i/2.0f - 1.0f, j - 1.0f, (float) screenWidth / NUM_SPRITES * 0.8f, (float) screenHeight / NUM_SPRITES * 0.8f, Vec4(float_rand(0, 1), 0, float_rand(0, 1), float_rand(0, 1)), &shaderSys, 2));
            break;
         case 2: sprites.push_back(new Sprite(i - 1.0f, j - 1.0f, 2.0f / NUM_SPRITES * 0.8f, 2.0f / NUM_SPRITES * 0.8f, Vec4(float_rand(0, 1), 0, float_rand(0, 1), float_rand(0, 1)), &shaderSys, 2));
            break;
         default:
            countTest-=4;
         }
         countTest++;
      }
   }  
   sprites.push_back(new Sprite((xR - xL) / 4.0f, (yT - yB) / 4.0f, (xR - xL) / 4.0f, (yT - yB) / 4.0f, Vec4( 0, 0, 1.0f, 0 ), &shaderSys, 0));

   LOG("Num Sprites: " << sprites.size());

   sprites[0]->setUV(0, 0.75, 0.25, 0.25);
   

   renderables::Texture texture1("src/res/hero.jpg");
   renderables::Texture texture2("src/res/pepesprites.png");
   texture1.bind(1);
   texture2.bind(2);

   bool navActive;
   ImVec4 clear_color = ImVec4(0.5, 0.5, 0.5, 0);

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
         draw.submitSprite(sprite);
         // std::cout << sprite->getPosition() << std::endl;
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

      shaderSys.setUniform2f("light_pos", Vec2(2.0f * positions.x/screenWidth - 1.0f, 2.0f * positions.y/screenHeight - 1.0f));

      debug.update();
      count++;

      mat4 ortho = mat4::orthographic(0, screenWidth, 0, screenHeight, -1.0f, 1.0f);
      shaderSys.setUniformMat4("pr_matrix", ortho);

   }

   debug.pollPerformance();
#endif


   return 0;
}