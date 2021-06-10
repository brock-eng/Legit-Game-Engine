#pragma once

#include <vector>
#include <glew.h>

#include "buffer.h"

namespace legit_engine {
   namespace buffers {

      class VertexArray
      {
      private:
         GLuint m_ArrayID;
         std::vector<Buffer*> m_Buffers;

      public:
         VertexArray();
         ~VertexArray();
         void AddBuffer(Buffer* buffer, GLuint index);
         void Bind() const;
         void Unbind() const;
      };
} }
