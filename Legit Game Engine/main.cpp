
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glew.h>
#include <glfw3.h>

#include <Windows.h>

#include "src/graphics/window.h"
#include "src/Components/Sys.h"
#include "src/graphics/Shaders/shaders.h"

#include "src/graphics/Buffers/buffer.h"
#include "src/graphics/Buffers/indexbuffer.h"
#include "src/graphics/Buffers/vertexarray.h"

#define VAR_NAME(v) (#v)


int main()
{
   using namespace legit_engine;
   using namespace graphics;
   using namespace components;
   using namespace shaders;
   using namespace buffers;

   int screenWidth = 800, screenHeight = 600;
   
   Window gameWindow("babbees first graphics engine", screenWidth, screenHeight);
   // glClearColor(0.2f, 0.3f, 0.8f, 5.0f);
   
   double mouseX = 0, mouseY = 0;

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

   VertexArray va;
   Buffer* vb = new Buffer(vertices, 4 * 3, 3);
   IndexBuffer ibo(indices, 6);

   va.AddBuffer(vb, 0);
   mat4 ortho = mat4::orthographic(0.0, 0.5f, 0.0f, 0.5f, -1.0f, 1.0f);

   Shader shaderSys("src/graphics/shaders/light.shader");
   //Shader shaderSys("src/graphics/shaders/basic.shader");

   shaderSys.Enable();
   shaderSys.setUniformMat4("pr_matrix", ortho);
   shaderSys.setUniformMat4("ml_matrix", mat4::translation(Vec3(0, 0.5, 0)));

   shaderSys.setUniform2f("light_pos", Vec2(0, 0));
   shaderSys.setUniform4f("colour", Vec4(1.0f, 0.5f, 0.0f, 1.0f));



   // TestMatrices();
   va.bind();
   ibo.bind();
   glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_SHORT, 0);

   gameWindow.Update();
#if 1
   while (!gameWindow.Closed()) 
   {
      gameWindow.Clear();
      glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_SHORT, 0);

      gameWindow.Update();
      if (gameWindow.KeyPressed(GLFW_KEY_SPACE))
      {
         std::cout << "Lol bruh";
      }
      if (gameWindow.KeyPressed(GLFW_KEY_A))
      {
         std::cout << "True" << std::endl;
      }
      if (gameWindow.MouseButtonsPressed(GLFW_MOUSE_BUTTON_LEFT))
      {
         std::cout << "pressed:" << mouseX << " " << mouseY << std::endl;
         shaderSys.setUniform1f("click", 2.0f);
      }
      else
         shaderSys.setUniform1f("click", 1.0f);
      
      gameWindow.getWindowSize(screenWidth, screenHeight);
      Vec2 positions = gameWindow.getMousePosition();
      positions.x *= 2.0f / screenWidth;
      positions.y *= -2.0f / screenHeight;
      positions.x -= 1;
      positions.y += 1;

      shaderSys.setUniform2f("light_pos", positions);
      gameWindow.printXYPos();
      //std::cout << "Running" << std::endl;
      
   }
#endif
}