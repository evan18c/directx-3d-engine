#include "Game/Player.h"
#include "Engine/Core/Window.h"
#include "Engine/Graphics/Model.h"
#include "Engine/Core/Renderer.h"
#include <stdio.h>

Player::Player(Camera *camera) {
    m_position = { 0.0f, 0.0f, 0.0f };
    m_velocity = { 0.0f, 0.0f, 0.0f };
    m_gravity = 0.5f;
    m_speed = 5.0f;
    m_jump = 0.25f;
    m_grounded = false;
    m_camera = camera;
    m_camera->m_keyboardControls = false;
}

// Called Every Frame, Handles Movement + Collision
void Player::update() {

    // Calculating Look Directions
    Vec3 forward = Normalize({m_camera->m_look.x - m_camera->m_position.x, 0.0f, m_camera->m_look.z - m_camera->m_position.z});
    Vec3 right = Normalize(Cross({0.0f, 1.0f, 0.0f}, forward));

    // Storing Old Position
    Vec3 oldPosition = m_position;

    // Reset Horizontal Velocity
    m_velocity.x = 0.0f;
    m_velocity.z = 0.0f;

    // Calculating Horizontal Movement
    if (Window::s_keys['W']) m_velocity = Add(m_velocity, Mul(forward, m_speed * Window::s_delta));
    if (Window::s_keys['S']) m_velocity = Sub(m_velocity, Mul(forward, m_speed * Window::s_delta));
    if (Window::s_keys['D']) m_velocity = Add(m_velocity, Mul(right, m_speed * Window::s_delta));
    if (Window::s_keys['A']) m_velocity = Sub(m_velocity, Mul(right, m_speed * Window::s_delta));

    // Calculating Vertical Movement
    if (Window::s_keys[' '] && m_grounded) {
        m_grounded = false;
        m_velocity.y = m_jump;
    }

    // Calculating Gravity
    m_velocity.y -= m_gravity * Window::s_delta;

    // Applying X Velocity
    m_position.x += m_velocity.x;
    for (Model *model : *Window::s_renderer->m_modelList) {
        if (!getAABB().Intersects(model->getAABB())) continue;
        for (int i=0; i<model->m_mesh->m_triangles.size(); i += 3) {
            Vec3 v0 = TransformPoint(model->m_mesh->m_triangles.at(i), model->transform());
            Vec3 v1 = TransformPoint(model->m_mesh->m_triangles.at(i + 1), model->transform());
            Vec3 v2 = TransformPoint(model->m_mesh->m_triangles.at(i + 2), model->transform());
            if (AABBvsTriangle(getAABB(), v0, v1, v2)) {
                m_position.x -= m_velocity.x;
                break;
            }
        }
    }

    // Applying Y Velocity
    m_position.y += m_velocity.y;
    for (Model *model : *Window::s_renderer->m_modelList) {
        if (!getAABB().Intersects(model->getAABB())) continue;
        for (int i=0; i<model->m_mesh->m_triangles.size(); i += 3) {
            Vec3 v0 = TransformPoint(model->m_mesh->m_triangles.at(i), model->transform());
            Vec3 v1 = TransformPoint(model->m_mesh->m_triangles.at(i + 1), model->transform());
            Vec3 v2 = TransformPoint(model->m_mesh->m_triangles.at(i + 2), model->transform());
            if (AABBvsTriangle(getAABB(), v0, v1, v2)) {
                m_position.y -= m_velocity.y;
                if (m_velocity.y < 0.0f) {
                    m_velocity.y = 0.0f;
                    m_grounded = true;
                }
                break;
            }
        }
    }

    // Applying Z Velocity
    m_position.z += m_velocity.z;
    for (Model *model : *Window::s_renderer->m_modelList) {
        if (!getAABB().Intersects(model->getAABB())) continue;
        for (int i=0; i<model->m_mesh->m_triangles.size(); i += 3) {
            Vec3 v0 = TransformPoint(model->m_mesh->m_triangles.at(i), model->transform());
            Vec3 v1 = TransformPoint(model->m_mesh->m_triangles.at(i + 1), model->transform());
            Vec3 v2 = TransformPoint(model->m_mesh->m_triangles.at(i + 2), model->transform());
            if (AABBvsTriangle(getAABB(), v0, v1, v2)) {
                m_position.z -= m_velocity.z;
                break;
            }
        }
    }

    // Move Camera To Player
    m_camera->m_position = m_position;

}

// Returns Current AABB Position
AABB Player::getAABB() {
    float halfSize = 0.5f;
    return {
        {m_position.x - halfSize, m_position.y - halfSize * 4, m_position.z - halfSize},
        {m_position.x + halfSize, m_position.y, m_position.z + halfSize}
    };
}