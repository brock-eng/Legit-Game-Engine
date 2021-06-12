#pragma once

#include "../Buffers/buffer.h"
#include "../Buffers/indexbuffer.h"
#include "../Buffers/vertexarray.h"

#include "../Shaders/shaders.h"

#include "../../Components/Sys.h"

namespace legit_engine {
   namespace renderables {

      struct VertexData
      {
         components::Vec3 vertex;
         components::Vec4 color;

      };

      class Renderable2D
      {
      protected:
         components::Vec2 m_Size;
         components::Vec3 m_Position;
         components::Vec4 m_Color;

      protected:
         Renderable2D(components::Vec3 position, components::Vec2 size, components::Vec4 color)
            : m_Position(position), m_Size(size), m_Color(color) {}

      public:
         
   /*      const buffers::VertexArray* getVAO() const 
         { 
            return m_VertexArray;
         }

         const buffers::IndexBuffer* getIBO() const
         {
            return m_IndexBuffer;
         }*/

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