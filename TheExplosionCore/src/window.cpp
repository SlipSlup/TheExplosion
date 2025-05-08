
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
#include "OpenGL/index_buffer.hpp"

namespace TheExplosion {

    static bool s_GLFW_initialized = false;

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
    std::unique_ptr<VertexBuffer> p_positions_colors_vbo;
    std::unique_ptr<IndexBuffer> p_index_buffer;
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

        GLfloat square_data[] = {

                m_square_angle1_position[0], m_square_angle1_position[1], m_square_angle1_position[2], m_square_angle1_color[0], m_square_angle1_color[1], m_square_angle1_color[2],
                m_square_angle2_position[0], m_square_angle2_position[1], m_square_angle2_position[2], m_square_angle2_color[0], m_square_angle2_color[1], m_square_angle2_color[2],
                m_square_angle3_position[0], m_square_angle3_position[1], m_square_angle3_position[2], m_square_angle3_color[0], m_square_angle3_color[1], m_square_angle3_color[2],
                m_square_angle4_position[0], m_square_angle4_position[1], m_square_angle4_position[2], m_square_angle4_color[0], m_square_angle4_color[1], m_square_angle4_color[2]

        };

        GLuint indices[] = { 0, 1, 2, 3, 2, 1 };

        BufferLayout buffer_layout_2vec3{

            ShaderDataType::Float3,
            ShaderDataType::Float3

        };

        p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
        p_vao = std::make_unique<VertexArray>();
        p_positions_colors_vbo = std::make_unique<VertexBuffer>(square_data, sizeof(square_data), buffer_layout_2vec3);
        p_index_buffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));
        p_vao->add_vertex_buffer(*p_positions_colors_vbo);
        p_vao->set_index_buffer(*p_index_buffer);

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
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(p_vao->get_indices_count()), GL_UNSIGNED_INT, nullptr);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if(ImGui::IsKeyPressed(ImGuiKey_Escape)) leave = !leave;

        if(leave) {

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

        ImGui::Begin("Menu");

        ImGui::Text("Background");

        if(ImGui::CollapsingHeader("Background Color")) { ImGui::ColorEdit3("Background Color", m_background_color); }

        ImGui::Text("Square");

        if(ImGui::CollapsingHeader("Square Colors")) {

            if(ImGui::ColorEdit3("Angle 1 Color", m_square_angle1_color)) square_edit = true;
            if(ImGui::ColorEdit3("Angle 2 Color", m_square_angle2_color)) square_edit = true;
            if(ImGui::ColorEdit3("Angle 3 Color", m_square_angle3_color)) square_edit = true;
            if(ImGui::ColorEdit3("Angle 4 Color", m_square_angle4_color)) square_edit = true;

        }

        if(ImGui::CollapsingHeader("Square Positions")) {

            if(ImGui::DragFloat3("Angle 1 Position", m_square_angle1_position, 0.005f, -1.0f, 1.0f)) square_edit = true;
            if(ImGui::DragFloat3("Angle 2 Position", m_square_angle2_position, 0.005f, -1.0f, 1.0f)) square_edit = true;
            if(ImGui::DragFloat3("Angle 3 Position", m_square_angle3_position, 0.005f, -1.0f, 1.0f)) square_edit = true;
            if(ImGui::DragFloat3("Angle 4 Position", m_square_angle4_position, 0.005f, -1.0f, 1.0f)) square_edit = true;

        }

        ImGui::End();

        if(square_edit) {

            square_edit = false;

            GLfloat square_data[] = {

                m_square_angle1_position[0], m_square_angle1_position[1], m_square_angle1_position[2], m_square_angle1_color[0], m_square_angle1_color[1], m_square_angle1_color[2],
                m_square_angle2_position[0], m_square_angle2_position[1], m_square_angle2_position[2], m_square_angle2_color[0], m_square_angle2_color[1], m_square_angle2_color[2],
                m_square_angle3_position[0], m_square_angle3_position[1], m_square_angle3_position[2], m_square_angle3_color[0], m_square_angle3_color[1], m_square_angle3_color[2],
                m_square_angle4_position[0], m_square_angle4_position[1], m_square_angle4_position[2], m_square_angle4_color[0], m_square_angle4_color[1], m_square_angle4_color[2]

            };

            GLuint indices[] = { 0, 1, 2, 3, 2, 1 };

            BufferLayout buffer_layout_2vec3{

                ShaderDataType::Float3,
                ShaderDataType::Float3

            };

            p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
            p_vao = std::make_unique<VertexArray>();
            p_positions_colors_vbo = std::make_unique<VertexBuffer>(square_data, sizeof(square_data), buffer_layout_2vec3);
            p_index_buffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));
            p_vao->add_vertex_buffer(*p_positions_colors_vbo);
            p_vao->set_index_buffer(*p_index_buffer);

        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();

	}

}
