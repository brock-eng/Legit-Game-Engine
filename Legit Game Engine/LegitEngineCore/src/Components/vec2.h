#pragma once

#include <iostream>

namespace legit_engine
{
   namespace components
   {

      // 2D Vector math class
      class Vec2
      {
      public: 
         float x, y;
         
         Vec2();
         Vec2(const float& x, const float& y);

         // math operation methods
         Vec2& add(const Vec2& v2);
         Vec2& subtract(const Vec2& v2);
         Vec2& multiply(const Vec2& v2);
         Vec2& multiply(const float f);
         Vec2& divide(const Vec2& v2);
         Vec2& divide(const float f);

         // math operators
         friend Vec2& operator+(Vec2 v1, const Vec2& v2);
         friend Vec2& operator-(Vec2 v1, const Vec2& v2);
         friend Vec2& operator*(Vec2 v1, const Vec2& v2);
         friend Vec2& operator*(Vec2 v1, const float& f);
         friend Vec2& operator/(Vec2 v1, const Vec2& v2);
         friend Vec2& operator/(Vec2 v1, const float& f);

         // compound operators
         Vec2& operator+=(const Vec2& v2);
         Vec2& operator-=(const Vec2& v2);
         Vec2& operator*=(const Vec2& v2);
         Vec2& operator/=(const Vec2& v2);

         // comparison operators
         bool operator==(const Vec2& v2);
         bool operator!=(const Vec2& v2);
         bool operator>(const Vec2& v2);
         bool operator<(const Vec2& v2);
         bool operator>=(const Vec2& v2);
         bool operator<=(const Vec2& v2);

         friend std::ostream& operator<<(std::ostream& stream, const Vec2& v2);

         // line operations
         float magnitude() const;
         Vec2 normalise() const;
         
      };
   }
}