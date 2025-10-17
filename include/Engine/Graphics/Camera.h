#pragma once
#include "Engine/Math/Maths.h"

class Camera {

    public:
        Vec3 m_position;
        Vec3 m_look;
        float m_yaw, m_pitch;
        float m_speed;
        float m_sensitivity;
        bool m_mouseControls;
        bool m_keyboardControls;

    private:
        Camera();
        void update();

    friend class Renderer;

};