// Contains All Engine Imports
#pragma once

// Core
#include "Engine/Core/Object.h"
#include "Engine/Core/Renderer.h"
#include "Engine/Core/Scene.h"
#include "Engine/Core/Window.h"

// Game
#include "Engine/Game/Player.h"

// Graphics
#include "Engine/Graphics/Camera.h"
#include "Engine/Graphics/Material.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/Graphics/Shader.h"
#include "Engine/Graphics/Texture.h"

// Math
#include "Engine/Math/Maths.h"

// Objects
#include "Engine/Objects/Light.h"
#include "Engine/Objects/Model.h"
#include "Engine/Objects/Sprite.h"

// Utils
#include "Engine/Utils/Clock.h"
#include "Engine/Utils/Files.h"

// ImGui
#include "imgui.h"

// Static Engine Class
class Engine {
    public:
        static void create();
        static void run(Scene *scene);
        static Window *window;
        static Renderer *renderer;
        static Camera *camera;
        static Scene *activeScene;
};