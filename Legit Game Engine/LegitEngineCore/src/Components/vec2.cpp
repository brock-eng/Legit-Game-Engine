#pragma once
#include "vec2.h"

#include <iostream>

namespace legit_engine 
{
   namespace components 
   {
      Vec2::Vec2()
      {
         x = 0.0f;
         y = 0.0f;
      }

      Vec2::Vec2(const float& x, const float& y)
      {
         this->x = x;
         this->y = y;
      }
      
      Vec2& Vec2::add(const Vec2& v2) 
      {
         x += v2.x;
         y += v2.y;

         return *this;
      }

      Vec2& Vec2::subtract(const Vec2& v2)
      {
         x -= v2.x;
         y -= v2.y;

         return *this;
      }

      Vec2& Vec2::multiply(const Vec2& v2)
      {
         x *= v2.x;
         y *= v2.y;

         return *this;
      }
      
      Vec2& Vec2::multiply(const float f)
      {
         x *= f;
         y *= f;
         return *this;
      }


      Vec2& Vec2::divide(const Vec2& v2)
      {
         x /= v2.x;
         y /= v2.y;

         return *this;
      }

      Vec2& Vec2::divide(const float f)
      {
         x /= f;
         y /= f;
         return *this;
      }


      std::ostream& operator<<(std::ostream& stream, const Vec2& v2)
      {
         stream << "Vec2: <" << v2.x << ", " << v2.y << ">";
         return stream;
      }


      Vec2& operator+(Vec2 v1, const Vec2& v2)
      {
         return v1.add(v2);
      }

      Vec2& operator-(Vec2 v1, const Vec2& v2)
      {
         return v1.subtract(v2);
      }
      
      Vec2& operator*(Vec2 v1, const Vec2& v2)
      {
         return v1.multiply(v2);
      }

      Vec2& operator*(Vec2 v1, const float& f)
      {
         
         return v1.multiply(f);
      }


      Vec2& operator/(Vec2 v1, const Vec2& v2)
      {
         return v1.divide(v2);
      }

      Vec2& operator/(Vec2 v1, const float& f)
      {

         return v1.divide(f);
      }

      bool Vec2::operator==(const Vec2& v2)
      {
         return (x == v2.x && y == v2.y);
      }

      bool Vec2::operator!=(const Vec2& v2)
      {
         return !(*this == v2);
      }

      bool Vec2::operator>(const Vec2& v2)
      {
         return false;
      }
      
      bool Vec2::operator<(const Vec2& v2)
      {
         return false;
      }

      bool Vec2::operator>=(const Vec2& v2)
      { 
         return false;
      }
      
      bool Vec2::operator<=(const Vec2& v2)
      {
         return false;
      }

      Vec2& Vec2::operator+=(const Vec2& v2)
      {
         add(v2);
         return *this;
      }

      Vec2& Vec2::operator-=(const Vec2& v2)
      {
         subtract(v2);
         return *this;
      }

      Vec2& Vec2::operator*=(const Vec2& v2)
      {
         multiply(v2);
         return *this;
      }

      Vec2& Vec2::operator/=(const Vec2& v2)
      {
         divide(v2);
         return *this;
      }

      float Vec2::magnitude() const
      {
         return sqrt(x * x + y * y);
      }

      Vec2 Vec2::normalise() const
      {
         float length = magnitude();
         return Vec2(x / length, y / length);
      }
   }
}