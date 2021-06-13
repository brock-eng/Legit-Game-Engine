/*
   class DebugUtil mostly provides methods for console logging and input testing
*/

#pragma once

#include <Windows.h>
#include <iostream>
#include <thread>

#include "../graphics/window.h"

namespace legit_engine {
   namespace utils {

      class DebugUtil
      {
      private:
         double m_Now;
         double m_Last;
         int m_FrameCount;
      public:
         DebugUtil();
         ~DebugUtil();
         void clearConsole();
         void printMousePos(const graphics::Window& window);
         void setKeyWatch(const graphics::Window* window, unsigned int keycode);
         void showFPS(graphics::Window* window, const char* title = "Application");

         static void GLClearError();
         static void GLCheckErrors();

      };
   }
}