#pragma once

#include <glew.h>
#include "renderer2d.h"
#include "../../Components/Sys.h"
#include "../static_sprite.h"

#include <deque>

namespace legit_engine {
   namespace renderables {

      class Simple2DRenderer : public Renderer2D
      {
      private:
         std::deque<const graphics::StaticSprite*> m_RenderQueue;
      public:
         void submit(const Renderable2D* renderable) override;
         void flush() override;
      };
} }
