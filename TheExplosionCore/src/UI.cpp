
#include "UI.hpp"
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

namespace TheExplosion {

	void UI::on_window_create(GLFWwindow* pWindow) {

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::GetIO().IniFilename = nullptr;
        ImGui_ImplOpenGL3_Init();

        ImGui_ImplGlfw_InitForOpenGL(

            pWindow,
            true

        );

	}

    void UI::on_window_close() {

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

    }

    void UI::on_ui_draw_begin() {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

    }

    void UI::on_ui_draw_end() {

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    }

}