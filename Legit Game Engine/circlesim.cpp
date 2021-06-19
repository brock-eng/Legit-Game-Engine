#pragma once


#include "LegitEngineCore/legit_engine.h"

#include "LegitEngineCore/src/Components/vec2.h"

#include <chrono>

#include <math.h>

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


   struct ball
   {
      float x, y;
      float vx, vy;
      float ax, ay;
      float radius;
      float mass;
      int id;


   };

   struct sLineSegment
   {
      float sx, sy;
      float ex, ey;
      float radius;
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
      ball* newBall = new ball();
      newBall->x = x;
      newBall->y = y;
      newBall->vx = 0;
      newBall->vy = 0;

      newBall->radius = radius;
      newBall->mass = radius * massFactor;

      newBall->id = theBalls.size();
      theBalls.emplace_back(newBall);
   }

   void AddLine()
   {
      theLines.push_back(new sLineSegment{ m_ScreenWidth / 4.0f, m_ScreenHeight / 4.0f, m_ScreenWidth / 2.0f, m_ScreenHeight / 2.0f, lineRadius });
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

         for (sLineSegment* line : theLines)
         {
            // Check that line formed by velocity vector, intersects with line segment
            float fLineX1 = line->ex - line->sx;
            float fLineY1 = line->ey - line->sy;

            float fLineX2 = currBall->x - line->sx;
            float fLineY2 = currBall->y - line->sy;

            float fEdgeLength = fLineX1 * fLineX1 + fLineY1 * fLineY1;

            // This effectively calculates the 'shadow' casted on the line by the ball
            // effectively retrieves the closest point on the line to the ball
            float t = max(0, min(fEdgeLength, (fLineX1 * fLineX2 + fLineY1 * fLineY2))) / fEdgeLength;
            float fClosestPointX = line->sx + t * fLineX1;
            float fClosestPointY = line->sy + t * fLineY1;

            // check for a collision
            float fDistance = sqrtf((currBall->x - fClosestPointX) * (currBall->x - fClosestPointX) + (currBall->y - fClosestPointY) * (currBall->y - fClosestPointY));


            // 
            if (fDistance <= (currBall->radius + line->radius))
            {
               ball* temp = new ball();
               temp->radius = line->radius;
               temp->mass = currBall->mass * 0.8f;
               temp->x = fClosestPointX;
               temp->y = fClosestPointY;
               temp->vx = -currBall->vx;	
               temp->vy = -currBall->vy;	

               fakeBalls.push_back(temp);

               collisionLog.push_back({ currBall, temp });

               float fOverlap = 1.0f * (fDistance - currBall->radius - temp->radius);

               currBall->x -= fOverlap * (currBall->x - temp->x) / fDistance;
               currBall->y -= fOverlap * (currBall->y - temp->y) / fDistance;
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

            // from wikipedia: conservation of momentum in 1D
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
         currBall->vy += currBall->ay * elapsedTime - gravity;

         currBall->x += currBall->vx * elapsedTime;
         currBall->y += currBall->vy * elapsedTime;

         if (fabs(currBall->vx * currBall->vx + currBall->vy * currBall->vy) < 0.2f)
         {
            currBall->vx = 0;
            currBall->vy = 0;
         }

         // wrap balls around the edge
         if (currBall->y > m_ScreenHeight)
            currBall->y -= m_ScreenHeight;
         else if (currBall->y < 0)
            currBall->y += m_ScreenHeight;

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

   bool CircleSelected(float x1, float y1, float radius)
   {
      return ((m_MousePosition.x - x1) * (m_MousePosition.x - x1) + (m_MousePosition.y - y1) * (m_MousePosition.y - y1) < (radius * radius));
   }

public:
   // selected object pointers
   ball* selectedBall = nullptr;
   sLineSegment* selectedLine = nullptr;

   // containers for our simulation data
   vector <collision> collisionLog; 
   vector <sLineSegment*> theLines;
   vector <ball*> fakeBalls;

   // declaring some clock variables
   std::chrono::system_clock::time_point prevTime = chrono::system_clock::now();
   std::chrono::system_clock::time_point currTime = chrono::system_clock::now();

   // some class scope variables used throughout the simulation
   float elapsedTime;
   float intensity = 0.5f;
   bool renderline = false;
   bool bSelectedLineStart = false;
   float gravity;
   int numSimulationUpdates = 4;
   float lineRadius = 50.f;
   bool renderControlInfo = false;
   bool renderAbout = false;
   // game assets
   Texture ballSprite = Texture("res/cirlce.png");

public:
   bool OnUserCreate() override
   {

      // allocating and creating our game objects
      theBalls.reserve(startingNumBalls);
      collisionLog.reserve(startingNumBalls);
      AddRandomBalls(startingNumBalls, 50, 100);
      AddRandomBalls(1, 100, 100);
      AddBall(m_ScreenWidth, m_ScreenHeight, &ballSprite, 100);
      
      theLines.push_back(new sLineSegment{ 100, 100, 600, 600, lineRadius });

      // background color
      glClearColor(0, 0, 0, 0);

      return true;
   }

   bool OnUserUpdate() override
   {
      // handling clock and time information
      currTime = chrono::system_clock::now();
      chrono::duration<float> elapsedTimeCalc = currTime - prevTime;
      elapsedTime = elapsedTimeCalc.count();
      prevTime = currTime;
      elapsedTime /= (float)numSimulationUpdates;


      mat4 ortho = mat4::orthographic(0, m_ScreenWidth, 0, m_ScreenHeight, -1.0f, 1.0f);
      m_Shader->setUniformMat4("pr_matrix", ortho);

      //=========================================================================//
      //                            input handling                               //
      //=========================================================================//

      if (m_Keys[KEY_1].bPressed) AddRandomBalls(10, 5, 20);      // add small balls
      if (m_Keys[KEY_2].bPressed) AddRandomBalls(10, 20, 40);     // add medium balls
      if (m_Keys[KEY_3].bPressed) AddRandomBalls(10, 50, 150);    // add large balls
      if (m_Keys[KEY_4].bPressed) AddRandomBalls(1, 300, 500);    // add one massive ball
      if (m_Keys[KEY_L].bPressed) AddLine();    // add one line
      if (m_Keys[KEY_C].bPressed)                                 // clearing balls
      {
         for (int i = 1; i < theBalls.size(); i++)
         {
            delete theBalls[i];
         }
         theBalls.clear();
         // AddBall(m_ScreenWidth / 2.0f, m_ScreenHeight / 2.0f, &ballSprite, 50);
      }

      // detecting which circle (if any) has been selected and setting our selected
      // ball pointer to said ball
      for (auto& i : theBalls)
      {
         if ((m_Mouse[BUTTON_1].bPressed || m_Mouse[BUTTON_2].bPressed) && CircleSelected(i))
         {
            selectedBall = i;
            break;
         }
      }

      // detecting if a line endpoint has been selected
      // bSelectedLineStart will refer to whether the start or endpoint
      // has bee selected
      for (auto& line : theLines)
      {
         if (m_Mouse[BUTTON_1].bPressed)
         {
            if (CircleSelected(line->sx, line->sy, line->radius))
            {
               selectedLine = line;
               bSelectedLineStart = true;
               break;
            }
            else if (CircleSelected(line->ex, line->ey, line->radius))
            {
               selectedLine = line;
               bSelectedLineStart = false;
               break;
            }
         }
      }
      // if we have a selected ball, we operate on it according to which
      // mouse button is pressed
      if (selectedBall != nullptr)
      {
         // left mouse button for static collisions
         if (m_Mouse[BUTTON_1].bHeld)
         {
            selectedBall->vx = 0;
            selectedBall->vy = 0;

            selectedBall->x = m_MousePosition.x;
            selectedBall->y = m_MousePosition.y;
         }
         // right mouse button for dynamic collisions
         // tell the renderer to render the 'pool cue'
         else if (m_Mouse[BUTTON_2].bHeld)
         {
            renderline = true;
         }
         // once right mouse button released, launch the selected ball
         else if (m_Mouse[BUTTON_2].bReleased)
         {
            selectedBall->vx = velocityFactor * (m_MousePosition.x - selectedBall->x);
            selectedBall->vy = velocityFactor * (m_MousePosition.y - selectedBall->y);
            selectedBall = nullptr;
            renderline = false;
         }
      }
      // if selectedball is null, we then check for a selected line and move it
      else if (selectedLine != nullptr)
      {
         if (m_Mouse[BUTTON_1].bHeld)
         {
            if (bSelectedLineStart)
            {
               selectedLine->sx = m_MousePosition.x;
               selectedLine->sy = m_MousePosition.y;
            }
            else
            {
               selectedLine->ex = m_MousePosition.x;
               selectedLine->ey = m_MousePosition.y;
            }
         }
      }
      // setting nullptrs when button is released
      if (m_Mouse[BUTTON_1].bReleased)
      {
         selectedBall = nullptr;
         selectedLine = nullptr;
      }

      //==========================================================================//


      // main simulation body for physics and collision resolutions
      for (int i = 0; i < numSimulationUpdates; i++)
      {
         PhysicsSim(elapsedTime);
         CollisionSim(collisionLog);
         collisionLog.clear();
         collisionLog.reserve(startingNumBalls);
      }


      // resolving final ball positions after sims
      for (ball* i : theBalls)
      {
         m_Renderer->submitEntity(i->x, i->y, i->radius, i->radius, 0.0f, &ballSprite);
      }

      // renders a 'pool cue' line for launching a ball
      if (renderline && selectedBall)
         m_Renderer->submitLine(m_MousePosition.x, m_MousePosition.y, selectedBall->x, selectedBall->y, 10101010, 5);

      // render lines
      for (auto& line : theLines)
      {
         float lineThickness = 5.0f;
         float nx = -(line->ey - line->sy);
         float ny = (line->ex - line->sx);
         float d = sqrt(nx * nx + ny * ny);

         nx = nx / d * (line->radius - lineThickness/2.0f);
         ny = ny / d * (line->radius - lineThickness/2.0f);

         m_Renderer->submitEntity(line->sx, line->sy, line->radius, line->radius, 0.0f, &ballSprite);
         m_Renderer->submitEntity(line->ex, line->ey, line->radius, line->radius, 0.0f, &ballSprite);
         m_Renderer->submitLine(line->sx + nx, line->sy + ny, line->ex + nx, line->ey + ny, 10101010, lineThickness);
         m_Renderer->submitLine(line->sx - nx, line->sy - ny, line->ex - nx, line->ey - ny, 10101010, lineThickness);

      }

      // clearing our fake balls vector
      for (auto& fakeBall : fakeBalls)
      {
         delete fakeBall;
      }
      fakeBalls.clear();

      // menu stuff
      int size = theBalls.size();
      std::pair<float, float> debugInfo = m_DebugAPI->getMemoryUsage();
      ImGui::Begin("Circle Simulation", 0, ImGuiWindowFlags_AlwaysAutoResize);
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::Text("Press F3 for control information");
      ImGui::Text("Press F4 for application info");
      ImGui::InputInt("Number of balls", &size);
      ImGui::SliderFloat("Light Intensity", &intensity, 0, 2.0f);;
      ImGui::SliderFloat("Gravity", &gravity, 0.0f, 20.0f);
      ImGui::DragFloat("V_MEMORY%", &debugInfo.first);
      ImGui::DragFloat("P_MEMORY%", &debugInfo.second);
      ImGui::SetWindowFontScale(1.5f);
      ImGui::End();

      // display control information
      if (m_Keys[KEY_F3].bPressed)
         renderControlInfo = !renderControlInfo;

      if (renderControlInfo)
      {
         ImGui::Begin("Control Information", 0, ImGuiWindowFlags_AlwaysAutoResize);
         ImGui::Text("1-2-3: Create circles of size small, medium, large");
         ImGui::Text("  C  : Clear circles");
         ImGui::Text("  L  : Create new line object");
         ImGui::Text("Right click and hold to launch a circle");
         ImGui::Text("Left click and hold to drag circles (includes lines)");
         ImGui::SetWindowFontScale(1.5f);
         ImGui::End();
      }

      // display control information
      if (m_Keys[KEY_F4].bPressed)
         renderAbout = !renderAbout;

      if (renderAbout)
      {
         ImGui::Begin("Application info", 0, ImGuiWindowFlags_AlwaysAutoResize);
         ImGui::Text("Authored by Zach Brock - 6/19/2021");
         ImGui::Text("This is a sample application created using the");
         ImGui::Text("Legit Game Engine, an OpenGL graphics and game");
         ImGui::Text("programming API that handles all of the rendering.");
         ImGui::Text("");
         ImGui::Text("This simulation approximates circle collisions by");
         ImGui::Text("using 1D conservation of momentum and several");
         ImGui::Text("time steps per frame.");
         ImGui::SetWindowFontScale(1.5f);
         ImGui::End();
      }

      // shader stuff
      if (theBalls.size() > 0)
         m_Shader->setUniform2f("light_pos", Vec2(2.0f * theBalls[0]->x / m_ScreenWidth - 1.0f, 2.0f * theBalls[0]->y / m_ScreenHeight - 1.0f));
      m_Shader->setUniform1f("light_level", intensity);
      return true;
   }
};


int main()
{
   int screenWidth = 2000, screenHeight = 1500;
   double mouseX = 0, mouseY = 0;
   const char* appName = "Circle Physics Simulation";

   CircleCollisions game(appName, screenWidth, screenHeight);

   game.Start();

   return 0;
}