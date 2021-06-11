#pragma once

#include <glew.h>
#include "renderer2d.h"
#include "../../Components/Sys.h"

#include <deque>

namespace legit_engine {
   namespace renderables {

      class Simple2DRenderer : public Renderer2d
      {
      private:
         std::deque<const Renderable2D*> m_RenderQueue;
      public:
         void submit(const Renderable2D* renderable) override;
         void flush() override;
      };
} }
