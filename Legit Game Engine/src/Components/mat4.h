#pragma once

#include "mat4.h"
#include "sys.h"

namespace legit_engine {
   namespace components {

      const int SIZE = 4;
      const float PI = (float) 3.14159;

      class mat4
      {
      public:

         union
         {
            float elements[4 * 4];
            Vec4 collumns[4];
         };

         mat4();
         mat4(float diagonal);

         static mat4 identity();

         mat4& multiply(const mat4& m);
         friend mat4 operator*(mat4 left, const mat4& right);
         mat4& operator*=(const mat4& right);

         mat4 orthographic(float left, float right, float top, float bottom, float near, float far);
         mat4 perspective(float fov, float aspect, float near, float far);

         mat4 rotation(float angle, const Vec3& axis);
         mat4 translation(const Vec3& translation);
         mat4 scale(const Vec3& scale);
      };
   }
}