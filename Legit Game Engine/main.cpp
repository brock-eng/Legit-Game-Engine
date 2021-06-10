
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glew.h>
#include <glfw3.h>

#include "src/graphics/window.h"
#include "src/Components/Sys.h"
#include "src/graphics/shaders.h"

#define VAR_NAME(v) (#v)



int main()
{
   using namespace legit_engine;
   using namespace graphics;
   using namespace components;
   using namespace Shaders;

   int screenWidth = 800, screenHeight = 600;
   
   Window gameWindow("Legit Engine", screenWidth, screenHeight);
   glClearColor(0.2f, 0.3f, 0.8f, 5.0f);
   
   double mouseX = 0, mouseY = 0;

   float positions[6] =
   {
      -0.5f,  0.5f,
       0.5f, -0.5f,
      -0.5f, -0.5f,
   };

   unsigned int buffer;
   glGenBuffers(1, &buffer);
   glBindBuffer(GL_ARRAY_BUFFER, buffer);
   glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

   Shader shaderSys("src/graphics/shaders/basic.shader");
   shaderSys.Enable();

   // TestMatrices();

   while (!gameWindow.Closed()) 
   {
      gameWindow.Clear();
      glDrawArrays(GL_TRIANGLES, 0, 3);
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

}