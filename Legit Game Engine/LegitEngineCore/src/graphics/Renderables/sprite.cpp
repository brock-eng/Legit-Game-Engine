#pragma once

#include "sprite.h"

namespace legit_engine {
   namespace renderables {

      Sprite::Sprite()
         : Renderable2D(components::Vec3(0, 0, 0), components::Vec2(0, 0), 0, 0)
      {}

      Sprite::Sprite(float x, float y, float width, float height, const components::Vec4& color, shaders::Shader* shader)
         : Renderable2D(components::Vec3(x, y, 0), components::Vec2(width, height), color, 0), m_Shader(shader)
      {}

      Sprite::Sprite(float x, float y, float width, float height, renderables::Texture* texture, shaders::Shader* shader)
         : Renderable2D(components::Vec3(x, y, 0), components::Vec2(width, height), texture), m_Shader(shader)
      {}
      

      // Sets the Renderable2D UV values used for selecting the area of a texture to be rendered.
      // Useful for texture maps with multiple sprites/textures per single image file.
      // Coordinates are relative and the bottom left corner of a texture is treated as (0, 0) 
      // with the top right being (1, 1)
      void Sprite::setUV(float x, float y, float width, float height)
      {
         m_UV[0].x = x;
         m_UV[0].y = y;
         m_UV[1].x = x;
         m_UV[1].y = y + height;
         m_UV[2].x = x + width;
         m_UV[2].y = y + height;
         m_UV[3].x = x + width;
         m_UV[3].y = y;
      }

   }
}