#pragma once

#include "indexbuffer.h"

namespace legit_engine {
   namespace buffers {

      IndexBuffer::IndexBuffer(GLushort* data, GLsizei count)
      {
         m_Count = count;

         glGenBuffers(1, &m_BufferID);
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
         glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLushort), data, GL_STATIC_DRAW);
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      }

      void IndexBuffer::Bind() const
      {
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
      }
      void IndexBuffer::Unbind() const
      {
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

      }

      GLuint IndexBuffer::GetCount() const { return m_Count; }

   }
}