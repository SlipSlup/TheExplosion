
#include <application.hpp>
#include <imgui/imgui.h>

class TheExplosionGame : public TheExplosion::Application {
	
	virtual void on_update() override {}

	virtual void on_ui_draw() override {

        ImGui::Begin("Menu");
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

        ImGui::Text("Camera");

        ImGui::SliderFloat3(

            "Camera Position",
            camera_position,
            -10.0f,
            10.0f

        );

        ImGui::SliderFloat3(

            "Camera Rotation",
            camera_rotation,
            -180.0f,
            180.0f

        );

        ImGui::End();

	}

};

const char* game_name = "The Explosion";

int main() {

    auto app = std::make_unique<TheExplosionGame>();
	app->start(game_name);

}
