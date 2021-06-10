#pragma once

#include <iostream>

namespace legit_engine
{
   namespace components
   {
      class Vec4
      {
      public: 
         float x, y, z, w;
         
         Vec4() = default;
         Vec4(const float& x, const float& y, const float& z, const float& w);

         // math operation methods
         Vec4& add(const Vec4& v2);
         Vec4& subtract(const Vec4& v2);
         Vec4& multiply(const Vec4& v2);
         Vec4& divide(const Vec4& v2);

         // math operators
         friend Vec4& operator+(Vec4 v1, const Vec4& v2);
         friend Vec4& operator-(Vec4 v1, const Vec4& v2);
         friend Vec4& operator*(Vec4 v1, const Vec4& v2);
         friend Vec4& operator/(Vec4 v1, const Vec4& v2);

         // compound operators
         Vec4& operator+=(const Vec4& v2);
         Vec4& operator-=(const Vec4& v2);
         Vec4& operator*=(const Vec4& v2);
         Vec4& operator/=(const Vec4& v2);

         // comparison operators
         bool operator==(const Vec4& v2);
         bool operator!=(const Vec4& v2);
         bool operator>(const Vec4& v2);
         bool operator<(const Vec4& v2);
         bool operator>=(const Vec4& v2);
         bool operator<=(const Vec4& v2);

         friend std::ostream& operator<<(std::ostream& stream, const Vec4& v2);
         
      };
   }
}