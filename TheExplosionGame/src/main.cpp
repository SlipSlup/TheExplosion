
#include <application.hpp>
#include <imgui/imgui.h>
#include <string>
#include <input.hpp>

class TheExplosionGame : public TheExplosion::Application {

    float camera_translation_speed = 1.0f;
    float camera_rotation_speed = 1.0f;
    double last_cursor_position[2] = { 0, 0 };
    double current_cursor_position[2] = { 0, 0 };
    int framerate = 0;
    float delay = 0;
    bool cursor_position_got = false;
	
	virtual void on_update() override {

        if(!cursor_position_got) {

            last_cursor_position[0] = get_current_cursor_position().x;
            last_cursor_position[1] = get_current_cursor_position().y;
            cursor_position_got = true;

        }
        
        glm::vec3 movement_delta{ 0, 0, 0 };
        glm::vec3 rotation_delta{ 0, 0, 0 };
        current_cursor_position[0] = get_current_cursor_position().x;
        current_cursor_position[1] = get_current_cursor_position().y;
    
        if(TheExplosion::Input::isKeyPressed(TheExplosion::KeyCode::KEY_W)) { movement_delta.x += (float)(camera_translation_speed * deltaTime); }
        if(TheExplosion::Input::isKeyPressed(TheExplosion::KeyCode::KEY_S)) { movement_delta.x -= (float)(camera_translation_speed * deltaTime); }
        if(TheExplosion::Input::isKeyPressed(TheExplosion::KeyCode::KEY_A)) { movement_delta.y -= (float)(camera_translation_speed * deltaTime); }
        if(TheExplosion::Input::isKeyPressed(TheExplosion::KeyCode::KEY_D)) { movement_delta.y += (float)(camera_translation_speed * deltaTime); }
        
        if(current_cursor_position[0] != last_cursor_position[0]) {

            rotation_delta.z -= (float)((current_cursor_position[0] - last_cursor_position[0]) * camera_rotation_speed);
            last_cursor_position[0] = current_cursor_position[0];
        
        }
        if(current_cursor_position[1] != last_cursor_position[1]) {

            rotation_delta.y += (float)((current_cursor_position[1] - last_cursor_position[1]) * camera_rotation_speed);
            last_cursor_position[1] = current_cursor_position[1];

        }

        if(camera.get_camera_rotation().z > 180) rotation_delta.z -= 360;
        if(camera.get_camera_rotation().z < -180) rotation_delta.z += 360;

        camera.add_movement_rotation(movement_delta, rotation_delta);
    
    }

	virtual void on_ui_draw() override {

        delay += (float)deltaTime;

        if(delay >= 0.5f) {

            framerate = (int)(1 / deltaTime);
            delay = 0;

        }
        
        ImGui::Begin("Menu");
        ImGui::Text(" ");
        ImGui::Text("FPS : ");

        ImGui::SetCursorPos(
            
            ImVec2(
                
                49,
                44
            
            )
        
        );

        ImGui::Text(std::to_string(framerate).c_str());
        ImGui::Text(" ");
        ImGui::Text("Camera Position");
        ImGui::Text("X : ");

        ImGui::SetCursorPos(
            
            ImVec2(
                
                35,
                95
            
            )
        
        );

        ImGui::Text(std::to_string(camera.get_camera_position().x).c_str());
        ImGui::Text("Y : ");

        ImGui::SetCursorPos(
            
            ImVec2(
                
                35,
                112
            
            )
        
        );

        ImGui::Text(std::to_string(camera.get_camera_position().y).c_str());
        ImGui::Text("Z : ");

        ImGui::SetCursorPos(
            
            ImVec2(
                
                35,
                129
            
            )
        
        );

        ImGui::Text(std::to_string(camera.get_camera_position().z).c_str());
        ImGui::Text(" ");
        ImGui::Text("Camera Rotation");
        ImGui::Text("X : ");

        ImGui::SetCursorPos(
            
            ImVec2(
                
                35,
                180
            
            )
        
        );

        ImGui::Text(std::to_string(camera.get_camera_rotation().x).c_str());
        ImGui::Text("Y : ");

        ImGui::SetCursorPos(
            
            ImVec2(
                
                35,
                197
            
            )
        
        );

        ImGui::Text(std::to_string(camera.get_camera_rotation().y).c_str());
        ImGui::Text("Z : ");

        ImGui::SetCursorPos(
            
            ImVec2(
                
                35,
                214
            
            )
        
        );

        ImGui::Text(std::to_string(camera.get_camera_rotation().z).c_str());
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
	app -> start(game_name);

}
