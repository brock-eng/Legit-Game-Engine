#pragma once

#include <glew.h>

namespace legit_engine {
   namespace buffers {

      class IndexBuffer
      {
      private:
         GLuint m_BufferID;
         GLuint m_Count;

      public:
         // ** max indices is 65535, for 4e10 indices use GLuint for array
         IndexBuffer(GLushort* data, GLsizei count);

         void bind() const;
         void unbind() const;

         GLuint getCount() const;
      };
   }
}