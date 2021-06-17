#pragma once

#include "../../utils/StbImage/stb_image_include.h"

#include <string>
#include <glew.h>
#include <glfw3.h>

namespace legit_engine {
   namespace renderables {


      class Texture
      {
      public:
         static unsigned int m_GlobalTextureCount;
      private:

         std::string m_Path;
         unsigned char* m_LocalBuffer;
         int m_Width, m_Height, m_BPP;

         unsigned int m_ID;
      public: 
         Texture(const std::string& path);
         ~Texture();
         void bind(unsigned int slot = 0);
         void unbind() const;

         static void setGlobalTextureCount(unsigned int count) { m_GlobalTextureCount = count; }
         inline const unsigned int getID() const { return m_ID; }
         inline const unsigned int getWidth() const { return m_Width; }
         inline const unsigned int getHeight() const { return m_Height; }
      private:
         GLuint load();


      };
} }