/*
   class DebugUtil mostly provides methods for console logging and input testing
*/

#pragma once

#include <Windows.h>
#include <iostream>

#include "../graphics/window.h"

namespace legit_engine {
   namespace utils {

      class DebugUtil
      {
      private:

      public:
         void clearConsole();
         void printMousePos(const graphics::Window& window);
         void setKeyWatch(const graphics::Window* window, unsigned int keycode);
      };
   }
}