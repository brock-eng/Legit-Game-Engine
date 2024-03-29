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


   Application::Application()//const char* name, unsigned int screenWidth, unsigned int screenHeight)
   {
      m_Window = new graphics::Window("null", 500, 500);
      m_Renderer = new renderables::BatchRenderer2D();
      m_Shader = new shaders::Shader("res/Shaders/textured.shader");
      m_DebugAPI = new utils::DebugUtil();
   }

   Application::~Application()
   {
      delete m_DebugAPI;
      delete m_Renderer;
      delete m_Shader;
      delete m_Window;

      // report memory leaks to visual studio debugger window
      _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
      _CrtDumpMemoryLeaks();
   }

   void Application::Construct(const char* name, unsigned int screenWidth, unsigned int screenHeight)
   {
      m_Window->setWindowSize(screenWidth, screenHeight);
      m_Window->renameWindow(name);

      // Basic shader setup.  Legit engine currently does not support multiple shader files for 
      // a single application
      m_Shader->Enable();
      mat4 ortho = mat4::orthographic(0, screenWidth, 0, screenHeight, -1.0f, 1.0f);
      m_Shader->setUniformMat4("pr_matrix", ortho);
      auto loc = glGetUniformLocation(m_Shader->getRendererID(), "textures");
      int textures[MAX_TEXTURES];

      // allocating texture I.D. array
      for (int i = 0; i < MAX_TEXTURES; i++)
         textures[i] = i;
      glUniform1iv(loc, MAX_TEXTURES, textures);

      m_Constructed = true;
   }

   void Application::Start()
   {
      ASSERT(m_Constructed, "Must call construct method before starting");

      LOG("Starting");

      // ImGui setup procedure
      BootImGui(m_Window);

      m_Window->getWindowSize(m_ScreenWidth, m_ScreenHeight);
      m_Window->getMousePositionNormalized(m_MousePosition.x, m_MousePosition.y);

      ApplicationThread();
      LOG("Stopping.");
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
         float xscroll, yscroll;

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
         
         GetScrollWheel();

         m_Renderer->begin();

         ImGui_ImplOpenGL3_NewFrame();
         ImGui_ImplGlfw_NewFrame();
         ImGui::NewFrame();
         

         if (!OnUserUpdate())
         {
            m_Active = false;
            LOG("User ended application.");
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


   void Application::GetScreenSize(int& width, int& height)
   {
      m_Window->getWindowSize(width, height);
   }

   components::Vec2 Application::GetScreenSize()
   {
      return m_Window->getWindowSize();
   }

   void Application::GetMousePosition(float& mouseX, float& mouseY)
   {
      return m_Window->getMousePosition(mouseX, mouseY);
   }

   components::Vec2 Application::GetMousePosition()
   {
      return m_Window->getMousePosition();
   }

   void Application::GetMousePositionNormalized(float& mouseX, float& mouseY)
   {
      m_Window->getMousePositionNormalized(mouseX, mouseY);
   }

   components::Vec2 Application::GetMousePositionNormalized()
   {
      return m_Window->getMousePositionNormalized();
   }

   void Application::GetScrollWheel()
   {
      float xOffset, yOffset;
      m_Window->getScrollWheel(xOffset, yOffset);
      m_MouseScroll.up =     (yOffset ==  1);
      m_MouseScroll.down =   (yOffset == -1);
      m_MouseScroll.right =  (xOffset ==  1);
      m_MouseScroll.left =   (xOffset == -1);
   }

   void Application::Render(Entity* entityPointer)
   {
      m_Renderer->submitEntity(entityPointer);
   }

   void Application::Render(Sprite* spritePointer)
   {
      m_Renderer->submitSprite(spritePointer);
   }

   void Application::RenderQuad(float xpos, float ypos, float width, float height, float rotation, Texture* texture, const std::vector<components::Vec2> uv)
   {
      m_Renderer->submitEntity(xpos, ypos, width, height, rotation, texture, uv);
   }

   void Application::RenderLine(float x0, float y0, float x1, float y1, unsigned int color, float thickness)
   {
      m_Renderer->submitLine(x0, y0, x1, y1, color, thickness);
   }

   void Application::BootImGui(legit_engine::graphics::Window* window)
   {
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGui::StyleColorsDark();
      ImGui_ImplGlfw_InitForOpenGL(window->getWindowPointer(), true);
      ImGui_ImplOpenGL3_Init();
      ImGui::GetStyle().ScaleAllSizes(2.0f);
   }
}