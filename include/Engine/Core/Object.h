#pragma once
#include "Engine/Math/Maths.h"
class Scene;
class Renderer;

// Specific Types Of Objects
enum class ObjectType {
    PLAYER,
    MODEL,
    SPRITE,
    LIGHT,
    ENTITY
};

// Base class for all objects in a scene.
class Object {
    public:
        Scene *m_scene = nullptr;
        ObjectType m_type;
        virtual void update(float dt){};
        virtual void render(Renderer *renderer){};
        virtual ~Object(){};
};