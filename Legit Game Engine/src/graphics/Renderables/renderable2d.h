#pragma once

#include "../Buffers/buffer.h"
#include "../Buffers/indexbuffer.h"
#include "../Buffers/vertexarray.h"

#include "../Shaders/shaders.h"

#include "../../Components/Sys.h"

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
         short m_TextureID;

      protected:
         Renderable2D(components::Vec3 position, components::Vec2 size, components::Vec4 color, short texture, const std::vector<components::Vec2>* UV = nullptr)
            : m_Position(position), m_Size(size), m_Color(color) , m_TextureID(texture)
         {
            if (UV == nullptr)
               defaultUV();
         }

         ~Renderable2D()
         {}

      public:
         inline const components::Vec2& getSize() const { return m_Size; }
         inline const components::Vec3& getPosition() const  { return m_Position; }
         inline const components::Vec4& getColor() const {return m_Color; }
         inline const std::vector<components::Vec2>& getUV() const { return m_UV;}
         inline const short getTextureID() const { return m_TextureID; }
         inline components::Vec3& getPositionM() { return m_Position; }
         inline void setPosition(float x, float y, float z)
         {
            m_Position.x = x;
            m_Position.y = y;
            m_Position.z = z;
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