#pragma once

#include <glew.h>
#include <glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "../../Components/Sys.h"

namespace legit_engine {
   namespace shaders {
      
      struct ShaderSources;

      class Shader
      {
      private:
         const char* m_filepath;
         unsigned int m_ShaderID;
      public:
         Shader(const char* filepath);
         ~Shader();

         void setUniform1f(const GLchar* name, float value);
         void setUniform1i(const GLchar* name, int value);
         void setUniform2f(const GLchar* name, const components::Vec2& matrix);
         void setUniform3f(const GLchar* name, const components::Vec3& matrix);
         void setUniform4f(const GLchar* name, const components::Vec4& matrix);
         void setUniformMat4(const GLchar* name, const components::mat4& matrix);

         void Enable() const;
         void Disable() const;
      private:
         static friend ShaderSources ParseShader(const std::string& filepath);
         static friend unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
         static friend unsigned int CompileShader(unsigned int type, const std::string& source);

         GLint GetUniformLocation(const GLchar* name);
      };
} }