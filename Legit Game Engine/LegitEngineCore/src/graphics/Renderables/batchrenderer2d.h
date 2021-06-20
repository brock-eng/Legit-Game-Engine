#pragma once

#include <vector>

#include "sprite.h"
#include "renderer2D.h"
#include "../../game_assets/entity.h"

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
         components::Vec4 s_QuadVertices[4];

      protected:
         buffers::IndexBuffer* m_IBO;
         GLuint m_VBO;
         GLuint m_VAO;
         VertexData* m_Buffer;
         GLsizei m_IndexCount;

         float m_ScreenWidth, m_ScreenHeight;

         std::vector<unsigned int> m_Textures;
      protected:


      public:
         BatchRenderer2D(float screenWidth, float screenHeight);
         ~BatchRenderer2D();
         void UpdateScreenSize(float screenWidth, float screenHeight);
         void begin();

         void submit(const Renderable2D* renderable) override;
         void submitSprite(const Sprite* sprite);
         void submitEntity(const Entity* entity);
         void submitEntity(float x, float y, float width, float height, float rotation, Texture* texture, const std::vector<components::Vec2> uv = Renderable2D::getDefaultUV());
         void submitLine(float x0, float y0, float x1, float y1, unsigned int color, float thickness);

         void end();
         void flush() override;

         inline const bool bufferHasData() { return m_Buffer != nullptr; };

      private: 
         void init();
      };
   }
}