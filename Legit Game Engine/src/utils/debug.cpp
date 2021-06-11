#pragma once

#include "debug.h"

namespace legit_engine {
   namespace utils {


      void DebugUtil::clearConsole()
      {
         COORD topLeft = { 0, 0 };
         HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
         CONSOLE_SCREEN_BUFFER_INFO screen;
         DWORD written;

         GetConsoleScreenBufferInfo(console, &screen);
         FillConsoleOutputCharacterA(
            console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
         );
         FillConsoleOutputAttribute(
            console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
            screen.dwSize.X * screen.dwSize.Y, topLeft, &written
         );
         SetConsoleCursorPosition(console, topLeft);
      }

      void DebugUtil::printMousePos(const graphics::Window& window)
      {
         double X, Y;
         window.getMousePosition(X, Y);
         std::cout << "Mouse_Pos: <" << X << ", " << Y;
      }

      void DebugUtil::setKeyWatch(const graphics::Window* window, unsigned int keycode)
      {
         window->KeyPressed(keycode);
      }

      void delKeyWatch()
      {

      }

   }
}