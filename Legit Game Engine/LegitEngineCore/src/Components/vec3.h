#pragma once

#include <iostream>

namespace legit_engine
{
   namespace components
   {
      class Vec3
      {
      public: 
         float x, y, z;
         
         Vec3();
         Vec3(const float& x, const float& y, const float& z);

         // math operation methods
         Vec3& add(const Vec3& v2);
         Vec3& subtract(const Vec3& v2);
         Vec3& multiply(const Vec3& v2);
         Vec3& divide(const Vec3& v2);

         // math operators
         friend Vec3& operator+(Vec3 v1, const Vec3& v2);
         friend Vec3& operator-(Vec3 v1, const Vec3& v2);
         friend Vec3& operator*(Vec3 v1, const Vec3& v2);
         friend Vec3& operator/(Vec3 v1, const Vec3& v2);

         // compound operators
         Vec3& operator+=(const Vec3& v2);
         Vec3& operator-=(const Vec3& v2);
         Vec3& operator*=(const Vec3& v2);
         Vec3& operator/=(const Vec3& v2);

         // comparison operators
         bool operator==(const Vec3& v2);
         bool operator!=(const Vec3& v2);
         bool operator>(const Vec3& v2);
         bool operator<(const Vec3& v2);
         bool operator>=(const Vec3& v2);
         bool operator<=(const Vec3& v2);

         friend std::ostream& operator<<(std::ostream& stream, const Vec3& v2);
      };
   }
}