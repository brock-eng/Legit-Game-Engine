#pragma once

#include "../Renderables/renderable2d.h"
#include "../Renderables/texture.h"

namespace legit_engine {
   namespace renderables {

      class Sprite : public renderables:: Renderable2D
      {
      private:
         shaders::Shader* m_Shader;
      public:
         Sprite(float x, float y, float width, float height, const components::Vec4& color, shaders::Shader* shader, short textureID = -1);
         ~Sprite();

         void setUV(float x, float y, float width, float height);

         inline shaders::Shader& getShader() const { return *m_Shader; }
         inline const components::Vec2& getSize() const { return m_Size; }
         inline const components::Vec3& getPosition() const { return m_Position; }
         inline const components::Vec4& getColor() const { return m_Color; }
         inline const std::vector<components::Vec2>& getUV() const { return m_UV; }
         inline const short getTextureID() const { return m_TextureID; }
         inline components::Vec3& getPositionM() { return m_Position; }
         inline void setPosition(float x, float y, float z)
         {
            m_Position.x = x;
            m_Position.y = y;
            m_Position.z = z;
         }
      };
} }