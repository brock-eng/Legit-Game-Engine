#pragma once


#pragma once

#include "sys.h"

namespace legit_engine {
   namespace components {

      class mat3
      {
      public:

         union
         {
            float elements[9] = { 0 };
            Vec2 rows[3];
         };

         mat3();
         mat3(float diagonal);

         static mat3 identity();

         mat3& multiply(const mat3& m);
         friend mat3 operator*(mat3 left, const mat3& right);
         mat3& operator*=(const mat3& right);

         static mat3 rotation(float angle);
         static mat3 translation(const Vec2& translation);
         static mat3 scale(const Vec2& scale);

         std::string toString() const;
      };
   }
}