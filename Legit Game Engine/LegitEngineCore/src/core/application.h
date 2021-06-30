#pragma once

#include <glew.h>
#include <glfw3.h>

#include <iostream>
#include <stdio.h>

#include "../utils/general_utils.h"

#include "../Components/mathlib.h"
#include "../utils/debug.h"

#include "../utils/ImGui/imgui.h"
#include "../utils/ImGui/imgui_impl_glfw.h"
#include "../utils/ImGui/imgui_impl_opengl3.h"

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
      bool m_Constructed = false;

      bool m_KeysNewState[MAX_KEYS]     = { 0 };
      bool m_KeysOldState[MAX_KEYS]     = { 0 };
      bool m_MouseOldState[MAX_BUTTONS] = { 0 };
      bool m_MouseNewState[MAX_BUTTONS] = { 0 };

   public:
      // Main key state container.   
      // m_Keys and m_Mouse can be probed for their current state
      // by using m_Keys[EL_KEY]
      // Refer to input.h for the complete key list
      struct sKeyStates
      {
         bool bPressed;
         bool bReleased;
         bool bHeld;
      } m_Keys[MAX_KEYS], m_Mouse[MAX_BUTTONS];

      // This is intended to be probed for the pointers x and y coordinates
      struct mouseCoords
      {
         float x;
         float y;
      } m_MousePosition;

      // Mouse scroll info container
      struct scrollState
      {
         bool up;
         bool down;
         bool right;
         bool left;
      } m_MouseScroll;
      
      float m_ScreenWidth, m_ScreenHeight;


   public:
      Application();//const char* name, unsigned int screenWidth, unsigned int screenHeight);
      ~Application();

      virtual bool OnUserCreate() = 0;
      virtual bool OnUserUpdate() = 0;

      void Construct(const char* name, unsigned int screenWidth, unsigned int screenHeight);
      void Start();

   private:
      void ApplicationThread();

   public:
      void GetScreenSize(int& width, int& height);
      components::Vec2 GetScreenSize();
      void GetMousePosition(float& mouseX, float& mouseY);
      components::Vec2 GetMousePosition();
      void GetMousePositionNormalized(float& mouseX, float& mouseY);
      components::Vec2 GetMousePositionNormalized();
      inline const void SetFullScreen() const { m_Window->setFullscreen(); }
      void GetScrollWheel();

      void Render(Entity* entityPointer);
      void Render(renderables::Sprite* spritePointer);
      void RenderQuad(float xpos, float ypos, float width, float height, float rotation, renderables::Texture* texture = nullptr, const std::vector<components::Vec2> uv = renderables::Renderable2D::getDefaultUV());
      void RenderLine(float x0, float y0, float x1, float y1, unsigned int color, float thickness);

   private:
      void BootImGui(legit_engine::graphics::Window* window);
      };

}