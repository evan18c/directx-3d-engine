#include "Engine/Engine.h"
#include <stdio.h>

Player::Player(Camera *camera) {
    m_position = { 0.0f, 0.0f, 0.0f };
    m_velocity = { 0.0f, 0.0f, 0.0f };
    m_gravity = 9.8f;
    m_speed = 5.0f;
    m_jump = 5.0f;
    m_grounded = false;
    m_camera = camera;
    m_camera->m_keyboardControls = false;
    m_type = ObjectType::PLAYER;
}

// Called Every Frame, Handles Movement + Collision
void Player::update(float dt) {

    // Calculating Look Directions
    Vec3 forward = Normalize({m_camera->m_look.x - m_camera->m_position.x, 0.0f, m_camera->m_look.z - m_camera->m_position.z});
    Vec3 right = Normalize(Cross({0.0f, 1.0f, 0.0f}, forward));

    // Storing Old Position
    Vec3 oldPosition = m_position;

    // Reset Horizontal Velocity
    m_velocity.x = 0.0f;
    m_velocity.z = 0.0f;

    // Calculating Horizontal Movement
    if (Window::s_keys['W']) m_velocity = Add(m_velocity, Mul(forward, m_speed));
    if (Window::s_keys['S']) m_velocity = Sub(m_velocity, Mul(forward, m_speed));
    if (Window::s_keys['D']) m_velocity = Add(m_velocity, Mul(right,   m_speed));
    if (Window::s_keys['A']) m_velocity = Sub(m_velocity, Mul(right,   m_speed));

    // Calculating Vertical Movement
    if (Window::s_keys[' '] && m_grounded) {
        m_grounded = false;
        m_velocity.y = m_jump;
    }

    // Calculating Gravity
    m_velocity.y -= m_gravity * dt;

    // Applying X Velocity
    m_position.x += m_velocity.x * dt;
    for (Object *object : m_scene->m_objects) {

        // Model Check
        if (object->m_type != ObjectType::MODEL) continue;
        Model *model = static_cast<Model*>(object);

        // AABB Check
        if (!getAABB().Intersects(model->getAABB())) continue;

        // Checking Each Triangle Chunk
        for (TriangleChunk chunk : model->m_mesh->m_chunks) {
            
            // Check if in triangle chunk
            if (!getAABB().Intersects(TransformAABB(chunk.aabb, model->transform()))) continue;

            // Normal Collision:
            for (int i=0; i<chunk.triangles.size(); i += 3) {
                Vec3 v0 = TransformPoint(chunk.triangles[i],   model->transform());
                Vec3 v1 = TransformPoint(chunk.triangles[i+1], model->transform());
                Vec3 v2 = TransformPoint(chunk.triangles[i+2], model->transform());
                if (AABBvsTriangle(getAABB(), v0, v1, v2)) {
                    m_position.x -= m_velocity.x * dt; // undo velocity
                }
            }

        }
    }

    // Applying Y Velocity
    m_position.y += m_velocity.y * dt;
    for (Object *object : m_scene->m_objects) {

        // Model Check
        if (object->m_type != ObjectType::MODEL) continue;
        Model *model = static_cast<Model*>(object);

        // AABB Check
        if (!getAABB().Intersects(model->getAABB())) continue;

        // Checking Each Triangle Chunk
        for (TriangleChunk chunk : model->m_mesh->m_chunks) {
            
            // Check if in triangle chunk
            if (!getAABB().Intersects(TransformAABB(chunk.aabb, model->transform()))) continue;

            // Normal Collision:
            for (int i=0; i<chunk.triangles.size(); i += 3) {
                Vec3 v0 = TransformPoint(chunk.triangles[i],   model->transform());
                Vec3 v1 = TransformPoint(chunk.triangles[i+1], model->transform());
                Vec3 v2 = TransformPoint(chunk.triangles[i+2], model->transform());
                if (AABBvsTriangle(getAABB(), v0, v1, v2)) {
                    m_position.y -= m_velocity.y * dt;
                    if (m_velocity.y < 0.0f) {
                        m_velocity.y = 0.0f;
                        m_grounded = true;
                    }
                }
            }

        }
    }

    // Applying Z Velocity
    m_position.z += m_velocity.z * dt;
    for (Object *object : m_scene->m_objects) {

        // Model Check
        if (object->m_type != ObjectType::MODEL) continue;
        Model *model = static_cast<Model*>(object);

        // AABB Check
        if (!getAABB().Intersects(model->getAABB())) continue;

        // Checking Each Triangle Chunk
        for (TriangleChunk chunk : model->m_mesh->m_chunks) {
            
            // Check if in triangle chunk
            if (!getAABB().Intersects(TransformAABB(chunk.aabb, model->transform()))) continue;

            // Normal Collision:
            for (int i=0; i<chunk.triangles.size(); i += 3) {
                Vec3 v0 = TransformPoint(chunk.triangles[i],   model->transform());
                Vec3 v1 = TransformPoint(chunk.triangles[i+1], model->transform());
                Vec3 v2 = TransformPoint(chunk.triangles[i+2], model->transform());
                if (AABBvsTriangle(getAABB(), v0, v1, v2)) {
                    m_position.z -= m_velocity.z * dt; // undo velocity
                }
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
        {m_position.x - halfSize, m_position.y - halfSize * 3, m_position.z - halfSize},
        {m_position.x + halfSize, m_position.y + halfSize,     m_position.z + halfSize}
    };
}