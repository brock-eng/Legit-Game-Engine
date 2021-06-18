#pragma once

#include "../graphics/Renderables/renderable2d.h"

namespace legit_engine {

   class Entity : public renderables::Renderable2D
   {
   private:
      float m_VelX, m_VelY;
      float m_Rotation;

   public:
      Entity(float x, float y, float width, float height, float rotation, renderables::Texture* texture, std::vector<components::Vec2>* textureCoordinates = nullptr);
      ~Entity();

      bool CollidesWith(const Entity& other);

      void Move();
   public:
      void setPosition(float x, float y);
      void setWrappedPosition(float x, float y, float screenWidth, float screenHeight);
      void setVelocity(float x, float y);
      void setPositionX(float x);
      void setPositionY(float y);
      inline const void setRotation(float rotation) { m_Rotation = rotation; }
      inline const void modifyRotation(float angle) { (m_Rotation + angle > 360.0f) ? m_Rotation += angle - 360 : m_Rotation += angle; }
      inline const float getRotation() const { return m_Rotation; }


   };
}
