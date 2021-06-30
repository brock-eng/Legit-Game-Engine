#include <chrono>
#include <math.h>

#include "LegitEngineCore/legit_engine.h"
#include "LegitEngineCore/src/Components/vec2.h"

#ifndef maximum
#define maximum(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef mininmum
#define minimum(a,b)            (((a) < (b)) ? (a) : (b))
#endif

using namespace legit_engine;
using namespace renderables;
using namespace components;
using namespace std;

// global physics constants
const float defaultRadius = 16.0f;
const int timeStep = 0;
const int startingNumBalls = 20;
const float frictionFactor = -0.45;
const float velocityFactor = -5.0f;
const float massFactor = 10.0f;

class CircleCollisions : public Application
{
public:

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

      bool operator==(sLineSegment line)
      {
         return (sx == line.sx && sy == line.sy && ex == line.ex && ey == line.ey);
      }
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

   // selected object pointers
   ball* selectedBall = nullptr;
   sLineSegment* selectedLine = nullptr;

   // containers for our simulation data
   vector <ball*> theBalls;
   vector <collision> collisionLog;
   vector <sLineSegment*> theLines;
   vector <ball*> fakeBalls;

   // assorted simulation control variables
   std::chrono::system_clock::time_point prevTime = chrono::system_clock::now();
   std::chrono::system_clock::time_point currTime = chrono::system_clock::now();
   float elapsedTime;
   float intensity = 0.5f;
   bool renderline = false;
   bool bSelectedLineStart = false;
   float gravity;
   int numSimulationUpdates = 4;
   float lineRadius = 25.f;
   bool renderControlInfo = false;
   bool renderAbout = false;
   bool dynamicLighting = false;
   bool exitApplication;

   // game textures
   Texture ballSprite = Texture("res/circle.png");
   Texture ballLineSprite = Texture("res/circle_line.png");

private:
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
      theLines.push_back(new sLineSegment{ m_ScreenWidth / 4.0f, m_ScreenHeight / 2.0f, 3.0f * m_ScreenWidth / 4.0f, m_ScreenHeight / 2.0f, lineRadius });
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

   // detects and resolves both static and dynamic collisions
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

                  float distance = sqrtf((currBall->x - targetBall->x) * (currBall->x - targetBall->x) +
                     (currBall->y - targetBall->y) * (currBall->y - targetBall->y));
                  float overlap = distance - currBall->radius - targetBall->radius;
                  collisionLog.emplace_back(currBall, targetBall);


                  // resolving static collision, move both balls away by half of overlap
                  currBall->x -= overlap * 0.5f * (currBall->x - targetBall->x) / distance;
                  currBall->y -= overlap * 0.5f * (currBall->y - targetBall->y) / distance;

                  targetBall->x += overlap * 0.5f * (currBall->x - targetBall->x) / distance;
                  targetBall->y += overlap * 0.5f * (currBall->y - targetBall->y) / distance;
               }
            }
         }

         // checking for collisions on the line
         for (sLineSegment* line : theLines)
         {
            // Check that line formed by velocity vector, intersects with line segment
            float fLineX1 = line->ex - line->sx;
            float fLineY1 = line->ey - line->sy;

            float fLineX2 = currBall->x - line->sx;
            float fLineY2 = currBall->y - line->sy;

            float fEdgeLength = fLineX1 * fLineX1 + fLineY1 * fLineY1;

            // This effectively calculates the 'shadow' casted on the line by the ball
            // finding the closest point to the ball that is on the line
            float t = maximum(0, minimum(fEdgeLength, (fLineX1 * fLineX2 + fLineY1 * fLineY2))) / fEdgeLength;
            float fClosestPointX = line->sx + t * fLineX1;
            float fClosestPointY = line->sy + t * fLineY1;

            // distance from ball to closest point on the line
            float fDistance = sqrtf((currBall->x - fClosestPointX) * (currBall->x - fClosestPointX) +
               (currBall->y - fClosestPointY) * (currBall->y - fClosestPointY));


            // checking for overlap
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
      // resolving dynamic collisions
      if (collisionLog.size() > 0)
      {
         for (auto& collision : collisionLog)
         {
            ball* b1 = collision.b1;
            ball* b2 = collision.b2;
            float fDistance = sqrtf((collision.b1->x - collision.b2->x) * (collision.b1->x - collision.b2->x) +
               (collision.b1->y - collision.b2->y) * (collision.b1->y - collision.b2->y));
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

   // modifying each balls physics values
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

         // clamp to zero at low enough velocity
         if (fabs(currBall->vx * currBall->vx + currBall->vy * currBall->vy) < 0.05f)
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

   // getting user input and getting the current selected ball if it exists
   bool HandleInput()
   {
      if (m_Keys[KEY_1].bPressed) AddRandomBalls(10, 5, 10);      // add small balls
      if (m_Keys[KEY_2].bPressed) AddRandomBalls(10, 15, 30);     // add medium balls
      if (m_Keys[KEY_3].bPressed) AddRandomBalls(10, 40, 80);     // add large balls
      if (m_Keys[KEY_4].bPressed) AddRandomBalls(1, 100, 200);    // add one massive ball
      if (m_Keys[KEY_L].bPressed) AddLine();                      // add one line
      if (m_Keys[KEY_X].bPressed)                                 // clear all on screen lines
      {
         for (int i = 0; i < theLines.size(); i++)
         {
            delete theLines[i];
         }
         theLines.clear();
      }
      if (m_Keys[KEY_C].bPressed) // clear all on screen balls
      {
         for (int i = 1; i < theBalls.size(); i++)
         {
            delete theBalls[i];
         }
         theBalls.clear();
      }

      if (m_Keys[KEY_R].bPressed) // reset and create the 'waterfall' program
      {
         for (int i = 0; i < theLines.size(); i++)
         {
            delete theLines[i];
         }
         theLines.clear();
         for (int i = 1; i < theBalls.size(); i++)
         {
            delete theBalls[i];
         }
         theBalls.clear();
         WaterfallProgram();
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

      // toggle fullscreen
      if (m_Keys[KEY_F2].bPressed)
         SetFullScreen();

      // exit application
      if (m_Keys[KEY_ESCAPE].bPressed)
         return false;

      return true;
   }

   void DisplayMenus()
   {
      // toggle the display control information gui
      if (m_Keys[KEY_I].bPressed)
         renderControlInfo = !renderControlInfo;

      // toggle the control information gui
      if (m_Keys[KEY_A].bPressed)
         renderAbout = !renderAbout;

      // main menu
      int size = theBalls.size();
      ImGui::Begin("Circle Collision Simulation", 0, ImGuiWindowFlags_AlwaysAutoResize);
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::Text("Press 'I' to show/hide control information");
      ImGui::Text("Press 'A' to show/hide application info");
      ImGui::Text("Number of balls: %d", size);
      ImGui::Checkbox("Dynamic Lighting", &dynamicLighting);
      if (dynamicLighting) { ImGui::SliderFloat("Light Intensity", &intensity, 0, 2.0f); }
      ImGui::SliderFloat("Gravity", &gravity, 0.0f, 20.0f);
      ImGui::SetWindowFontScale(1.25f);
      ImGui::End();

      // controls and button mappings
      if (renderControlInfo)
      {
         ImGui::Begin("Control Information", 0, ImGuiWindowFlags_AlwaysAutoResize);
         ImGui::Text("1-2-3-4: Create circles of size small-medium-large-XL");
         ImGui::Text("  L  : Create new line object");
         ImGui::Text("  C  : Clear circles");
         ImGui::Text("  X  : Clear lines");
         ImGui::Text("Right click and hold to launch a circle");
         ImGui::Text("Left click and hold to drag circles (including endpoints for lines)");
         ImGui::SetWindowFontScale(1.25f);
         ImGui::End();
      }

      // about page
      if (renderAbout)
      {
         ImGui::Begin("Application info", 0, ImGuiWindowFlags_AlwaysAutoResize);
         ImGui::Text("Authored by Zach Brock - 3/19/2021");
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
   }

   void WaterfallProgram() 
   {
      AddRandomBalls(300, 5, 10);

      gravity = 2.0f;

      theLines.push_back(new sLineSegment{ 0, 0, 0, m_ScreenHeight, lineRadius });
      theLines.push_back(new sLineSegment{ m_ScreenWidth, 0, m_ScreenWidth, m_ScreenHeight, lineRadius });

      theLines.push_back(new sLineSegment{ 0, 0.9f * m_ScreenHeight, 0.85f * m_ScreenWidth, 0.80f * m_ScreenHeight, lineRadius });
      theLines.push_back(new sLineSegment{ m_ScreenWidth, 0.6f * m_ScreenHeight, 0.20f * m_ScreenWidth, 0.55f * m_ScreenHeight , lineRadius });
      theLines.push_back(new sLineSegment{ 0, 0.4f * m_ScreenHeight, 0.85f * m_ScreenWidth, 0.30f * m_ScreenHeight, lineRadius });
      theLines.push_back(new sLineSegment{ m_ScreenWidth, 0.15f * m_ScreenHeight, 0.20f * m_ScreenWidth, 0.05f * m_ScreenHeight , lineRadius });
   }

public:
   bool OnUserCreate() override
   {

      // allocating and creating our game objects
      theBalls.reserve(startingNumBalls);
      collisionLog.reserve(startingNumBalls);
      
      // setting up a cool physics demo that gives off a waterfall type appearance
      WaterfallProgram();

      // shader default settings
      mat4 ortho = mat4::orthographic(0, m_ScreenWidth, 0, m_ScreenHeight, -1.0f, 1.0f);
      m_Shader->setUniformMat4("pr_matrix", ortho);
      m_Shader->setUniform4f("colour", Vec4(0.0, 0.3, 0.8, 1.0));
      m_Shader->setUniform1f("light_level", 1.0f);

      // background color
      glClearColor(0, 0, 0, 0);

      return true;
   }

   // main game loop
   bool OnUserUpdate() override
   {
      // update orthographic matrix in shader depending on screen size
      mat4 ortho = mat4::orthographic(0, m_ScreenWidth, 0, m_ScreenHeight, -1.0f, 1.0f);
      m_Shader->setUniformMat4("pr_matrix", ortho);

      // handling clock and time information
      currTime = chrono::system_clock::now();
      chrono::duration<float> elapsedTimeCalc = currTime - prevTime;
      elapsedTime = elapsedTimeCalc.count();
      prevTime = currTime;
      elapsedTime /= (float)numSimulationUpdates;

      // get all user input (if ImGui is not active)
      if (!ImGui::GetIO().WantCaptureMouse)
         exitApplication = HandleInput();

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

         nx = nx / d * (line->radius - lineThickness / 2.0f);
         ny = ny / d * (line->radius - lineThickness / 2.0f);

         m_Renderer->submitLine(line->sx, line->sy, line->ex, line->ey, 10101010, line->radius);
      }
      for (auto& line : theLines)
      {
         m_Renderer->submitEntity(line->sx, line->sy, line->radius, line->radius, 0.0f, &ballLineSprite);
         m_Renderer->submitEntity(line->ex, line->ey, line->radius, line->radius, 0.0f, &ballLineSprite);
      }

      // clearing the 'fake' balls vector (for ball against line collision)
      for (auto& fakeBall : fakeBalls)
      {
         delete fakeBall;
      }
      fakeBalls.clear();

      // setting the light position for dynamic lighting
      // this is attached to the position of the first ball in theBalls vector
      if (dynamicLighting)
      {
         m_Shader->setUniform1i("dynamic_lighting", 1);
         if (theBalls.size() > 0)
            m_Shader->setUniform2f("light_pos", Vec2(2 * theBalls[0]->x / m_ScreenWidth - 1.0f, 2 * theBalls[0]->y / m_ScreenHeight - 1.0f));

         m_Shader->setUniform1f("light_level", intensity);
      }
      else
         m_Shader->setUniform1i("dynamic_lighting", 0);

      // display application guis
      DisplayMenus();

      return exitApplication;
   }
};


int main()
{
   int screenWidth = 1200, screenHeight = 800;
   double mouseX = 0, mouseY = 0;
   const char* appName = "Circle Physics Simulation";

   CircleCollisions game;// appName, screenWidth, screenHeight);

   game.Construct(appName, screenWidth, screenHeight);

   game.Start();

   return 0;
}