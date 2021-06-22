#pragma once

#include "pixelrenderer2d.h"

namespace legit_engine {
   namespace renderables {


      PixelRenderer2D::PixelRenderer2D()
      {
         init();
      }

      void PixelRenderer2D::init()
      {
         float vertices[] = {
               -1.0f, -1.0f, 0.0f,
                1.0f, -1.0f, 0.0f,
                1.0f, 1.0f, 0.0f,
               -1.0f, -1.0f, 0.0f,
                1.0f, 1.0f, 0.0f,
               -1.0f, 1.0f, 0.0f
         };

         unsigned VAO;
         glGenVertexArrays(1, &VAO);
         glBindVertexArray(VAO);

         unsigned int VBO;
         glGenBuffers(1, &VBO);
         glBindBuffer(GL_ARRAY_BUFFER, VBO);
         glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

         glVertexAttribPointer
         (
            0,					      // Location = 0
            3,					      // Size of vertex attribute
            GL_FLOAT,			   // Type of the data
            GL_FALSE,			   // Normalize data
            3 * sizeof(float),	// Stride
            (void*)0			      // Offset
         );

         glEnableVertexAttribArray(0);
         glBindVertexArray(0);
      }

      void PixelRenderer2D::PaintPixel(int x, int y, components::Vec4 color, GLint shaderProgram, const char* uniformFloat)
      {
         GLint transformLoc = glGetUniformLocation(shaderProgram, uniformFloat);
         glUniform4f(transformLoc, color.x, color.y, color.z, color.w);

         glEnable(GL_SCISSOR_TEST);
         glScissor(x, y, 1, 1);
         glDrawArrays(GL_TRIANGLES, 0, 6);
         glDisable(GL_SCISSOR_TEST);
      }
   }
}