#pragma once

/*
   Main #include directory for Legit Engine
*/

#include <glew.h>
#include <glfw3.h>

#include <iostream>
#include <stdio.h>

#include "src/core/application.h"

#include "src/utils/ImGui/imgui.h"
#include "src/utils/ImGui/imgui_impl_glfw.h"
#include "src/utils/ImGui/imgui_impl_opengl3.h"

#include "src/Components/Sys.h"
#include "src/utils/debug.h"

#include "src/graphics/window.h"

#include "src/graphics/Renderables/batchrenderer2d.h"
#include "src/graphics/Renderables/pixelrenderer2d.h"

#include "src/graphics/Shaders/shaders.h"

#include "src/graphics/Renderables/sprite.h"

#include "src/graphics/Renderables/texture.h"

#if 0 // only for some internal testing
#include "Buffers/buffer.h"
#include "Buffers/vertexarray.h"
#include "Buffers/indexbuffer.h
#endif

#include "src/input/input.h"

