#pragma once

#include "sprite.h"

namespace legit_engine {
   namespace graphics {


      Sprite::Sprite(float x, float y, float width, float height, const components::Vec4& color)
         : Renderable2D(components::Vec3(x, y, 0), components::Vec2(width, height), color)
      {}
 
   }
}