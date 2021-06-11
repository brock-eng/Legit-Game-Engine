#pragma once

#include <glew.h>

namespace legit_engine {
   namespace buffers {

      class Buffer
      {
      private:
         GLuint m_BufferID;
         GLuint m_ComponentCount;

      public:
         Buffer(GLfloat* data, GLsizei count, GLuint componentCount);

         void bind() const;
         void unbind() const;

         GLuint GetComponentCount() const;
      };
} }