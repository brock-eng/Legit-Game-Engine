#pragma once

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
            float elements[SIZE * SIZE];
            Vec4 rows[SIZE];
         };

         mat4();
         mat4(float diagonal);

         static mat4 identity();

         mat4& multiply(const mat4& m);
         friend mat4 operator*(mat4 left, const mat4& right);
         mat4& operator*=(const mat4& right);

         static mat4 orthographic(float left, float right, float top, float bottom, float near, float far);
         static mat4 perspective(float fov, float aspect, float near, float far);

         static mat4 rotation(float angle, const Vec3& axis);
         static mat4 translation(const Vec3& translation);
         static mat4 scale(const Vec3& scale);

         std::string toString() const;
      };
   }
}