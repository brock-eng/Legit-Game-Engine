#pragma once

#include "sprite.h"

namespace legit_engine {
   namespace renderables {


      Sprite::Sprite(float x, float y, float width, float height, const components::Vec4& color, shaders::Shader* shader, short textureID)
         : Renderable2D(components::Vec3(x, y, 0), components::Vec2(width, height), color, textureID) , m_Shader(shader)
      {
      }

      void Sprite::setUV(float x, float y, float width, float height)
      {
         m_UV[0].x = x;
         m_UV[0].y = y;
         m_UV[1].x = x + width;
         m_UV[1].y = y + height;
      }

   }
}