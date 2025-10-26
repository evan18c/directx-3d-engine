// Includes
#include "Engine/Engine.h"
#include "imgui.h"

// Demo
class DemoScene : public Scene {
    public:
        Player *player;
        Model *ground;
        Sprite *crosshair;
        Light *light1;

        void onStart() override {

            // Assets
            Mesh *mesh1 = Mesh::create("../assets/objects/ground/ground.obj");
            Shader *shader1 = Shader::default3D();
            Shader *shader2 = Shader::default2D();
            Texture *texture1 = Texture::create("../assets/textures/crosshair.png");

            // Player Object
            player = new Player(Engine::camera);
            player->m_position.x = 0.0f;
            player->m_position.y = 5.0f;
            player->m_position.z = 0.0f;

            // Ground Object
            ground = Model::create(mesh1, shader1);

            // Lights
            light1 = new Light({0.0f, 5.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, 1.0f, 10.0f);

            // Crosshair
            crosshair = Sprite::create(texture1, shader2);
            crosshair->m_size.x = 32;
            crosshair->m_size.y = 32;

            // Adding Objects To Scene
            this->addObject(player);
            this->addObject(ground);
            this->addObject(light1);
            this->addObject(crosshair);

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
-for horizontal movement (x/z axis), calculate step up required when they move into a wall.

https://imagetostl.com/convert/file/glb/to/obj#convert
https://sketchfab.com/jameslucino117/collections/halo-1-campaign-37211b4caa104c6ca6365f7229df5267
https://sketchfab.com/jameslucino117/collections/halo-2-multiplayer-e14166c2dcbc4e19baf1b57a2a280d93
*/