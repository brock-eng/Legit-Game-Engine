#pragma once

#define LOG(x) (std::cout << x << std::endl)

#define FR (float_rand(-1, 1))

float float_rand(float b, float e)
{
   return ((float)rand() / RAND_MAX) * (e - b) + b;
}


void bootGui(legit_engine::graphics::Window* window)
{
   IMGUI_CHECKVERSION();
   ImGui::CreateContext();
   ImGuiIO& io = ImGui::GetIO(); (void)io;
   ImGui::StyleColorsDark();
   ImGui_ImplGlfw_InitForOpenGL(window->getWindowPointer(), true);
   ImGui_ImplOpenGL3_Init();
   ImGui::GetStyle().ScaleAllSizes(2.0f);
}
