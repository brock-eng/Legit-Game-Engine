#pragma once
/*
#include "static_sprite.h"

namespace legit_engine {
   namespace graphics {

      StaticSprite::StaticSprite(float x, float y, float width, float height, const components::Vec4& color, shaders::Shader* shader, ) 
         : Renderable2D(components::Vec3(x, y, 0), components::Vec2(width, height), color, textureID), m_Shader((shaders::Shader*)shader)
      {
         m_VertexArray = new buffers::VertexArray();

         GLfloat vertices[] =
         {
            // rect construction
            0, 0, 0,
            0, height, 0,
            width, height, 0,
            width, 0, 0
         };

         GLfloat colors[] =
         {
            color.x, color.y, color.z, color.w,
            color.x, color.y, color.z, color.w,
            color.x, color.y, color.z, color.w,
            color.x, color.y, color.z, color.w
         };

         m_VertexArray->AddBuffer(new buffers::Buffer(vertices, 4 * 3, 3), 0);
         m_VertexArray->AddBuffer(new buffers::Buffer(colors, 4 * 4, 4), 1);


         m_IndexBuffer = new buffers::IndexBuffer(new GLushort[]{ 0, 1, 2, 2, 3, 0 }, 6);

      };

      StaticSprite::~StaticSprite()
      {
         delete m_VertexArray;
         delete m_IndexBuffer;
      }

   }
}
*/
