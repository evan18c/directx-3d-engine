// Engine
#include "Engine/Engine.h"

// ImGui
#include "imgui.h"

int main() {

    // Main Objects
    Window window = Window(1280, 720, "Window");
    Renderer *renderer = window.s_renderer;
    Camera *camera = renderer->m_camera;

    // Player Object
    Player *player = new Player(camera);
    player->m_position.y = 15.0f;
    player->m_position.x = 5.0f;

    // Assets
    Mesh *mesh = renderer->createMesh("../assets/meshes/exam.obj");
    Texture *texture = renderer->createTexture("../assets/textures/checker.bmp");
    Shader *shader3D = renderer->createShader("../assets/shaders/3dvs.hlsl", "../assets/shaders/3dps.hlsl", Layout::MODEL);
    Shader *shader2D = renderer->createShader("../assets/shaders/2dvs.hlsl", "../assets/shaders/2dps.hlsl", Layout::SPRITE);

    // Models
    Model *model = renderer->createModel(mesh, texture, shader3D);

    // Sprites
    Sprite *sprite = renderer->createSprite(texture, shader2D);

    // Render Callback
    renderer->m_renderCallback = [&]() {
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(300, 150), ImGuiCond_Once);
        ImGui::Begin("Debug");
        ImGui::Text("FPS: %.0f", 1.0f / Window::s_delta);
        ImGui::End();
    };

    // Main Game Loop
    while (window.update()) {

        renderer->renderModel(model);
        renderer->renderSprite(sprite);

        player->update();

    }

    return 0;

}