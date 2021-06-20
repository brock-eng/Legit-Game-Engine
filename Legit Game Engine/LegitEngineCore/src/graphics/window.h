#pragma once

#include <glew.h>
#include <glfw3.h>

#include "../Components/Sys.h"

namespace legit_engine {
   namespace graphics {

#ifndef MAX_KEYS
   #define MAX_KEYS  1024
   #define MAX_BUTTONS 32
#endif
   

      struct WindowDimensions
      {
         float height, width;
         float xPos, yPos;

         void set(float  w, float h, float  x, float  y)
         {
            height = h;
            width = w;
            xPos = x;
            yPos = y;
         }
      };

      class Window
      {
      private:
         const char* m_Name;
         int m_ScreenWidth, m_ScreenHeight;
         GLFWwindow* m_Window;
         GLFWmonitor* m_Monitor;
         bool m_Closed;
         bool m_DisplayMode;

         WindowDimensions m_WindowProperties;

         static bool m_Keys[MAX_KEYS];
         static bool m_MouseButtons[MAX_BUTTONS];
         static double m_MouseX, m_MouseY;
         
      public:
         Window(const char* name, int width, int height);
         ~Window();
         void Clear() const;
         void Update();
         bool Closed() const;
         static bool KeyPressed(unsigned int keyCode);
         static bool MouseButtonsPressed(unsigned int keyCode);

         void getWindowSize(int& width, int& height);
         void getWindowSize(float& width, float& height);
         components::Vec2 getWindowSize();
         void setWindowSize(int width, int height);
         void getMousePosition(float& X, float& Y);
         components::Vec2 getMousePosition();
         void getMousePositionNormalized(float& X, float& Y);
         components::Vec2 getMousePositionNormalized();

         void setFullscreen();

         GLFWwindow* getWindowPointer();

         void printXYPos();

      private:
         bool Initialize();
         friend static void GLFWkey_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
         friend static void GLFWmouse_callback(GLFWwindow* window, int button, int action, int mods);
         friend static void GLFWcursor_position_callback(GLFWwindow* window, double xpos, double ypos);
      };
   }
}