#include "Engine/Core/Scene.h"

void Scene::addObject(Object *obj) {
    obj->m_scene = this;
    m_objects.push_back(obj);
}