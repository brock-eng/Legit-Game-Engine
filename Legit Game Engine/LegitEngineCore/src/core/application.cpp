#pragma once

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#ifndef _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "application.h"

namespace legit_engine {

   using namespace graphics;
   using namespace components;
   using namespace shaders;
   using namespace buffers;
   using namespace renderables;

   Application::Application(const char* name, unsigned int screenWidth, unsigned int screenHeight)
   {
      m_Window = new graphics::Window(name, screenWidth, screenHeight);
      m_Renderer = new renderables::BatchRenderer2D(screenWidth, screenHeight);
      m_Shader = new shaders::Shader("res/textured.shader");
      m_DebugAPI = new utils::DebugUtil();

      // Basic shader setup.  Legit engine currently does not support multiple shader files for 
      // a single application
      glUseProgram(m_Shader->getRendererID());
      mat4 ortho = mat4::orthographic(0, screenWidth, 0, screenHeight, -1.0f, 1.0f);
      m_Shader->setUniformMat4("pr_matrix", ortho);
      auto loc = glGetUniformLocation(m_Shader->getRendererID(), "textures");
      int textures[MAX_TEXTURES];
      for (int i = 0; i < MAX_TEXTURES; i++)
         textures[i] = i;
      glUniform1iv(loc, MAX_TEXTURES, textures);

      m_MousePosition = { 0, 0 };
   }

   Application::~Application()
   {
      delete m_DebugAPI;
      delete m_Renderer;
      delete m_Shader;
      delete m_Window;

      // report memory leaks to vs debugger window
      _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
      _CrtDumpMemoryLeaks();
   }

   void Application::Start()
   {
      bootGui(m_Window);

      m_Window->getMousePositionNormalized(m_MousePosition.x, m_MousePosition.y);
      m_Window->getWindowSize(m_ScreenWidth, m_ScreenHeight);

      ApplicationThread();

      m_DebugAPI->pollPerformance();
   }

   void Application::ApplicationThread()
   {
      m_Active = true;
      if (!OnUserCreate())
      {
         m_Active = false;
      }


      float fOldTime = glfwGetTime();
      m_DebugAPI->timerStart();

      while (m_Active)
      {
         m_DebugAPI->GLCheckErrors();
         m_DebugAPI->update();

         float fNow = glfwGetTime();
         float fElapsedTime = (float)(fNow - fOldTime);
         fOldTime = fNow;

         for (int i = 0; i < MAX_KEYS; i++)
         {
            m_KeysNewState[i] = m_Window->KeyPressed(i);

            m_Keys[i].bPressed = false;
            m_Keys[i].bReleased = false;

            if (m_KeysNewState[i] != m_KeysOldState[i])
            {
               if (m_KeysNewState[i])
               {
                  m_Keys[i].bPressed = true;
                  m_Keys[i].bHeld = true;
               }
               else
               {
                  m_Keys[i].bReleased = true;
                  m_Keys[i].bHeld = false;
               }
            }

            m_KeysOldState[i] = m_KeysNewState[i];
         }

         for (int i = 0; i < MAX_BUTTONS; i++)
         {
            m_MouseNewState[i] = m_Window->MouseButtonsPressed(i);

            m_Mouse[i].bPressed = false;
            m_Mouse[i].bReleased = false;

            if (m_MouseNewState[i] != m_MouseOldState[i])
            {
               if (m_MouseNewState[i])
               {
                  m_Mouse[i].bPressed = true;
                  m_Mouse[i].bHeld = true;
               }
               else
               {
                  m_Mouse[i].bReleased = true;
                  m_Mouse[i].bHeld = false;
               }
            }

            m_MouseOldState[i] = m_MouseNewState[i];
         }
         m_Window->getMousePosition(m_MousePosition.x, m_MousePosition.y);
         m_Window->getWindowSize(m_ScreenWidth, m_ScreenHeight);
         m_Window->Clear();

         m_Renderer->begin();

         ImGui_ImplOpenGL3_NewFrame();
         ImGui_ImplGlfw_NewFrame();
         ImGui::NewFrame();
         

         if (!OnUserUpdate())
         {
            m_Active = false;
         }
         m_Renderer->end();
         m_Renderer->flush();

           
         ImGui::Render();
         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
         
         m_Window->Update();

         if (m_Window->Closed())
            m_Active = false;
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

   void Application::getMousePosition(float& mouseX, float& mouseY)
   {
      return m_Window->getMousePosition(mouseX, mouseY);
   }

   components::Vec2 Application::getMousePosition()
   {
      return m_Window->getMousePosition();
   }

   void Application::getMousePositionNormalized(float& mouseX, float& mouseY)
   {
      m_Window->getMousePositionNormalized(mouseX, mouseY);
   }

   components::Vec2 Application::getMousePositionNormalized()
   {
      return m_Window->getMousePositionNormalized();
   }

   void Application::bootGui(legit_engine::graphics::Window* window)
   {
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGuiIO& io = ImGui::GetIO(); (void)io;
      ImGui::StyleColorsDark();
      ImGui_ImplGlfw_InitForOpenGL(window->getWindowPointer(), true);
      ImGui_ImplOpenGL3_Init();
      ImGui::GetStyle().ScaleAllSizes(2.0f);
   }
}