#pragma once

#include "../../utils/StbImage/stb_image_include.h"

#include <string>
#include <glew.h>
#include <glfw3.h>

namespace legit_engine {
   namespace renderables {

      class Texture
      {
      private:
         unsigned int m_RendererID;
         std::string m_Path;
         unsigned char* m_LocalBuffer;
         int m_Width, m_Height, m_BPP;
      public: 
         Texture(const std::string& path);
         ~Texture();
         void bind(unsigned int slot = 0);
         void unbind() const;

         inline const unsigned int getWidth() { return m_Width; }
         inline const unsigned int getHeight() { return m_Height; }
      private:
         GLuint load();


      };
} }