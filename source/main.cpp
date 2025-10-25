// Includes
#include "Engine/Engine.h"
#include "imgui.h"

// Demo
class DemoScene : public Scene {
    public:
        Player *player;
        Model *ground;
        Model *halo;
        Sprite *sprite;

        void onStart() override {

            // Assets
            Mesh *mesh1 = Mesh::create("../assets/objects/ground/ground.obj");
            Mesh *mesh2 = Mesh::create("../assets/objects/poa/halo_1campaignmission_1_pillar_of_autumn.obj");
            Shader *shader1 = Shader::create("../assets/shaders/3dvs.hlsl", "../assets/shaders/3dps.hlsl", Layout::MODEL);
            Shader *shader2 = Shader::create("../assets/shaders/2dvs.hlsl", "../assets/shaders/2dps.hlsl", Layout::SPRITE);
            Texture *texture1 = Texture::create("../assets/textures/crosshair.png");

            // Player Object
            player = new Player(Engine::camera);
            player->m_position.x = -20.0f;
            player->m_position.y = 42.0f;
            player->m_position.z = 0.0f;

            // Ground Object
            ground = Model::create(mesh1, shader1);

            // Halo Object
            halo = Model::create(mesh2, shader1);

            // Crosshair
            sprite = Sprite::create(texture1, shader2);
            sprite->m_size.x = 32;
            sprite->m_size.y = 32;

            // Adding Objects To Scene
            this->addObject(player);
            this->addObject(ground);
            this->addObject(halo);
            this->addObject(sprite);

        }

        void onUpdate(float dt) override {

            ImGui::Begin("Debug");
            ImGui::Text("Player Position:");
            ImGui::Text("X: %.0f", player->m_position.x);
            ImGui::Text("Y: %.0f", player->m_position.y);
            ImGui::Text("Z: %.0f", player->m_position.z);
            ImGui::End();

        }
};

int main() {

    Engine::create();
    Engine::run(new DemoScene());

    return 0;
}

/*
TODO:
-add rendering for triangles both sides
-for horizontal movement (x/z axis), calculate step up required when they move into a wall.
-break up m_triangles into chunks and check collision only if ur in the aabb of it
-maybe try messing with shaders for cool lighting

https://imagetostl.com/convert/file/glb/to/obj#convert
https://sketchfab.com/jameslucino117/collections/halo-1-campaign-37211b4caa104c6ca6365f7229df5267
https://sketchfab.com/jameslucino117/collections/halo-2-multiplayer-e14166c2dcbc4e19baf1b57a2a280d93
*/