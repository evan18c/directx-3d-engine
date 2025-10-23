// Includes
#include "Engine/Engine.h"
#include "imgui.h"

// Demo
class DemoScene : public Scene {
    public:
        Player *player;
        Model *model;
        Sprite *sprite;

        void onStart() override {

            // Assets
            Mesh *mesh1 = Mesh::create("../assets/objects/level/level.obj");
            Shader *shader1 = Shader::create("../assets/shaders/3dvs.hlsl", "../assets/shaders/3dps.hlsl", Layout::MODEL);
            Shader *shader2 = Shader::create("../assets/shaders/2dvs.hlsl", "../assets/shaders/2dps.hlsl", Layout::SPRITE);
            Texture *texture1 = Texture::create("../assets/textures/crosshair.png");

            // Player Object
            player = new Player(Engine::camera);
            player->m_position.y = 10.0f;

            // Ground Object
            model = Model::create(mesh1, shader1);

            // Crosshair
            sprite = Sprite::create(texture1, shader2);
            sprite->m_size.x = 32;
            sprite->m_size.y = 32;

            // Adding Objects To Scene
            this->addObject(player);
            this->addObject(model);
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
-add support for png
-for horizontal movement (x/z axis), calculate step up required when they move into a wall.
*/