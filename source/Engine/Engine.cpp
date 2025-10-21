#include "Engine/Engine.h"

// Static Members
Window *Engine::window = nullptr;
Renderer *Engine::renderer = nullptr;
Camera *Engine::camera = nullptr;
Scene *Engine::activeScene = nullptr;

// Instantiate Static Members
void Engine::create() {
    window = new Window(1280, 720, "Game");
    renderer = new Renderer(window->m_hwnd, window->m_width, window->m_height);
    camera = new Camera();
}

// Start Main Loop + Scene
void Engine::run(Scene *scene) {

    // Scene Management
    activeScene = scene;
    activeScene->onStart();

    // Main Engine Loop
    while (window->update()) {
        
        // Start Frame
        renderer->beginFrame();

        // Updating Scene
        activeScene->onUpdate(window->s_delta);
        for (Object *object : activeScene->m_objects) {
            object->update(window->s_delta);
        }

        // Update Camera
        camera->update();

        // Render
        for (Object *object : activeScene->m_objects) {
            object->render(renderer);
        }

        // Post Frame
        renderer->endFrame();

    }
}