#pragma once

#include <glew.h>
#include "../../Components/Sys.h"

#include "renderable2d.h"

namespace legit_engine {
   namespace renderables {

      class Renderer2D
      {
      protected:
         virtual void submit(const Renderable2D* renderable) = 0;
         virtual void flush() = 0;
      };
   }
}

