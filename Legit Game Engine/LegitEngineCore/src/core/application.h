#pragma once

#include <glew.h>
#include <glfw3.h>

#include <iostream>
#include <stdio.h>

#include "../Components/Sys.h"
#include "../utils/debug.h"


#include "../utils/ImGui/imgui.h"
#include "../utils/ImGui/imgui_impl_glfw.h"
#include "../utils/ImGui/imgui_impl_opengl3.h"

#if 0 // for debugging
#define INCLUDE_ALL_GRAPHICS_COMPONENTS
#endif

#include "../graphics/graphics_components.h"

#include "../input/input.h"

#define MAX_KEYS  1024
#define MAX_BUTTONS 32

namespace legit_engine {

   class Application
   {
   protected:
      graphics::Window* m_Window;
      renderables::BatchRenderer2D* m_Renderer;
      shaders::Shader* m_Shader;
      utils::DebugUtil* m_DebugAPI;
     
      bool m_Active;

      bool m_KeysNewState[MAX_KEYS] = { 0 };
      bool m_KeysOldState[MAX_KEYS] = { 0 };
      bool m_MouseOldState[MAX_BUTTONS] = { 0 };
      bool m_MouseNewState[MAX_BUTTONS] = { 0 };

   public:
      // This is main keystate container.   
      // m_Keys and m_Mouse can be probed for their current state
      // by using m_Keys[EL_KEY]
      // Refer to input.h for the complete key list
      struct sKeyStates
      {
         bool bPressed;
         bool bReleased;
         bool bHeld;
      } m_Keys[MAX_KEYS], m_Mouse[MAX_BUTTONS];

      // Similar to sKeyStates, this is intended to be probed for the pointers x and y coords
      struct mouseCoords
      {
         float x;
         float y;
      } m_MousePosition;
      
      float m_ScreenWidth, m_ScreenHeight;


   public:
      Application(const char* name, unsigned int screenWidth, unsigned int screenHeight);
      ~Application();

      virtual bool OnUserCreate() = 0;
      virtual bool OnUserUpdate() = 0;

      void Start();

   private:
      void ApplicationThread();

   public:
      void getScreenSize(int& width, int& height);
      components::Vec2 getScreenSize();
      void getMousePosition(float& mouseX, float& mouseY);
      components::Vec2 getMousePosition();
      void getMousePositionNormalized(float& mouseX, float& mouseY);
      components::Vec2 getMousePositionNormalized();

   private:
      void bootGui(legit_engine::graphics::Window* window);
      };

}