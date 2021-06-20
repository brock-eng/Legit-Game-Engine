#pragma once

#include "simple2drenderer.h"

namespace legit_engine {
   namespace renderables {

      void Simple2DRenderer::submit(const Renderable2D* renderable)
      {
         m_RenderQueue.push_back((graphics::StaticSprite*) renderable);
      }
      
      void Simple2DRenderer::flush() 
      {
         while (!m_RenderQueue.empty())
         {
            const graphics::StaticSprite* renderable = m_RenderQueue.front();
            renderable->getVAO()->bind();
            renderable->getIBO()->bind();
            renderable->getShader().setUniformMat4("ml_matrix", components::mat4::translation(renderable->getPosition()));
            glDrawElements(GL_TRIANGLES, renderable->getIBO()->getCount(), GL_UNSIGNED_SHORT,  nullptr);
            renderable->getIBO()->bind();
            renderable->getVAO()->unbind();
         }

      };
   }
}