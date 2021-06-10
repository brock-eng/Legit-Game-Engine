#pragma once
#include "Vec3.h"

#include <iostream>

namespace legit_engine 
{
   namespace components 
   {
      Vec3::Vec3()
      {
         x = 0.0f;
         y = 0.0f;
         z = 0.0f;
      }

      Vec3::Vec3(const float& x, const float& y, const float& z)
      {
         this->x = x;
         this->y = y;
         this->z = z;
      }


      Vec3& Vec3::add(const Vec3& v2) 
      {
         this->x += v2.x;
         this->y += v2.y;
         this->z += v2.z;

         return *this;
      }

      Vec3& Vec3::subtract(const Vec3& v2)
      {
         this->x -= v2.x;
         this->y -= v2.y;
         this->z -= v2.z;

         return *this;
      }

      Vec3& Vec3::multiply(const Vec3& v2)
      {
         this->x *= v2.x;
         this->y *= v2.y;
         this->z *= v2.z;

         return *this;
      }

      Vec3& Vec3::divide(const Vec3& v2)
      {
         this->x /= v2.x;
         this->y /= v2.y;
         this->z /= v2.z;

         return *this;
      }

      std::ostream& operator<<(std::ostream& stream, const Vec3& v2)
      {
         stream << "Vec3: <" << v2.x << ", " << v2.y << ", " << v2.z << ">";
         return stream;
      }


      Vec3& operator+(Vec3 v1, const Vec3& v2)
      {
         return v1.add(v2);
      }

      Vec3& operator-(Vec3 v1, const Vec3& v2)
      {
         return v1.subtract(v2);
      }
      
      Vec3& operator*(Vec3 v1, const Vec3& v2)
      {
         return v1.multiply(v2);
      }

      Vec3& operator/(Vec3 v1, const Vec3& v2)
      {
         return v1.divide(v2);
      }

      bool Vec3::operator==(const Vec3& v2)
      {
         return (x == v2.x && y == v2.y && z == v2.z);
      }

      bool Vec3::operator!=(const Vec3& v2)
      {
         return !(*this == v2);
      }

      bool Vec3::operator>(const Vec3& v2)
      {
         return false;
      }
      
      bool Vec3::operator<(const Vec3& v2)
      {
         return false;
      }

      bool Vec3::operator>=(const Vec3& v2)
      {
         return false;
      }
      
      bool Vec3::operator<=(const Vec3& v2)
      {
         return false;
      }

      Vec3& Vec3::operator+=(const Vec3& v2)
      {
         add(v2);
         return *this;
      }

      Vec3& Vec3::operator-=(const Vec3& v2)
      {
         subtract(v2);
         return *this;
      }

      Vec3& Vec3::operator*=(const Vec3& v2)
      {
         multiply(v2);
         return *this;
      }

      Vec3& Vec3::operator/=(const Vec3& v2)
      {
         divide(v2);
         return *this;
      }
   }
}