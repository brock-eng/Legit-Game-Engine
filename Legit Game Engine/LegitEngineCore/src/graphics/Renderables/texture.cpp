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

#if 1
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


#elif 0
         stbi_set_flip_vertically_on_load(1);
         m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 3);
         glGenTextures(1, &m_ID);
         glBindTexture(GL_TEXTURE_2D, m_ID);

         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_LocalBuffer);
         glBindTexture(GL_TEXTURE_2D, 0);
#else
         // load and create a texture 
// -------------------------
         unsigned int texture;
         glGenTextures(1, &texture);
         glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
         // set the texture wrapping parameters
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
         // set texture filtering parameters
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         // load image, create texture and generate mipmaps
         int width, height, nrChannels;
         // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
         unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 3);

#endif
         if (m_LocalBuffer)
         {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_LocalBuffer);
         }
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