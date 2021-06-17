#pragma once

#include <vector>

#include "sprite.h"
#include "renderer2D.h"
#include "../Buffers/vertexarray.h"

#include "../../Components/Sys.h"

namespace legit_engine {
   namespace renderables {

#define RENDERER_MAX_SPRITES     60000
#define RENDERER_VERTEX_SIZE     sizeof(VertexData)
#define RENDERER_SPRITE_SIZE     RENDERER_VERTEX_SIZE * 4
#define RENDERER_BUFFER_SIZE     RENDERER_SPRITE_SIZE * RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE    RENDERER_MAX_SPRITES * 6
               
#define SHADER_VERTEX_INDEX 0
#define SHADER_UV_INDEX 1
#define SHADER_COLOR_INDEX 2
#define SHADER_TEXTURE_INDEX 3

#define MAX_TEXTURES 32

      class BatchRenderer2D : public Renderer2D
      {
      private:
         buffers::IndexBuffer* m_IBO;
         GLuint m_VBO;
         GLuint m_VAO;
         VertexData* m_Buffer;
         GLsizei m_IndexCount;

         std::vector<unsigned int> m_Textures;
      protected:


      public:
         BatchRenderer2D();
         ~BatchRenderer2D();
         void begin();
         void submit(const Renderable2D* renderable) override;
         void submitSprite(const Sprite* sprite);
         void end();
         void flush() override;

      private: 
         void init();
      };
   }
}
