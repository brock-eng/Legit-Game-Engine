#pragma once

#include <glew.h>
#include <glfw3.h>

namespace legit_engine {
   namespace graphics {

   #define MAX_KEYS  1024
   #define MAX_BUTTONS 32

      class Window
      {
      private:
         const char *m_Name;
         int m_ScreenWidth, m_ScreenHeight;
         GLFWwindow *m_Window;
         bool m_Closed;

         static bool m_Keys[MAX_KEYS];
         static bool m_MouseButtons[MAX_BUTTONS];
         static double m_MouseX, m_MouseY;
         
      public:
         Window(const char* name, int width, int height);
         ~Window();
         void Clear() const;
         void Update();
         bool Closed() const;
         bool KeyPressed(unsigned int keyCode);
         bool MouseButtonsPressed(unsigned int keyCode);
         void getMousePosition(double& X, double& Y);
         
         void printXYPos();

      private:
         bool Initialize();
         friend static void GLFWkey_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
         friend static void GLFWmouse_callback(GLFWwindow* window, int button, int action, int mods);
         friend static void GLFWcursor_position_callback(GLFWwindow* window, double xpos, double ypos);
      };
   }
}