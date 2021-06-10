#pragma once
#include "Vec4.h"

#include <iostream>

namespace legit_engine 
{
   namespace components 
   {

      Vec4::Vec4(const float& x, const float& y, const float& z, const float& w)
      {
         this->x = x;
         this->y = y;
         this->z = z;
         this->w = w;
      }


      Vec4& Vec4::add(const Vec4& v2) 
      {
         x += v2.x;
         y += v2.y;
         z += v2.z;
         w += v2.w;

         return *this;
      }

      Vec4& Vec4::subtract(const Vec4& v2)
      {
         x -= v2.x;
         y -= v2.y;

         return *this;
      }

      Vec4& Vec4::multiply(const Vec4& v2)
      {
         x *= v2.x;
         y *= v2.y;

         return *this;
      } 

      Vec4& Vec4::divide(const Vec4& v2)
      {
         x /= v2.x;
         y /= v2.y;

         return *this;
      }

      std::ostream& operator<<(std::ostream& stream, const Vec4& v2)
      {
         stream << "Vec4: <" << v2.x << ", " << v2.y << ", " << v2.z << ", " << v2.w << ">";
         return stream;
      }


      Vec4& operator+(Vec4 v1, const Vec4& v2)
      {
         return v1.add(v2);
      }

      Vec4& operator-(Vec4 v1, const Vec4& v2)
      {
         return v1.subtract(v2);
      }
      
      Vec4& operator*(Vec4 v1, const Vec4& v2)
      {
         return v1.multiply(v2);
      }

      Vec4& operator/(Vec4 v1, const Vec4& v2)
      {
         return v1.divide(v2);
      }

      bool Vec4::operator==(const Vec4& v2)
      {
         return (x == v2.x && y == v2.y && z == v2.z && w == v2.w);
      }

      bool Vec4::operator!=(const Vec4& v2)
      {
         return !(*this == v2);
      }

      bool Vec4::operator>(const Vec4& v2)
      {
         return false;
      }
      
      bool Vec4::operator<(const Vec4& v2)
      {
         return false;
      }

      bool Vec4::operator>=(const Vec4& v2)
      {
         return false;
      }
      
      bool Vec4::operator<=(const Vec4& v2)
      {
         return false;
      }

      Vec4& Vec4::operator+=(const Vec4& v2)
      {
         add(v2);
         return *this;
      }

      Vec4& Vec4::operator-=(const Vec4& v2)
      {
         subtract(v2);
         return *this;
      }

      Vec4& Vec4::operator*=(const Vec4& v2)
      {
         multiply(v2);
         return *this;
      }

      Vec4& Vec4::operator/=(const Vec4& v2)
      {
         divide(v2);
         return *this;
      }
   }
}