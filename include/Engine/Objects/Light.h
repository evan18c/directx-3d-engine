#pragma once
#include "Engine/Math/Maths.h"

class Light : public Object {

    public:
        Vec3 m_position;
        Vec3 m_color;
        float m_intensity;
        float m_radius;

        Light(Vec3 position, Vec3 color, float intensity, float radius);
        static Light *create(Vec3 position, Vec3 color, float intensity, float radius);

};