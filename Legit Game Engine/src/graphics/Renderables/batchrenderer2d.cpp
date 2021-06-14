#pragma once

#include "batchrenderer2d.h"

namespace legit_engine {
   namespace renderables {

      BatchRenderer2D::BatchRenderer2D()
      {
         init();
      }

      BatchRenderer2D::~BatchRenderer2D()
      {
         delete m_IBO;
         glDeleteBuffers(1, &m_VBO);
      }

      void BatchRenderer2D::init()
      {
         glGenVertexArrays(1, &m_VAO);
         glGenBuffers(1, &m_VBO);

         glBindVertexArray(m_VAO);
         glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
         glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, 0, GL_DYNAMIC_DRAW);

         glEnableVertexAttribArray(SHADER_VERTEX_INDEX);
         glEnableVertexAttribArray(SHADER_UV_INDEX);
         glEnableVertexAttribArray(SHADER_COLOR_INDEX);

         glVertexAttribPointer(SHADER_VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)0);
         glVertexAttribPointer(SHADER_UV_INDEX, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)offsetof(VertexData, VertexData::texCoords));
         glVertexAttribPointer(SHADER_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, RENDERER_VERTEX_SIZE, (const GLvoid*)offsetof(VertexData, VertexData::color));
         glBindBuffer(GL_ARRAY_BUFFER, 0);

         GLuint* indices = new GLuint[RENDERER_INDICES_SIZE];

         int offset = 0;
         for (int i = 0; i < RENDERER_INDICES_SIZE; i+=6)
         {
            indices[i    ] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;
            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;
            
            offset += 4;
         }

         m_IBO = new buffers::IndexBuffer(indices, RENDERER_INDICES_SIZE);

      }

      void BatchRenderer2D::begin()
      {
         glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
         m_Buffer = (VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
      }

      void BatchRenderer2D::submit(const Renderable2D* renderable)
      {

         const components::Vec2& size = renderable->getSize();
         const components::Vec3& position = renderable->getPosition();
         const components::Vec4& color = renderable->getColor();
         const std::vector<components::Vec2>& uv = renderable->getUV();

         int r = color.x * 255.0f;
         int g = color.y * 255.0f; 
         int b = color.z * 255.0f;
         int a = color.w * 255.0f;

         unsigned int c = a << 24 | b << 16 | g << 8 | r;

         m_Buffer->vertex = position;
         m_Buffer->texCoords = uv[0];
         m_Buffer->color = c;
         m_Buffer++; // moves by sizeof (VertexData) -> 24 bytes

         m_Buffer->vertex = components::Vec3(position.x, position.y + size.y, position.z);
         m_Buffer->texCoords = uv[1];
         m_Buffer->color = c;
         m_Buffer++;

         m_Buffer->vertex = components::Vec3(position.x + size.x, position.y + size.y, position.z);
         m_Buffer->texCoords = uv[2];
         m_Buffer->color = c;
         m_Buffer++;

         m_Buffer->vertex = components::Vec3(position.x + size.x, position.y, position.z);
         m_Buffer->texCoords = uv[3];
         m_Buffer->color = c;
         m_Buffer++;

         m_IndexCount += 6;
      }

      
      void BatchRenderer2D::end()
      {
         glUnmapBuffer(GL_ARRAY_BUFFER);
         glBindBuffer(GL_ARRAY_BUFFER, 0);
      }

      void BatchRenderer2D::flush()
      {
         glBindVertexArray(m_VAO);
         m_IBO->bind();

         glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0);

         m_IBO->unbind();
         glBindVertexArray(0);

         m_IndexCount = 0;
      }
   }
}