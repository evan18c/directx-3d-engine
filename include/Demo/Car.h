#pragma once
#include "Engine/Engine.h"

// Represents a car that you are and can drive
class Car {

    public:
        Car(Renderer *renderer, Camera *camera);
        void update();
        Vec3 m_position;
        Vec3 m_rotation;
        float m_speed;

    private:
        Renderer *m_renderer;
        Camera *m_camera;
        Model *m_model;
        Vec3 m_facing;

};