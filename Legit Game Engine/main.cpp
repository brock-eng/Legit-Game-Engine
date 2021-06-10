
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glew.h>
#include <glfw3.h>

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
   using namespace Shaders;
   using namespace buffers;

   int screenWidth = 800, screenHeight = 600;
   
   Window gameWindow("We out here son", screenWidth, screenHeight);
   glClearColor(0.2f, 0.3f, 0.8f, 5.0f);
   
   double mouseX = 0, mouseY = 0;

   GLfloat vertices[] =
   {
      0, 0, 0,
      0, 0.5, 0,
      0.35, 0.5, 10,
      0.45, -0.5, 1,
      0.35, -0.4, 0,
      -0.35, -0.5, 0
   };

   GLushort indices[] =
   {
      0, 1, 2,
      3, 4, 5
   };

   VertexArray va;
   Buffer* vb = new Buffer(vertices, 6 * 3, 3);
   IndexBuffer ibo(indices, 6);

   va.AddBuffer(vb, 0);

   Shader shaderSys("src/graphics/shaders/basic.shader");
   shaderSys.Enable();

   // TestMatrices();
   va.Bind();
   ibo.Bind();
   glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_SHORT, 0);

   gameWindow.Update();
#if 1
   while (!gameWindow.Closed()) 
   {
      gameWindow.Clear();
      glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_SHORT, 0);

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
      }
      
      gameWindow.getMousePosition(mouseX, mouseY);
      //gameWindow.printXYPos();

      //std::cout << "Running" << std::endl;
      
   }
#endif
}