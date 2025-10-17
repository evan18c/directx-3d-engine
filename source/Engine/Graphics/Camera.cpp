#include <math.h>
#include <stdio.h>
#include "Engine/Graphics/Camera.h"
#include "Engine/Core/Window.h"
#include "Engine/Utils/Clock.h"

// Constructs Camera
Camera::Camera() {
    m_position = {0.0f, 0.0f, -1.0f};
    m_look = {0.0f, 0.0f, 0.0f};
    m_yaw = 0.0f;
    m_pitch = 0.0f;
    m_speed = 5.0f;
    m_sensitivity = 0.2f;
    m_mouseControls = true;
    m_keyboardControls = true;
}

// Handles Controls
void Camera::update() {

    // Mouse Movement
    if (m_mouseControls) {
        m_yaw   += m_sensitivity * Window::s_mdx * Window::s_delta;
        m_pitch -= m_sensitivity * Window::s_mdy * Window::s_delta;
    }

    // Reset Mouse Deltas
    Window::s_mdx = 0;
    Window::s_mdy = 0;

    // Clamping Pitch
    if (m_pitch < -1.5f) m_pitch = -1.5f;
    if (m_pitch >  1.5f) m_pitch =  1.5f;

    // Keyboard Movement
    if (m_keyboardControls) {
        if (Window::s_keys['W']) {
            m_position.x += sin(m_yaw) * m_speed * Window::s_delta;
            m_position.z += cos(m_yaw) * m_speed * Window::s_delta;
        }
        if (Window::s_keys['S']) {
            m_position.x -= sin(m_yaw) * m_speed * Window::s_delta;
            m_position.z -= cos(m_yaw) * m_speed * Window::s_delta;
        }
        if (Window::s_keys['A']) {
            m_position.x -= sin(m_yaw + PI / 2) * m_speed * Window::s_delta;
            m_position.z -= cos(m_yaw + PI / 2) * m_speed * Window::s_delta;
        }
        if (Window::s_keys['D']) {
            m_position.x += sin(m_yaw + PI / 2) * m_speed * Window::s_delta;
            m_position.z += cos(m_yaw + PI / 2) * m_speed * Window::s_delta;
        }
        if (Window::s_keys[VK_SPACE]) {
            m_position.y += m_speed * Window::s_delta;
        }
        if (Window::s_keys[VK_SHIFT]) {
            m_position.y -= m_speed * Window::s_delta;
        }
    }

    // Applying Look
    m_look.x = m_position.x + sin(m_yaw) * cos(m_pitch);
    m_look.y = m_position.y + sin(m_pitch);
    m_look.z = m_position.z + cos(m_yaw) * cos(m_pitch);

}