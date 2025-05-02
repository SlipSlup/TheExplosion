
#include "window.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include "application.hpp"
#include "OpenGL/shader_program.hpp"
#include "OpenGL/vertex_buffer.hpp"
#include "OpenGL/vertex_array.hpp"

namespace TheExplosion {

    static bool s_GLFW_initialized = false;
    bool escape = false;

    GLfloat points[] = {

         0.0f,  0.1f, 0.0f,
         0.1f, -0.1f, 0.0f,
        -0.1f, -0.1f, 0.0f

    };

    GLfloat colors[] = {

        0.5f, 0.5f, 0.0f,
        0.0f, 0.5f, 0.5f,
        0.5f, 0.0f, 0.5f

    };

    const char* vertex_shader = "#version 460\n"
                                " "
                                "layout(location = 0) in vec3 vertex_position;"
                                "layout(location = 1) in vec3 vertex_color;"
                                "out vec3 color;"
                                " "
                                "void main() {"
                                " "
                                "   color = vertex_color;"
                                "   gl_Position = vec4(vertex_position, 1.0);"
                                " "
                                "}";

    const char* fragment_shader =   "#version 460\n"
                                    " "
                                    "in vec3 color;"
                                    "out vec4 frag_color;"
                                    " "
                                    "void main() {"
                                    " "
                                    "   frag_color = vec4(color, 1.0);"
                                    " "
                                    "}";

    std::unique_ptr<ShaderProgram> p_shader_program;
    std::unique_ptr<VertexBuffer> p_points_vbo;
    std::unique_ptr<VertexBuffer> p_colors_vbo;
    std::unique_ptr<VertexArray> p_vao;

    Window::Window(std::string title, const unsigned int width, const unsigned int height) : m_data({ std::move(title), width, height }) {
        
        int resultCode = init();
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplOpenGL3_Init();
        ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);
    
    }

	Window::~Window() { shutdown(); }

	int Window::init() {

        glfwInit();
        glfwWindowHint(GLFW_DECORATED, false);
        m_pWindow = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_pWindow);
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        glfwSetWindowUserPointer(m_pWindow, &m_data);

        glfwSetWindowCloseCallback(
        
            m_pWindow,
            
            [](GLFWwindow* pWindow) {

                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                EventWindowClose event;
                data.eventCallbackFn(event);

            }
        
        );

        glfwSetFramebufferSizeCallback(

            m_pWindow,

            [](GLFWwindow* pWindow, int width, int height) { glViewport(0, 0, width, height); }

        );

        p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
        p_points_vbo = std::make_unique<VertexBuffer>(points, sizeof(points));
        p_colors_vbo = std::make_unique<VertexBuffer>(colors, sizeof(colors));
        p_vao = std::make_unique<VertexArray>();
        p_vao->add_buffer(*p_points_vbo);
        p_vao->add_buffer(*p_colors_vbo);

        return 0;

	}

	void Window::shutdown() {

        glfwDestroyWindow(m_pWindow);
        glfwTerminate();

	}

	void Window::on_update() {
        
        glClearColor(m_background_color[0], m_background_color[1], m_background_color[2], 1);
        glClear(GL_COLOR_BUFFER_BIT);
        p_shader_program->bind();
        p_vao->bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = static_cast<float>(get_width());
        io.DisplaySize.y = static_cast<float>(get_height());
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if(ImGui::IsKeyPressed(ImGuiKey_Escape)) escape = !escape;

        if(escape) {

            ImGui::SetNextWindowSize(ImVec2(185, 90));
            ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
            ImGui::Begin("Exit", (bool *)0, ImGuiWindowFlags_NoDecoration + ImGuiWindowFlags_NoMove);
            ImGui::SetCursorPos(ImVec2(15, 15));
            ImGui::Text("Do you want to leave?");
            ImGui::GetStyle().WindowTitleAlign = ImVec2(0.5, 0.5);
            ImGui::SetCursorPos(ImVec2(80, 55));
            if(ImGui::Button("Yes")) Escape();
            ImGui::End();

        }

        ImGui::ShowDemoWindow();

        ImGui::Begin("Background");
        ImGui::ColorEdit3("Background Color", m_background_color);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();

	}

}
