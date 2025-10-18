// Engine
#include "Engine/Engine.h"

// ImGui
#include "imgui.h"

// Demo Stuff
#include "Map.h"
#include "Car.h"

int main() {

    // Main Objects
    Window window = Window(1280, 720, "Window");
    Renderer *renderer = window.s_renderer;
    Camera *camera = renderer->m_camera;

    // Map Object
    Map *map = new Map(renderer);

    // Car Object
    Car *car = new Car(renderer, camera);

    // Render Callback
    renderer->m_renderCallback = [&]() {
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(300, 150), ImGuiCond_Once);
        ImGui::Begin("Debug");
        ImGui::Text("FPS: %.0f", 1.0f / Window::s_delta);
        ImGui::Text("Pitch: %f", camera->m_pitch);
        ImGui::End();
    };

    // Main Game Loop
    while (window.update()) {
        map->update();
        car->update();
    }

    // Exit
    return 0;

}