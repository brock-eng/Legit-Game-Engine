#pragma once

#include "Shaders.h"

namespace legit_engine {
   namespace Shaders {

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
         m_shaderID = CreateShader(filePaths.VertexSource, filePaths.FragmentSource);
      }

      Shader::~Shader()
      {
         glDeleteProgram(m_shaderID);
      }

      void Shader::Enable()
      {
         glUseProgram(m_shaderID);
      }

      void Shader::Disable()
      {
         glUseProgram(0);
      }

      static ShaderSources ParseShader(const std::string& filepath) 
      {
         std::fstream stream(filepath);

         enum class ShaderType
         {
            NONE = -1, VERTEX = 0, FRAGMENT = 1
         };

         std::string line;
         std::stringstream ss[2];

         ShaderType type = ShaderType::NONE;
         try
         {
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
         }
         catch (int error)
         {
            std::cout << "Error on reading Shaders file. Error No.: " << error;
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