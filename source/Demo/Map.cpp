#include "Map.h"

Map::Map(Renderer *renderer) {

    // Construction
    m_renderer = renderer;

    // Assets
    Mesh *mesh_ground = renderer->createMesh("../assets/meshes/ground.obj");
    Texture *texture_ground = renderer->createTexture("../assets/textures/ground.bmp");
    Shader *shader3D = renderer->createShader("../assets/shaders/3dvs.hlsl", "../assets/shaders/3dps.hlsl", Layout::MODEL);

    // Models
    m_models.push_back(renderer->createModel(mesh_ground, texture_ground, shader3D));
}

void Map::update() {

    // Render All Map Models
    for (Model *model : m_models) {
        m_renderer->renderModel(model);
    }

}