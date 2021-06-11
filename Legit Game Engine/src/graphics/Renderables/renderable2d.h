#pragma once

#include "../Buffers/buffer.h"
#include "../Buffers/indexbuffer.h"
#include "../Buffers/vertexarray.h"

#include "../Shaders/shaders.h"

#include "../../Components/Sys.h"

namespace legit_engine {
   namespace renderables {

      class Renderable2D
      {
      protected:
         components::Vec2 m_Size;
         components::Vec3 m_Position;
         components::Vec4 m_Color;

         buffers::VertexArray* m_VertexArray;
         buffers::IndexBuffer* m_IndexBuffer;
         
         shaders::Shader* m_Shader;

      protected:
         Renderable2D(components::Vec3 position, components::Vec2 size, components::Vec4 color, shaders::Shader* shader)
            : m_Position(position), m_Size(size), m_Color(color), m_Shader(shader)
         {
            m_VertexArray = new buffers::VertexArray();

            GLfloat vertices[] =
            {
               0, 0, 0,
               0, size.y, 0,
               size.x, size.y, 0,
               size.x, 0, 0
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
         }

      public:

         shaders::Shader* getShader() const
         {
            return m_Shader;
         }

         const buffers::VertexArray* getVAO() const 
         { 
            return m_VertexArray;
         }

         const buffers::IndexBuffer* getIBO() const
         {
            return m_IndexBuffer;
         }

         const components::Vec2& getSize() const
         {
            return m_Size;
         }
         
         const components::Vec3& getPosition() const
         {
            return m_Position;
         }
         
         const components::Vec4& getColor() const
         {
            return m_Color;
         }
      };
} }