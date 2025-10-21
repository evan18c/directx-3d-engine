#pragma once
#include <vector>
#include "Engine/Core/Object.h"

class Scene {
    public:
        void addObject(Object *obj);
        virtual void onStart(){};
        virtual void onUpdate(float dt){};
        virtual void onEnd(){};
        std::vector<Object *> m_objects;
};