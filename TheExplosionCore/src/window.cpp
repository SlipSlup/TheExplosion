
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
#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>

namespace TheExplosion {

    static bool s_GLFW_initialized = false;

    GLfloat square_data[] = {

        -0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.5f,
        -0.5f,  0.5f, 0.0f, 0.5f, 0.0f, 0.5f,
         0.5f,  0.5f, 0.0f, 0.5f, 0.0f, 0.0f

    };

    GLuint indices[] = { 0, 1, 2, 3, 2, 1 };

    float scale[3] = { 1.0f, 1.0f, 1.0f };
    float rotation = 0.0f;
    float translation[3] = { 0.0f, 0.0f, 0.0f };

    const char* vertex_shader = R"(
        
        #version 330
        
        layout(location = 0) in vec3 vertex_position;
        layout(location = 1) in vec3 vertex_color;
        uniform mat4 model_matrix;
        out vec3 color;
        
        void main() {
        
            color = vertex_color;
            gl_Position = model_matrix * vec4(vertex_position, 1.0);
        
        }
        
    )";

    const char* fragment_shader = R"(
        
        #version 330
        
        in vec3 color;
        out vec4 frag_color;
        
        void main() {
        
            frag_color = vec4(color, 1.0);
        
        }
        
    )";

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
        
        glm::mat4 scale_matrix(

            scale[0], 0,        0,        0,
            0,        scale[1], 0,        0,
            0,        0,        scale[2], 0,
            0,        0,        0,        1

        );

        float rotation_in_radians = glm::radians(rotation);

        glm::mat4 rotation_matrix(

             cos(rotation_in_radians), sin(rotation_in_radians), 0, 0,
            -sin(rotation_in_radians), cos(rotation_in_radians), 0, 0,
             0,                        0,                        1, 0,
             0,                        0,                        0, 1

        );

        glm::mat4 translation_matrix(

            1,              0,              0,              0,
            0,              1,              0,              0,
            0,              0,              1,              0,
            translation[0], translation[1], translation[2], 1

        );
        
        glClearColor(m_background_color[0], m_background_color[1], m_background_color[2], 1);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        p_shader_program->bind();
        glm::mat4 model_matrix = translation_matrix * rotation_matrix * scale_matrix;
        p_shader_program->setMatrix4("model_matrix", model_matrix);
        p_vao->bind();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(p_vao->get_indices_count()), GL_UNSIGNED_INT, nullptr);

        if(ImGui::IsKeyPressed(ImGuiKey_Escape)) leave = !leave;

        if(leave) {

            ImGui::SetNextWindowSize(ImVec2(185, 90));
            ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
            ImGui::Begin("Exit", (bool*)0, ImGuiWindowFlags_NoDecoration + ImGuiWindowFlags_NoMove);
            ImGui::SetCursorPos(ImVec2(15, 15));
            ImGui::Text("Do you want to leave?");
            ImGui::GetStyle().WindowTitleAlign = ImVec2(0.5, 0.5);
            ImGui::SetCursorPos(ImVec2(80, 55));

            if(ImGui::Button("Yes")) Escape();

            ImGui::End();

        }

        ImGui::Begin("Menu");

        ImGui::Text("Background");

        ImGui::ColorEdit3("Background Color", m_background_color);

        ImGui::Text("Square");

        ImGui::SliderFloat3("Square Scale", scale, 0.0f, 2.0f);

        ImGui::SliderFloat("Square Rotation", &rotation, 0.0f, 360.0f);

        ImGui::SliderFloat3("Square Translation", translation, -1.0f, 1.0f);
        
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();

	}

}
