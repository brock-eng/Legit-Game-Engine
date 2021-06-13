
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

#include "src/graphics/window.h"
#include "src/Components/Sys.h"
#include "src/utils/debug.h"

#include "src/graphics/graphics_components.h"

#define VAR_NAME(v) (#v)

#if 1
   #define INCLUDE_ALL_GRAPHICS_COMPONENTS
#endif

#define LOG(x) (std::cout << x << std::endl)
float float_rand(float b, float e)
{
   return ((float)rand() / RAND_MAX) * (e - b) + b;
}

#define FR (float_rand(-1, 1))

int main()
{
   using namespace legit_engine;
   using namespace graphics;
   using namespace components;
   using namespace shaders;
   using namespace buffers;
   using namespace renderables;

   int screenWidth = 800, screenHeight = 600;
   const char* appName = "babbees first graphics engine";
   Window gameWindow("babbees first graphics engine", screenWidth, screenHeight);
   glClearColor(0.2f, 0.3f, 0.8f, 5.0f);

   mat4 ortho = mat4::orthographic(0.0, 0.5f, 0.0f, 0.5f, -1.0f, 1.0f);

   Shader shaderSys("src/graphics/shaders/fancy_shader.shader");
   //Shader shaderSys("src/graphics/shaders/basic.shader");

   shaderSys.Enable();
   shaderSys.setUniformMat4("pr_matrix", ortho);
   shaderSys.setUniformMat4("ml_matrix", mat4::translation(Vec3(0, 0.5, 0)));

   shaderSys.setUniform2f("light_pos", Vec2(0, 0));
   shaderSys.setUniform4f("colour", Vec4(1.0f, 0.5f, 0.0f, 1.0f));
   
   Sprite sprite(0.5, 0.5, 0.4, 0.4, Vec4(1, 0, 1, 1));
   Sprite sprite2(0.7, 0.1, 0.2, 0.3, Vec4(0.2f, 0, 1, 1));
   int i = 0;

   utils::DebugUtil debug;
   
   std::vector<Sprite*> sprites;
   const int NUM_SPRITES = 240;
   
   srand(0);

   for (float i = 0; i < 2.0f; i += 2.0f / NUM_SPRITES)
   {
      for (float j = 0; j < 2.0f; j += 2.0f / NUM_SPRITES)
      {
         sprites.push_back(new Sprite(i - 1.0f, j - 1.0f, 2.0f / NUM_SPRITES * 0.8f, 2.0f / NUM_SPRITES * 0.8f, Vec4(float_rand(0, 1), 0, float_rand(0, 1), float_rand(0, 1))));
      }
   } 
   // sprites.push_back(new Sprite(float_rand(-1, 1), float_rand(-1, 1), 1, 1, Vec4(float_rand(0, 1), float_rand(0, 1), float_rand(0, 1), float_rand(0, 1))));
   LOG("Num Sprites: " << sprites.size());

   BatchRenderer2D draw;

   GLfloat vertices[] =
   {
      1, 1, 0,
      1, -1, 0,
      -1, 1, 0,
      -1, -1, 0,
   };

   GLushort indices[] =
   {
      0, 1, 2,
      1, 2, 3
   };

   double mouseX = 0, mouseY = 0;
   
   GLenum err = glewInit();
   if (err != GLEW_OK)
   {
      LOG("Glew Initializer Failed");
   }

   gameWindow.Update();

   IMGUI_CHECKVERSION();
   ImGui::CreateContext();
   ImGuiIO& io = ImGui::GetIO(); (void)io;
   ImGui::StyleColorsDark();
   ImGui_ImplGlfw_InitForOpenGL(gameWindow.getWindowPointer(), true);
   ImGui_ImplOpenGL3_Init();
   ImGui::GetStyle().ScaleAllSizes(2.0f);


   bool navActive;
   ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
#if 1
   while (!gameWindow.Closed()) 
   {
      bool windowBool = false, click_effect;
      static float f = 0.0f;
      gameWindow.Clear();
      draw.begin();
      debug.GLCheckErrors();
      for (auto sprite : sprites)
      {
         draw.submit(sprite);
      }
      draw.end();
      draw.flush();
      
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();
      ImGui::Begin("Debug Menu", 0, ImGuiWindowFlags_AlwaysAutoResize);
      ImGui::SliderFloat("Lighting Intensity", &f, 0.0f, 1.0f);            
      ImGui::ColorEdit3("Background", (float*)&clear_color); 
      ImGui::Checkbox("Click Effect", &click_effect);
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::SetWindowFontScale(1.5f);
      ImGui::End();
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);

      gameWindow.Update();
      if (click_effect && gameWindow.MouseButtonsPressed(GLFW_MOUSE_BUTTON_1))
      {
         f *= 2.0f;
         shaderSys.setUniform1f("click", f);
         f /= 2.0f;
      }
      else
         shaderSys.setUniform1f("click", f);
      

      // debug.showFPS(&gameWindow, "we out here");
      gameWindow.getWindowSize(screenWidth, screenHeight);
      Vec2 positions = gameWindow.getMousePositionNormalized();

      shaderSys.setUniform2f("light_pos", positions);

      if (gameWindow.MouseButtonsPressed(GLFW_MOUSE_BUTTON_2))
         f += 0.05;
      // gameWindow.printXYPos();
      //std::cout << "Running" << std::endl;
      
   }
#endif
}