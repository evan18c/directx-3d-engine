#pragma once
#include "Engine/Math/Maths.h"
#include "Engine/Graphics/Camera.h"

// Player class, contains all player information for an fps game
class Player {

    public:
        Player(Camera *camera);
        Vec3 m_position;
        Vec3 m_velocity;
        float m_gravity;
        float m_jump;
        float m_speed;
        bool m_grounded;
        Camera *m_camera;

        void update();

    private:
        AABB getAABB();

};