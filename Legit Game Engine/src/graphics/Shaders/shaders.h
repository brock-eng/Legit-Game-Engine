#pragma once

#include <glew.h>
#include <glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


namespace legit_engine {
   namespace Shaders {
      
      struct ShaderSources;

      class Shader
      {
      private:
         const char* m_filepath;
         unsigned int m_shaderID;
      public:
         Shader(const char* filepath);
         ~Shader();
         void Enable();
         void Disable();
      private:
         static friend ShaderSources ParseShader(const std::string& filepath);
         static friend unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
         static friend unsigned int CompileShader(unsigned int type, const std::string& source);
      };
} }