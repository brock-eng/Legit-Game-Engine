#pragma once

#include<fstream>
#include <iostream>

#include "texture.h"

namespace legit_engine {
   namespace renderables {
       
      unsigned int Texture::m_GlobalTextureCount{ 0 };

      Texture::Texture(const std::string& path)  
         : m_Path(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
      {
         stbi_set_flip_vertically_on_load(1);
         m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);
         glGenTextures(1, &m_ID);
         glBindTexture(GL_TEXTURE_2D, m_ID);

         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
         glBindTexture(GL_TEXTURE_2D, 0);

         if (m_LocalBuffer)
            stbi_image_free(m_LocalBuffer);
         else
         {
            std::string reason = stbi_failure_reason();
            std::cout << "Error: Shader could not load texture @ '" << path << "'. Reason: " << reason << std::endl;
         }
      }
      Texture::~Texture()
      {
         glDeleteTextures(1, &m_ID);
      }

      void Texture::bind(unsigned int slot)
      {
         glActiveTexture(GL_TEXTURE0 + slot);
         glBindTexture(GL_TEXTURE_2D, m_ID);
      }

      void Texture::unbind() const
      {
         glBindTexture(GL_TEXTURE_2D, 0);
      }
} }