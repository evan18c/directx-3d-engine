#include "Engine/Engine.h"

Light::Light(Vec3 position, Vec3 color, float intensity, float radius) {
    m_type = ObjectType::LIGHT;
    m_position = position;
    m_color = color;
    m_intensity = intensity;
    m_radius = radius;
}

Light *Light::create(Vec3 position, Vec3 color, float intensity, float radius) {
    return new Light(position, color, intensity, radius);
}