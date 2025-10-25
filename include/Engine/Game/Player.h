#pragma once
#include "Engine/Core/Object.h"
#include "Engine/Math/Maths.h"
#include "Engine/Graphics/Camera.h"

// Player class
class Player : public Object {
    public:
        Player(Camera *camera);
        void update(float dt) override;
        AABB getAABB();

        Vec3 m_position;
        Vec3 m_velocity;
        float m_gravity;
        float m_jump;
        float m_speed;
        bool m_grounded;
        Camera *m_camera;

};