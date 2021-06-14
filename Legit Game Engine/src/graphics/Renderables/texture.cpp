#pragma once

#include<fstream>
#include <iostream>

#include "texture.h"

namespace legit_engine {
   namespace renderables {

      Texture::Texture(const std::string& path)
         : m_RendererID(0), m_Path(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
      {
         //std::fstream stream(path);

         //if (stream.fail())
         {
         //   std::cout << "Error: Shader could not load texture @ '" << path << "'" << std::endl;
         }

         stbi_set_flip_vertically_on_load(1);
         m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);
         glGenTextures(1, &m_RendererID);
         glBindTexture(GL_TEXTURE_2D, m_RendererID);

         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
         glBindTexture(GL_TEXTURE_2D, 0);

         if (m_LocalBuffer)
            stbi_image_free(m_LocalBuffer);
         else
            std::cout << stbi_failure_reason() << std::endl;

      }
      Texture::~Texture()
      {
         glDeleteTextures(1, &m_RendererID);
      }

      void Texture::bind(unsigned int slot)
      {
         glActiveTexture(GL_TEXTURE0 + slot);
         glBindTexture(GL_TEXTURE_2D, m_RendererID);
      }

      void Texture::unbind() const
      {
         glBindTexture(GL_TEXTURE_2D, 0);
      }
} }