#pragma once

#include "entity.h"

namespace legit_engine {

   using namespace components;
   Entity::Entity(float x, float y, float width, float height, float rotation, renderables::Texture* texture, std::vector<components::Vec2>* textureCoordinates)
      : Renderable2D(Vec3(x, y, 0), Vec2(width, height), texture, textureCoordinates), m_Rotation(rotation)
   {
   }

   Entity::~Entity()
   {

   }

   bool Entity::CollidesWith(const Entity& other)
   {
      return ( m_Position.x < other.m_Position.x + other.m_Size.x &&
               m_Position.x + other.m_Size.x > other.m_Position.x &&
               m_Position.y < other.m_Position.y + other.m_Size.y &&
               m_Position.y + other.m_Size.y > other.m_Position.y    );
   }

   void Entity::Move()
   {
      setPosition(m_Position.x + m_VelX, m_Position.y + m_VelY);
   }

   void Entity::setPosition(float x, float y)
   {
      m_Position.x = x;
      m_Position.y = y;
   }

   void Entity::setWrappedPosition(float x, float y, float screenWidth, float screenHeight)
   {
      if (y > screenHeight)
         m_Position.y =  y - screenHeight;
      else if (y < 0)
         m_Position.y = y + screenHeight;
      else
         m_Position.y = y;

      if (x > screenWidth)
         m_Position.x = x - screenWidth;
      else if (x < 0)
         m_Position.x = x + screenWidth;
      else m_Position.x = x;
   }


   void Entity::setVelocity(float vx, float vy)
   {
      m_VelX = vx;
      m_VelY = vy;
   }


   void Entity::setPositionX(float x)
   {
      m_Position.x = x;
   }

   void Entity::setPositionY(float y)
   {
      m_Position.y = y;
   }
}