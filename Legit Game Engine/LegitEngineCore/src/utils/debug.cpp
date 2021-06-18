#pragma once

#include <sstream>
#include "debug.h"

namespace legit_engine {
   namespace utils {

      DebugUtil::DebugUtil()
      {
         m_StartTime = glfwGetTime();
         m_Last = -1.0f;
         m_FrameCount = 0;
      }

      DebugUtil::~DebugUtil()
      {
      }

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

      void DebugUtil::setKeyWatch(const graphics::Window* window, unsigned int keycode)
      {
         window->KeyPressed(keycode);
      }

      void DebugUtil::showFPS(graphics::Window* window, const char* title)
      {
         float now = glfwGetTime();
         float dt = now - m_Last;
         m_Last = now;
         float fps = 1.0f / dt;

         std::stringstream ss;
         ss << title << " [FPS: " << fps << "]";
         glfwSetWindowTitle(window->getWindowPointer(), ss.str().c_str());
      }

      void DebugUtil::timerStart()
      {
         m_StartTime = glfwGetTime();
      }

      void DebugUtil::update()
      {
         m_FrameCount++;
      }

      void DebugUtil::pollPerformance()
      {
         if (m_FrameCount == -1)
         {
            std::cout << "Error: DebugUtil.update() must be called at least once to poll performance time." << std::endl;
            return;
         }
         float now = glfwGetTime();
         float avgFPS = 1.0f / ((now - m_StartTime) /  (float) m_FrameCount );

         std::cout << "[Debug]: Average FPS -> " << avgFPS << std::endl;
      }

      void DebugUtil::GLClearError()
      {
         while (glGetError() != GL_NO_ERROR);
      }
      
      void DebugUtil::GLCheckErrors()
      {
         const char* errorMessage;
         while (glfwGetError(&errorMessage))
         {
            std::cout << "[OpenGL Error] " << errorMessage;
         }
     }
     

   }
}