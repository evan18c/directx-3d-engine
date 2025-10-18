#include "Car.h"
#include <math.h>

Car::Car(Renderer *renderer, Camera *camera) {

    // Construction
    m_position = {0.0f, 2.0f, 0.0f};
    m_rotation = {0.0f, 0.0f, 0.0f};
    m_facing = {0.0f, 0.0f, 1.0f};
    m_renderer = renderer;
    m_camera = camera;

    // Load Car Assets
    Mesh *mesh_car = renderer->createMesh("../assets/meshes/car.obj");
    Texture *texture_car = renderer->createTexture("../assets/textures/car.bmp");
    Shader *shader3D = renderer->createShader("../assets/shaders/3dvs.hlsl", "../assets/shaders/3dps.hlsl", Layout::MODEL);
    
    // Create Car Model
    m_model = renderer->createModel(mesh_car, texture_car, shader3D);

}

// Called Every Frame
void Car::update() {

    // Update Model
    m_model->m_position = m_position;
    m_model->m_rotation = m_rotation;

    // Calculating Facing Direction
    m_facing = Normalize({sinf(m_rotation.y), 0.0f, cosf(m_rotation.y)});

    // Controls
    if (Window::s_keys['Q']) m_rotation.y -= 1.0f * Window::s_delta;
    if (Window::s_keys['E']) m_rotation.y += 1.0f * Window::s_delta;
    if (Window::s_keys['R']) m_position = Add(m_position, Mul(m_facing, Window::s_delta));

    // Render Car
    m_renderer->renderModel(m_model);

}