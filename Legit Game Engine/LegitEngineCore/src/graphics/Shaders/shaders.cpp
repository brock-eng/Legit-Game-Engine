#pragma once

#include "shaders.h"

namespace legit_engine {
   namespace shaders {

      struct ShaderSources
      {
         std::string VertexSource;
         std::string FragmentSource;
      };

      static ShaderSources ParseShader(const std::string& filepath);
      static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
      static unsigned int CompileShader(unsigned int type, const std::string& source);

      Shader::Shader(const char* filepath)
      {
         m_filepath = filepath;
         ShaderSources filePaths = ParseShader(m_filepath);
         m_ShaderID = CreateShader(filePaths.VertexSource, filePaths.FragmentSource);
      }

      Shader::~Shader()
      {
         glDeleteProgram(m_ShaderID);
      }

      void Shader::Enable() const
      {
         glUseProgram(m_ShaderID);
      }

      void Shader::Disable() const
      {
         glUseProgram(0);
      }

      GLint Shader::GetUniformLocation(const GLchar* name)
      {
         return glGetUniformLocation(m_ShaderID, name);
      }

      void Shader::setUniform1f(const GLchar* name, float value)
      {
         glUniform1f(GetUniformLocation(name), value);
      }

      void Shader::setUniform1i(const GLchar* name, int value)
      {
         glUniform1i(GetUniformLocation(name), value);
      }

      void Shader::setUniform2f(const GLchar* name, const components::Vec2& vector2D)
      {
         glUniform2f(GetUniformLocation(name), vector2D.x, vector2D.y);
      }

      void Shader::setUniform3f(const GLchar* name, const components::Vec3& vector3D)
      {
         glUniform3f(GetUniformLocation(name), vector3D.x, vector3D.y, vector3D.z);
      }

      void Shader::setUniform4f(const GLchar* name, const components::Vec4& vector4D)
      {
         glUniform4f(GetUniformLocation(name), vector4D.x, vector4D.y, vector4D.z, vector4D.w);
      }

      void Shader::setUniformMat4(const GLchar* name, const components::mat4& matrix4X4)
      {
         glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, matrix4X4.elements);
      }

      static ShaderSources ParseShader(const std::string& filepath) 
      {
         std::fstream stream(filepath);

         if (stream.fail())
         {
            std::cout << "Error: Shader file '" << filepath << "' could not be opened." << std::endl;
         }

         enum class ShaderType
         {
            NONE = -1, VERTEX = 0, FRAGMENT = 1
         };

         std::string line;
         std::stringstream ss[2];

         ShaderType type = ShaderType::NONE;

         while (std::getline(stream, line))
         {
            if (line.find("#shader") != std::string::npos)
            {
               if (line.find("vertex") != std::string::npos)
                  type = ShaderType::VERTEX;
               else if (line.find("fragment") != std::string::npos)
                  type = ShaderType::FRAGMENT;
            }
            else
            {
               ss[(int)type] << line << '\n';
            }
         }

         return { ss[0].str(), ss[1].str() };
      }

      static unsigned int CompileShader(unsigned int type, const std::string& source)
      {
         unsigned int id = glCreateShader(type);
         const char* src = source.c_str();
         glShaderSource(id, 1, &src, nullptr);
         glCompileShader(id);

         int result;
         glGetShaderiv(id, GL_COMPILE_STATUS, &result);

         // error handling
         if (result == GL_FALSE)
         {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)alloca(length * sizeof(char));
            glGetShaderInfoLog(id, length, &length, message);
            std::cout << "Failed to compile shader: " << std::endl;
            std::cout << message << std::endl;
            glDeleteShader(id);
            return 0;
         }
         return id;
      }

      static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
      {
         unsigned int program = glCreateProgram();
         unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
         unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

         glAttachShader(program, vs);
         glAttachShader(program, fs);
         glLinkProgram(program);
         glValidateProgram(program);

         glDeleteShader(vs);
         glDeleteShader(fs);

         return program;
      }
   }
}