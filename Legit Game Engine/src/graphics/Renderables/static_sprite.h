#pragma once

#include "../Renderables/renderable2d.h"

namespace legit_engine {
   namespace graphics {

      class StaticSprite : public renderables::Renderable2D
      {
      private:
         components::Vec2 m_Size;
         components::Vec3 m_Position;
         components::Vec4 m_Color;
         
         shaders::Shader* m_Shader;
         buffers::VertexArray* m_VertexArray;
         buffers::IndexBuffer* m_IndexBuffer;
      public:
         StaticSprite(float x, float y, float width, float height, const components::Vec4& color, shaders::Shader* shader, short textureID = -1);
         ~StaticSprite();

         inline shaders::Shader& getShader() const { return *m_Shader; }
         inline buffers::VertexArray* getVAO() const { return m_VertexArray; }
         inline buffers::IndexBuffer* getIBO() const { return m_IndexBuffer; }
         };
      }
}
