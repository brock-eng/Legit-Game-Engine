#pragma once
#include <iostream>

#include "window.h"
namespace legit_engine {
   namespace graphics {

      bool Window::m_Keys[MAX_KEYS];
      bool Window::m_MouseButtons[MAX_BUTTONS];
      double Window::m_MouseX, Window::m_MouseY;

      // callback declarations
      void GLFWresize_callback(GLFWwindow* window, int m_ScreenWidth, int m_ScreenHeight);
      void GLFWkey_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
      void GLFWmouse_callback(GLFWwindow* window, int button, int action, int mods);
      void GLFWcursor_position_callback(GLFWwindow* window, double xpos, double ypos);

      Window::Window(const char *name, int width, int height)
      {
         m_Name = name;
         m_ScreenWidth = width;
         m_ScreenHeight = height;
         if (!Initialize())
         {
            glfwTerminate();
            std::cout << "GLFW: Initializer Failed.";
         }


         for (int i = 0; i < MAX_KEYS; i++)
         {
            m_Keys[i] = false;
         }

         for (int i = 0; i < MAX_BUTTONS; i++)
         {
            m_MouseButtons[i] = false;
         }
      };

      bool Window::Initialize()
      {
         if (!glfwInit())
         {
            std::cout << "GLFW: glfwInit() error." << std::endl;
            return false;
         }
         m_Window = glfwCreateWindow(m_ScreenWidth, m_ScreenHeight, m_Name, NULL, NULL);
         if (!m_Window)
         {
            std::cout << "GLFW: Window create failed." << std:: endl;
            return false;
         }

         // enable blending (transparency support)
         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
         glEnable(GL_BLEND);

         // setting function callbacks
         glfwMakeContextCurrent(m_Window);
         glfwSetWindowUserPointer(m_Window, this);
         glfwSetWindowSizeCallback(m_Window, GLFWresize_callback);
         glfwSetKeyCallback(m_Window, GLFWkey_callback);
         glfwSetMouseButtonCallback(m_Window, GLFWmouse_callback);
         glfwSetCursorPosCallback(m_Window, GLFWcursor_position_callback);

         if (glewInit() != GLEW_OK)
         {
            std::cout << "GLEW: glewInit() error -> != GLEW_OK" << std::endl;
            return false;
         }

         std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
         return true;
      }

      Window::~Window()
      {
         glfwTerminate();
      };

      void Window::Clear() const
      {
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      }

      void Window::Update() 
      {
         glfwPollEvents();
         glfwSwapBuffers(m_Window);
      };

      bool Window::Closed() const
      {
         return glfwWindowShouldClose(m_Window);
      }

      bool Window::KeyPressed(unsigned int keyCode)
      {
         if (keyCode >= MAX_KEYS)
            return false;
         return m_Keys[keyCode];
      }

      bool Window::MouseButtonsPressed(unsigned int keyCode)
      {
         if (keyCode >= MAX_BUTTONS)
            return false;
         return m_MouseButtons[keyCode];
      }

      /*
         Utilities, getters & setters
      */

      void Window::printXYPos()
      {
         double mouseX, mouseY;
         getMousePosition(mouseX, mouseY);
         std::cout << "x: " << mouseX << "  y: " << mouseY << std::endl;
      }

      void Window::setWindowSize(int width, int height)
      {
         m_ScreenWidth = width;
         m_ScreenHeight = height;
      }

      void Window::getWindowSize(int& width, int& height)
      {
         width = m_ScreenWidth;
         height = m_ScreenHeight;
      }

      components::Vec2 Window::getWindowSize()
      {
         return components::Vec2(m_ScreenWidth, m_ScreenHeight);
      }

      void Window::getMousePosition(double& X, double& Y)
      {
         X = m_MouseX;
         Y = m_MouseY;
      }

      components::Vec2 Window::getMousePosition()
      {
         return components::Vec2(m_MouseX, m_MouseY);
      }

      void Window::getMousePositionNormalized(double& X, double& Y)
      {
         X = m_MouseX * 2.0f / m_ScreenWidth - 1;
         Y = m_MouseY * -2.0f / m_ScreenHeight + 1;
      }

      components::Vec2 Window::getMousePositionNormalized()
      {
         return components::Vec2(m_MouseX, m_MouseY * -1.0f + m_ScreenHeight);
      }
      
      GLFWwindow* Window::getWindowPointer()
      {
         return m_Window;
      }

      /*
         Callback functions
      */
         
      void GLFWkey_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
      {
         Window* winD = (Window*)glfwGetWindowUserPointer(window);
         winD->m_Keys[key] = action != GLFW_RELEASE;
      }

      void GLFWmouse_callback(GLFWwindow* window, int button, int action, int mods)
      {
         Window* winD = (Window*)glfwGetWindowUserPointer(window);
         winD->m_MouseButtons[button] = (action != GLFW_RELEASE);
      }

      void GLFWcursor_position_callback(GLFWwindow* window, double xpos, double ypos)
      {
         Window* winD = (Window*)glfwGetWindowUserPointer(window);
         //glfwGetCursorPos(window, &xpos, &ypos);
         winD->m_MouseX = xpos;
         winD->m_MouseY = ypos;
      }

      void GLFWresize_callback(GLFWwindow* window, int width, int height)
      {
         Window* winD = (Window*)glfwGetWindowUserPointer(window);
         winD->setWindowSize(width, height);
         glViewport(0, 0, width, height);
      }
} }