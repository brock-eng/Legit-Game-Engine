#pragma once

#include "../Buffers/buffer.h"
#include "../Buffers/indexbuffer.h"
#include "../Buffers/vertexarray.h"
#include "../Renderables/texture.h"
#include "../Shaders/shaders.h"

#include "../../Components/mathlib.h"

namespace legit_engine {
   namespace renderables {

      struct VertexData
      {
         components::Vec3 vertex;
         components::Vec2 texCoords;
         unsigned int color;
         float textureIndex;
      };

      class Renderable2D
      {
      protected:
         components::Vec2 m_Size;
         components::Vec3 m_Position;
         components::Vec4 m_Color;
         std::vector<components::Vec2> m_UV;
         Texture* m_Texture;

      protected:
         Renderable2D(components::Vec3 position, components::Vec2 size, components::Vec4 color, std::vector<components::Vec2>* UV = nullptr)
            : m_Position(position), m_Size(size), m_Color(color) , m_Texture(nullptr)
         {
            if (UV == nullptr)
               defaultUV();
            else
               m_UV = *UV;
         }

         Renderable2D(components::Vec3 position, components::Vec2 size, Texture* texture, std::vector<components::Vec2>* UV = nullptr)
            : m_Position(position), m_Size(size)
         {
            if (UV == nullptr)
               defaultUV();
            else
               m_UV = *UV;

            m_Texture = texture;
         }

         ~Renderable2D()
         {
         }

      public:
         inline const components::Vec2& getSize()              const { return m_Size; }
         inline const components::Vec3& getPosition()          const { return m_Position; }
         inline const components::Vec4& getColor()             const { return m_Color; }
         inline const std::vector<components::Vec2>& getUV()   const { return m_UV;}
         inline const unsigned int getTextureID()              const { return m_Texture == nullptr ? 0 : m_Texture->getID(); }
         inline void setPosition(float x, float y, float z)
         {
            m_Position.x = x;
            m_Position.y = y;
            m_Position.z = z;
         }

         static const std::vector<components::Vec2> getDefaultUV()
         {
            return std::vector<components::Vec2>
            {
               { 0, 0 },
               { 0, 1 },
               { 1, 1 },
               { 1, 0 }
            };
         }
      private:
         void defaultUV()
         {
            m_UV.push_back(components::Vec2(0, 0));
            m_UV.push_back(components::Vec2(0, 1));
            m_UV.push_back(components::Vec2(1, 1));
            m_UV.push_back(components::Vec2(1, 0));
         }
      };
} }