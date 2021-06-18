#pragma once

#include "LegitEngineCore/legit_engine.h"

#include "LegitEngineCore/src/Components/vec2.h"

#include <chrono>

using namespace legit_engine;
using namespace renderables;
using namespace components;
using namespace std;


const float defaultRadius = 16.0f;
const int timeStep = 0; // time step in milliseconds
const int startingNumBalls = 20;
const float frictionFactor = -0.45;
const float velocityFactor = -5.0f;
const float massFactor = 10.0f;



class CircleCollisions : public Application
{
public:

   CircleCollisions(const char* name, unsigned int screenWidth, unsigned int screenHeight)
      : Application(name, screenWidth, screenHeight) {}

protected:


   struct ball : public Entity
   {
      ball(float xi, float yi, float radiusIn, Texture* sprite)
         : Entity(x, y, radiusIn, radiusIn, 0.0f, sprite)
      {

      }

      float x, y;
      float vx, vy;
      float ax, ay;
      float radius;
      float mass;
      int id;

   };

   struct collision
   {
      ball* b1;
      ball* b2;

      collision(ball* ball1, ball* ball2)
      {
         b1 = ball1;
         b2 = ball2;
      }
   };


   vector <ball*> theBalls;  // vector of balls

   void AddBall(float x, float y, Texture* texture, float radius = defaultRadius)
   {
      ball* newBall = new ball(x, y, radius, texture);
      newBall->x = x;
      newBall->y = y;
      newBall->vx = 0;
      newBall->vy = 0;

      newBall->radius = radius;
      newBall->mass = radius * massFactor;

      newBall->id = theBalls.size();
      theBalls.emplace_back(newBall);
   }

   void RemoveBall(int id)
   {
      theBalls.erase(theBalls.begin() + id);
      for (int i = id + 1; i < theBalls.size() - 1; i++)
      {
         theBalls[i]->id = theBalls[i + 1]->id;
      }
   }

   void AddRandomBalls(int numBalls, float minRadius, float maxRadius)
   {
      float radius;
      for (int i = 0; i < numBalls; i++)
      {
         radius = rand() % (int)maxRadius;
         AddBall(rand() % (int)m_ScreenWidth, rand() % (int)m_ScreenHeight, &ballSprite, (radius > minRadius) ? radius : minRadius);
      }
   }

   bool CircleCollisionDetect(ball* b1, ball* b2)
   {
      return ((b1->radius + b2->radius) * (b1->radius + b2->radius) >= (b1->x - b2->x) * (b1->x - b2->x) + (b1->y - b2->y) * (b1->y - b2->y));
   }

   void CollisionSim(vector <collision> collisionLog)
   {
      for (ball* currBall : theBalls)
      {
         for (ball* targetBall : theBalls)
         {
            if (currBall->id != targetBall->id)
            {
               if (CircleCollisionDetect(currBall, targetBall))
               {

                  float distance = sqrtf((currBall->x - targetBall->x) * (currBall->x - targetBall->x) + (currBall->y - targetBall->y) * (currBall->y - targetBall->y));
                  float overlap = distance - currBall->radius - targetBall->radius;
                  collisionLog.emplace_back(currBall, targetBall);


                  // resolving collision, move both balls away by half of overlap
                  currBall->x -= overlap * 0.5f * (currBall->x - targetBall->x) / distance;
                  currBall->y -= overlap * 0.5f * (currBall->y - targetBall->y) / distance;

                  targetBall->x += overlap * 0.5f * (currBall->x - targetBall->x) / distance;
                  targetBall->y += overlap * 0.5f * (currBall->y - targetBall->y) / distance;
               }
            }
         }
      }
      if (collisionLog.size() > 0)
      {
         for (auto& collision : collisionLog)
         {
            ball* b1 = collision.b1;
            ball* b2 = collision.b2;
            float fDistance = sqrtf((collision.b1->x - collision.b2->x) * (collision.b1->x - collision.b2->x) + (collision.b1->y - collision.b2->y) * (collision.b1->y - collision.b2->y));
            float nx = (b2->x - b1->x) / fDistance;
            float ny = (b2->y - b1->y) / fDistance;

            // from wikipedia
            float kx = (b1->vx - b2->vx);
            float ky = (b1->vy - b2->vy);
            float p = 2.0 * (nx * kx + ny * ky) / (b1->mass + b2->mass);
            b1->vx = b1->vx - p * b2->mass * nx;
            b1->vy = b1->vy - p * b2->mass * ny;
            b2->vx = b2->vx + p * b1->mass * nx;
            b2->vy = b2->vy + p * b1->mass * ny;
         }
      }
   }

   void PhysicsSim(float elapsedTime)
   {
      for (auto& currBall : theBalls)
      {
         currBall->ax = currBall->vx * frictionFactor;
         currBall->ay = currBall->vy * frictionFactor;

         currBall->vx += currBall->ax * elapsedTime;
         currBall->vy += currBall->ay * elapsedTime;

         currBall->x += currBall->vx * elapsedTime;
         currBall->y += currBall->vy * elapsedTime;

         if (fabs(currBall->vx * currBall->vx + currBall->vy * currBall->vy) < 0.2f)
         {
            currBall->vx = 0;
            currBall->vy = 0;
         }

         if (currBall->y > m_ScreenHeight)
            currBall->y -= m_ScreenHeight;
         else if (currBall->y < 0)
            currBall->y += m_ScreenWidth;

         if (currBall->x > m_ScreenWidth)
            currBall->x -= m_ScreenWidth; 
         else if (currBall->x < 0)
            currBall->x += m_ScreenWidth;
        
      }
   }

   bool CircleSelected(ball* b)
   {
      return ((m_MousePosition.x - b->x) * (m_MousePosition.x - b->x) + (m_MousePosition.y - b->y) * (m_MousePosition.y - b->y) < (b->radius * b->radius));
   }

public:
   ball* selectedBall = nullptr;
   vector <collision> collisionLog;  // collision record
   std::chrono::system_clock::time_point prevTime = chrono::system_clock::now();
   std::chrono::system_clock::time_point currTime = chrono::system_clock::now();
   float elapsedTime;
   Texture ballSprite = Texture("res/cirlce.png");



public:


   bool OnUserCreate() override
   {
      theBalls.reserve(startingNumBalls);
      collisionLog.reserve(startingNumBalls);
      AddRandomBalls(startingNumBalls, 50, 100);
      AddRandomBalls(1, 100, 100);
      glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
      AddBall(m_ScreenWidth, m_ScreenHeight, &ballSprite, 100);

      return true;
   }

   bool OnUserUpdate() override
   {
      // elapsed time for physics sim
      currTime = chrono::system_clock::now();
      chrono::duration<float> elapsedTimeCalc = currTime - prevTime;
      elapsedTime = elapsedTimeCalc.count();
      prevTime = currTime;


      mat4 ortho = mat4::orthographic(0, m_ScreenWidth, 0, m_ScreenHeight, -1.0f, 1.0f);
      m_Shader->setUniformMat4("pr_matrix", ortho);

      //=========================================================================//
      //                            input handling                               //
      //=========================================================================//

      if (m_Keys[KEY_1].bPressed) AddRandomBalls(10, 5, 20);
      if (m_Keys[KEY_2].bPressed) AddRandomBalls(10, 20, 40);
      if (m_Keys[KEY_3].bPressed) AddRandomBalls(10, 50, 150);
      if (m_Keys[KEY_4].bPressed) AddRandomBalls(2, 50, 150);
      if (m_Keys[KEY_C].bPressed)
      {
         for (auto& i : theBalls)
         {
            delete i;
         }
         theBalls.clear();

      }

      for (auto& i : theBalls)
      {
         if ((m_Mouse[BUTTON_1].bPressed || m_Mouse[BUTTON_2].bPressed) && CircleSelected(i))
         {
            selectedBall = i;
            break;
         }
      }
      if (selectedBall != nullptr)
      {
         if (m_Mouse[BUTTON_1].bHeld)
         {
            selectedBall->vx = 0;
            selectedBall->vy = 0;

            selectedBall->x = m_MousePosition.x;
            selectedBall->y = m_MousePosition.y;
         }
         else if (m_Mouse[BUTTON_2].bHeld)
         {
            m_Renderer->submitLine(m_MousePosition.x, m_MousePosition.y, selectedBall->x, selectedBall->y, 3, 5);
         }
         else if (m_Mouse[BUTTON_2].bReleased)
         {
            selectedBall->vx = velocityFactor * (m_MousePosition.x - selectedBall->x);
            selectedBall->vy = velocityFactor * (m_MousePosition.y - selectedBall->y);
            selectedBall = nullptr;
         }
      }
      if (m_Mouse[BUTTON_1].bReleased)
         selectedBall = nullptr;

      //===========================================================================//

      // call simulation functions
      CollisionSim(collisionLog);
      PhysicsSim(elapsedTime);
      collisionLog.clear();
      collisionLog.reserve(startingNumBalls);

      // resolving final ball positions after sims
      for (ball* i : theBalls)
      {
         i->setPosition(i->x, i->y);
         m_Renderer->submitEntity(i);
      }

      int size = theBalls.size();
      ImGui::Begin("Debug Menu", 0, ImGuiWindowFlags_AlwaysAutoResize);
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::InputInt("Rotation", &size);
      ImGui::DragFloat("mouse x", &m_MousePosition.x);
      ImGui::DragFloat("mouse y", &m_MousePosition.y);
      ImGui::SetWindowFontScale(1.5f);
      ImGui::End();

      return true;
   }



};


int main()
{
   int screenWidth = 2000, screenHeight = 1500;
   double mouseX = 0, mouseY = 0;
   const char* appName = "babbees first graphics engine";

   CircleCollisions game(appName, screenWidth, screenHeight);

   game.Start();

   return 0;
}