#pragma once

#include "renderer2d.h"

#include "../Buffers/vertexarray.h"
#include "../../Components/mathlib.h"

namespace legit_engine {
   namespace renderables {
      
      class PixelRenderer2D
      {
      private:

      public:
         PixelRenderer2D();
         ~PixelRenderer2D();

         void PaintPixel(int x, int y, components::Vec4 color, GLint shaderProgram, const char* uniformFloat);

      private:
         void init();
      };

   }
}