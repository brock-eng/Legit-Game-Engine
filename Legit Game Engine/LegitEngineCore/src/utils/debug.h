/*
   class DebugUtil mostly provides methods for console logging and input testing
*/

#pragma once

#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <thread>
#include <vector>

#include "../graphics/window.h"

namespace legit_engine {
   namespace utils {

      class DebugUtil
      {
      private:
         double m_StartTime;
         double m_Last;
         unsigned int m_FrameCount;
      public:
         DebugUtil();
         ~DebugUtil();
         void clearConsole();
         void printMousePos(const graphics::Window& window);
         void setKeyWatch(const graphics::Window* window, unsigned int keycode);
         void timerStart();
         void update();
         void showFPS(graphics::Window* window, const char* title = "Application");
         std::pair<float, float> getMemoryUsage();
         void pollPerformance();

         static void GLClearError();
         static void GLCheckErrors();

      };
   }
}