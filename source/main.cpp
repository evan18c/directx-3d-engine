#include <stdio.h>

// Engine
#include "Engine/Engine.h"

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
    Mesh *mesh = renderer->createMesh("assets/meshes/terrain.obj");
    Texture *texture = renderer->createTexture("assets/textures/terrain.bmp");
    Shader *shader = renderer->createShader("assets/shaders/vs.hlsl", "assets/shaders/ps.hlsl");

    // Models
    Model *model = renderer->createModel(mesh, texture, shader);

    // Main Game Loop
    while (window.update()) {

        renderer->renderModel(model);

        player->update();

    }

    return 0;

}

// TODO: finish adding collision, aabb vs triangle in Player class against world meshes.