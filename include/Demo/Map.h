#pragma once
#include <vector>
#include "Engine/Engine.h"

// The Game World
class Map {

    public:
        Map(Renderer *renderer);
        void update();

    private:
        Renderer *m_renderer;
        std::vector<Model *> m_models;

};