#pragma once

#include "Renderables/renderable2d.h"

namespace legit_engine {
   namespace graphics {

      class Sprite : public renderables:: Renderable2D
      {

      public:
         Sprite(float x, float y, float width, float height, const components::Vec4& color);
      };
} }