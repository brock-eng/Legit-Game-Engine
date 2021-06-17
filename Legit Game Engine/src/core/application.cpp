#pragma once

#include "application.h"

namespace legit_engine {

   using namespace graphics;
   using namespace components;
   using namespace shaders;
   using namespace buffers;
   using namespace renderables;

   Application::Application(const char* name, unsigned int screenWidth, unsigned int screenHeight)
   {
      // s_Instance = this;
      m_Window = new graphics::Window(name, screenWidth, screenHeight);
      m_Renderer = new renderables::BatchRenderer2D;
      m_Shader = new shaders::Shader("src/graphics/shaders/textured.shader");
      m_DebugAPI = new utils::DebugUtil();

      // Basic shader setup.  Legit engine currently does not support multiple shader files for 
      // a single application
      float xL = 0;
      float xR = screenWidth;
      float yB = 0;
      float yT = screenHeight;
      mat4 ortho = mat4::orthographic(xL, xR, yB, yT, -1.0f, 1.0f);
      m_Shader->setUniformMat4("pr_matrix", ortho);
      glUseProgram(m_Shader->getRendererID());
   }

   Application::~Application()
   {
      m_DebugAPI->pollPerformance();
      delete m_DebugAPI;
      delete m_Renderer;
      delete m_Shader;
      delete m_Window;
      // delete s_Instance;
   }

   void Application::ApplicationThread()
   {
   }

   void Application::Start()
   {
      OnUserCreate();

      m_DebugAPI->timerStart();

      while (! m_Window->Closed())
      {
         OnUserUpdate();
      }
   }

   void Application::getScreenSize(int& width, int& height) 
   {
      m_Window->getWindowSize(width, height);
   }

   components::Vec2 Application::getScreenSize()
   {
      return m_Window->getWindowSize();
   }

   void Application::getMousePosition(double& mouseX, double& mouseY)
   {
      return m_Window->getMousePosition(mouseX, mouseY);
   }

   components::Vec2 Application::getMousePosition()
   {
      return m_Window->getMousePosition();
   }

   void Application::getMousePositionNormalized(double& mouseX, double& mouseY)
   {
      m_Window->getMousePositionNormalized(mouseX, mouseY);
   }

   components::Vec2 Application::getMousePositionNormalized()
   {
      return m_Window->getMousePositionNormalized();
   }
}