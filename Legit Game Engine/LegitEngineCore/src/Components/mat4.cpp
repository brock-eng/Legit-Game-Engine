#pragma once

#include <math.h>
#include <sstream>

#include "mat4.h"

namespace legit_engine {
   namespace components {

      // initializes a matrix of zeros
      mat4::mat4()
      {
         for (int i = 0; i < SIZE * SIZE; i++)
            elements[i] = 0.0f;
      }

      mat4::mat4(float (&floatArray)[SIZE*SIZE])
      {
         for (int i = 0; i < SIZE * SIZE; i++)
         {
            elements[i] = floatArray[i];
         }
      }

      // initializes a diagonal matrix of input number
      mat4::mat4(float diagonal)
      {
         memset(elements, 0, 4 * 4 * sizeof(float));
         elements[0 + 0 * SIZE] = diagonal;
         elements[1 + 1 * SIZE] = diagonal;
         elements[2 + 2 * SIZE] = diagonal;
         elements[3 + 3 * SIZE] = diagonal;
      }

      // identity matrix
      mat4 mat4::identity()
      {
         return mat4(1.0f);
      }

      // standard matrix multiplication
      mat4& mat4::multiply(const mat4& other)
      {
         float data[16];
         for (int row = 0; row < 4; row++)
         {
            for (int col = 0; col < 4; col++)
            {
               float sum = 0.0f;
               for (int e = 0; e < 4; e++)
               {
                  sum += elements[e + row * 4] * other.elements[col + e * 4];
               }
               data[col + row * 4] = sum;
            }
         }
         memcpy(elements, data, 4 * 4 * sizeof(float));
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

      // returns orthographic projection matrix
      mat4 mat4::orthographic(float left, float right, float bottom, float top, float near, float far)
      {
         mat4 result(1.0f);

         result.elements[0 + 0 * 4] = 2.0f / (right - left);
         result.elements[1 + 1 * 4] = 2.0f / (top - bottom);
         result.elements[2 + 2 * 4] = 2.0f / (near - far);

         result.elements[3 + 0 * 4] = (left + right) / (left - right);
         result.elements[3 + 1 * 4] = (bottom + top) / (bottom - top);
         result.elements[3 + 2 * 4] = (far + near) / (far - near);

         return result;
      }

      // returns perspective matrix
      mat4 mat4::perspective(float fov, float aspect, float near, float far)
      {
         
         mat4 result(1.0f);

         float q = 1.0f / tan(PI/ 180.0f * (0.5f * fov));
         float a = q / aspect;

         float b = (near + far) / (near - far);
         float c = (2.0f * near * far) / (near - far);

         result.elements[0 + 0 * 4] = a;
         result.elements[1 + 1 * 4] = q;
         result.elements[2 + 2 * 4] = b;
         result.elements[2 + 3 * 4] = -1.0f;
         result.elements[3 + 2 * 4] = c;

         return result;
      }

      // returns a rotation matrix that applies a specified angle roation (in degrees)
      // around a specified 3D axis (defined via a Vec3)
      mat4 mat4::rotation(float angle, const Vec3& axis)
      {
         mat4 result(1.0f);

         float r = angle * 3.13159f / 180.0f;
         float c = cos(r);
         float s = sin(r);
         float omc = 1.0f - c;

         float x = axis.x;
         float y = axis.y;
         float z = axis.z;

         result.elements[0 + 0 * 4] = x * x * omc + c;
         result.elements[0 + 1 * 4] = y * x * omc + z * s;
         result.elements[0 + 2 * 4] = x * z * omc - y * s;

         result.elements[1 + 0 * 4] = x * y * omc - z * s;
         result.elements[1 + 1 * 4] = y * y * omc + c;
         result.elements[1 + 2 * 4] = y * z * omc + x * s;

         result.elements[2 + 0 * 4] = x * z * omc + y * s;
         result.elements[2 + 1 * 4] = y * z * omc - x * s;
         result.elements[2 + 2 * 4] = z * z * omc + c;

         return result;
      }

      // returns a scaling matrix
      mat4 mat4::scale(const Vec3& scale)
      {
         mat4 result(0.0f);
         
         result.elements[0 + 0 * SIZE] = scale.x;
         result.elements[1 + 1 * SIZE] = scale.y;
         result.elements[2 + 2 * SIZE] = scale.z;

         return result;
      }

      // returns a translation matrix
      mat4 mat4::translation(const Vec3& translation)
      {
         mat4 result(1.0f);

         result.elements[3 + 0 * 4] = translation.x;
         result.elements[3 + 1 * 4] = translation.y;
         result.elements[3 + 2 * 4] = translation.z;

         return result;
      }

      std::string mat4::toString() const
      {
         std::stringstream result;
         result << "mat4: (" << rows[0].x << ", " << rows[1].x << ", " << rows[2].x << ", " << rows[3].x << "), ";
         result << "(" << rows[0].y << ", " << rows[1].y << ", " << rows[2].y << ", " << rows[3].y << "), ";
         result << "(" << rows[0].z << ", " << rows[1].z << ", " << rows[2].z << ", " << rows[3].z << "), ";
         result << "(" << rows[0].w << ", " << rows[1].w << ", " << rows[2].w << ", " << rows[3].w << ")\n";
         return result.str();
      }

   }
}