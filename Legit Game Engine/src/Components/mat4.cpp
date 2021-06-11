#pragma once

#include <math.h>

#include "mat4.h"
#include "Sys.h"

namespace legit_engine {
   namespace components {

      // initializes a matrix of zeros
      mat4::mat4()
      {
         for (int i = 0; i < SIZE * SIZE; i++)
            elements[i] = 0.0f;
      }

      // initializes a diagonal matrix of input number
      mat4::mat4(float diagonal)
      {
         for (int i = 0; i < SIZE * SIZE; i++)
            elements[i] = 0.0f;
         for (int i = 0; i < SIZE; i++)
         {
            elements[i * SIZE] = diagonal;
         }
      }

      // identity matrix
      static mat4 identity()
      {
         return mat4(1.0f);
      }

      // standard matrix multiplication
      mat4& mat4::multiply(const mat4& m)
      {
         // r -> row ; c -> collumn
         for (int r = 0; r < 4; r++)
         {
            for (int c = 0; c < 4; c++)
            {
               float sum = 0.0f;
               for (int i = 0; i < 4; i++)
               {
                  sum += elements[r + i * 4] * m.elements[i + r * 4];
               }
               elements[c + r * 4] = sum;
            }
         }
         return *this;
      }
 
      mat4 operator*(mat4 left, const mat4& right)
      {
         return left.multiply(right);
      }
      
      mat4& mat4::operator*=(const mat4& right)
      {
         return multiply(right);
      }

      // returns orthographic projection matrix with 6-tuple input
      mat4 mat4::orthographic(float left, float right, float top, float bottom, float near, float far)
      {
         mat4 result;

         result.elements[0 + 0 * SIZE] = 2.0f / (right - left);
         result.elements[1 + 2 * SIZE] = 2.0f / (top - bottom);
         result.elements[2 + 2 * SIZE] = 2.0f / (near - far);

         result.elements[0 + 3 * SIZE] = (left + right) / (left - right);
         result.elements[1 + 3 * SIZE] = (bottom + top) / (bottom - top);
         result.elements[2 + 3 * SIZE] = (far + near)   / (far - near);

         return result;
      }

      // returns perspective matrix
      mat4 mat4::perspective(float fov, float aspect, float near, float far)
      {
         mat4 result;

         float deg = std::tan(fov * PI / (2 * 180));
         float res = (near + far) / (near - far);

         result.elements[0 + 0 * SIZE] = 1 / (aspect * deg);
         result.elements[1 + 1 * SIZE] = 1 / (deg);
         result.elements[2 + 2 * SIZE] = res;
         result.elements[3 + 2 * SIZE] = -1.0f;
         result.elements[2 + 3 * SIZE] = 2.0f * far * near / (far - near);

         return result;
      }

      // returns a rotation matrix that applies a specified angle roation (in degrees)
      // around a specified 3D axis (defined via a Vec3)
      mat4 mat4::rotation(float angle, const Vec3& axis)
      {
         mat4 result(1.0f);

         angle *= PI / 180.0f;
         float c = cos(angle);
         float s = sin(angle);
         float omc = 1.0f - c;

         float x = axis.x;
         float y = axis.y;
         float z = axis.z;

         result.elements[0 + 0 * SIZE] = x * omc + c;
         result.elements[1 + 0 * SIZE] = y * x * omc + z * s;
         result.elements[2 + 0 * SIZE] = x * z * omc - y *s;

         result.elements[0 + 1 * SIZE] = y * x * omc - z * s;
         result.elements[1 + 1 * SIZE] = y * omc + c;
         result.elements[2 + 1 * SIZE] = y * z * omc + x * s;
         
         result.elements[0 + 1 * SIZE] = x * z * omc + y * s;
         result.elements[1 + 1 * SIZE] = y * z * omc - x * s;
         result.elements[2 + 1 * SIZE] = z * omc + c;

         return result;
      }

      // returns a scaling matrix
      mat4 mat4::scale(const Vec3& scale)
      {
         mat4 result(1.0f);
         
         result.elements[0 + 4 * SIZE] = scale.x;
         result.elements[1 + 4 * SIZE] = scale.y;
         result.elements[2 + 4 * SIZE] = scale.z;

         return result;
      }

      // returns a translation matrix
      mat4 mat4::translation(const Vec3& translation)
      {
         mat4 result(1.0f);

         result.elements[3 + 0 * SIZE] = translation.x;
         result.elements[3 + 1 * SIZE] = translation.y;
         result.elements[3 + 2 * SIZE] = translation.z;

         return result;
      }
   }
}