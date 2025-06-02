
#include <application.hpp>
#include <imgui/imgui.h>
#include <string>
#include <input.hpp>

class TheExplosionGame : public TheExplosion::Application {

    float camera_translation_speed = 1.0f;
    float camera_rotation_speed = 100.0f;
    float camera_position[3] = { 0.0f, 0.0f, 0.0f };
    float camera_rotation[3] = { 0.0f, 0.0f, 0.0f };
	
	virtual void on_update() override {

        glm::vec3 movement_delta{ 0, 0, 0 };
        glm::vec3 rotation_delta{ 0, 0, 0 };
    
        if(TheExplosion::Input::isKeyPressed(TheExplosion::KeyCode::KEY_W)) { movement_delta.x += camera_translation_speed * deltaTime; }
        if(TheExplosion::Input::isKeyPressed(TheExplosion::KeyCode::KEY_S)) { movement_delta.x -= camera_translation_speed * deltaTime; }
        if(TheExplosion::Input::isKeyPressed(TheExplosion::KeyCode::KEY_A)) { movement_delta.y -= camera_translation_speed * deltaTime; }
        if(TheExplosion::Input::isKeyPressed(TheExplosion::KeyCode::KEY_D)) { movement_delta.y += camera_translation_speed * deltaTime; }
        if(TheExplosion::Input::isKeyPressed(TheExplosion::KeyCode::KEY_UP)) { rotation_delta.y -= camera_rotation_speed * deltaTime; }
        if(TheExplosion::Input::isKeyPressed(TheExplosion::KeyCode::KEY_DOWN)) { rotation_delta.y += camera_rotation_speed * deltaTime; }
        if(TheExplosion::Input::isKeyPressed(TheExplosion::KeyCode::KEY_LEFT)) { rotation_delta.z += camera_rotation_speed * deltaTime; }
        if(TheExplosion::Input::isKeyPressed(TheExplosion::KeyCode::KEY_RIGHT)) { rotation_delta.z -= camera_rotation_speed * deltaTime; }

        camera.add_movement_rotation(movement_delta, rotation_delta);
    
    }

	virtual void on_ui_draw() override {

        camera_position[0] = camera.get_camera_position().x;
        camera_position[1] = camera.get_camera_position().y;
        camera_position[2] = camera.get_camera_position().z;
        camera_rotation[0] = camera.get_camera_rotation().x;
        camera_rotation[1] = camera.get_camera_rotation().y;
        camera_rotation[2] = camera.get_camera_rotation().z;
        
        ImGui::Begin("Menu");
        ImGui::Text(" ");
        ImGui::Text("FPS : ");
        ImGui::SetCursorPos(ImVec2(49, 44));
        ImGui::Text(std::to_string((int)(1 / deltaTime)).c_str());
        ImGui::Text(" ");
        ImGui::Text("Camera Position");
        ImGui::Text("X : ");
        ImGui::SetCursorPos(ImVec2(35, 95));
        ImGui::Text(std::to_string(camera_position[0]).c_str());
        ImGui::Text("Y : ");
        ImGui::SetCursorPos(ImVec2(35, 112));
        ImGui::Text(std::to_string(camera_position[1]).c_str());
        ImGui::Text("Z : ");
        ImGui::SetCursorPos(ImVec2(35, 129));
        ImGui::Text(std::to_string(camera_position[2]).c_str());
        ImGui::Text(" ");

        ImGui::Text("Background");

        ImGui::ColorEdit3(

            "Background Color",
            m_background_color

        );

        ImGui::Text("Square");

        ImGui::SliderFloat3(

            "Square Scale",
            square_scale,
            0.0f,
            2.0f

        );

        ImGui::SliderFloat3(

            "Square Rotation",
            square_rotation,
            -180.0f,
            180.0f

        );

        ImGui::SliderFloat3(

            "Square Translation",
            square_translation,
            -1.0f,
            1.0f

        );

        ImGui::End();

	}

};

const char* game_name = "The Explosion";

int main() {

    auto app = std::make_unique<TheExplosionGame>();
	app->start(game_name);

}
