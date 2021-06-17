#pragma once


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>

#include <glew.h>
#include <glfw3.h>

#include <random>

#include "../utils/ImGui/imgui.h"
#include "../utils/ImGui/imgui_impl_glfw.h"
#include "../utils/ImGui/imgui_impl_opengl3.h"

#include "../Components/Sys.h"
#include "../utils/debug.h"

#include "../helpers.h"

#if 0
#define INCLUDE_ALL_GRAPHICS_COMPONENTS
#endif

#include "../graphics/graphics_components.h"


namespace legit_engine {

   class Application
   {
   private:
      // static Application* s_Instance;

   public:
      graphics::Window* m_Window;
      renderables::BatchRenderer2D* m_Renderer;
      shaders::Shader* m_Shader;
      utils::DebugUtil* m_DebugAPI;

   public:
      Application(const char* name, unsigned int screenWidth, unsigned int screenHeight);
      ~Application();

      virtual void OnUserCreate() = 0;
      virtual void OnUserUpdate() = 0;

      void Start();

   private:
      void ApplicationThread();

   public:
      void getScreenSize(int& width, int& height);
      components::Vec2 getScreenSize();
      void getMousePosition(double& mouseX, double& mouseY);
      components::Vec2 getMousePosition();
      void getMousePositionNormalized(double& mouseX, double& mouseY);
      components::Vec2 getMousePositionNormalized();

      };

}