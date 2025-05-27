
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
#include "camera.hpp"
#include "OpenGL/renderer.hpp"
#include <wtypes.h>

namespace TheExplosion {

    GLfloat square_data[] = {

        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f

    };

    GLuint square_indices[] = {
        
        0, 1, 3, 2, 3, 1,
        3, 2, 4, 5, 4, 2,
        3, 4, 0, 7, 0, 4,
        7, 4, 6, 5, 6, 4,
        7, 6, 0, 1, 0, 6,
        1, 6, 2, 5, 2, 6
    
    };

    float square_scale[3] = { 1.0f, 1.0f, 1.0f };
    float square_rotation[3] = { 0.0f, 0.0f, 0.0f };
    float square_translation[3] = { 0.0f, 0.0f, 0.0f };
    float camera_position[3] = { 0.0f, 0.0f, 2.0f };
    float camera_rotation[3] = { 0.0f, 0.0f, 0.0f };
    Camera camera;

    const char* vertex_shader = R"(
        
        #version 330
        
        layout(location = 0) in vec3 vertex_position;
        layout(location = 1) in vec3 vertex_color;
        uniform mat4 model_matrix;
        uniform mat4 view_projection_matrix;
        out vec3 color;
        
        void main() {
        
            color = vertex_color;
            gl_Position = view_projection_matrix * model_matrix * vec4(vertex_position, 1.0);
        
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

    Window::Window(
        
        std::string title,
        const unsigned int width,
        const unsigned int height
    
    ) : m_data({ 
        
        std::move(title),
        width,
        height
        
    }) {
        
        int resultCode = init();
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplOpenGL3_Init();

        ImGui_ImplGlfw_InitForOpenGL(
            
            m_pWindow,
            true
        
        );
    
    }

	Window::~Window() { shutdown(); }

	int Window::init() {

        glfwInit();

        glfwWindowHint(

            GLFW_MAXIMIZED,
            true

        );

        m_pWindow = glfwCreateWindow(
            
            m_data.width,
            m_data.height,
            m_data.title.c_str(),
            nullptr,
            nullptr
        
        );

        Renderer::init(m_pWindow);

        glfwSetWindowUserPointer(
            
            m_pWindow,
            &m_data
        
        );

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

            [](
                
                GLFWwindow* pWindow,
                int width,
                int height
                
            ) {
                    
                Renderer::set_viewport(
                
                    width,
                    height
                
                );
            
            }

        );

        BufferLayout buffer_layout_2vec3{

            ShaderDataType::Float3,
            ShaderDataType::Float3

        };

        p_shader_program = std::make_unique<ShaderProgram>(
            
            vertex_shader,
            fragment_shader
        
        );

        p_vao = std::make_unique<VertexArray>();

        p_positions_colors_vbo = std::make_unique<VertexBuffer>(
            
            square_data,
            sizeof(square_data),
            buffer_layout_2vec3
        
        );

        p_index_buffer = std::make_unique<IndexBuffer>(
            
            square_indices,
            sizeof(square_indices) / sizeof(GLuint)
        
        );

        p_vao->add_vertex_buffer(*p_positions_colors_vbo);
        p_vao->set_index_buffer(*p_index_buffer);

        return 0;

	}

	void Window::shutdown() {
        
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();

	}

	void Window::on_update() {
        
        glm::mat4 square_scale_matrix(

            square_scale[0], 0,               0,               0,
            0,               square_scale[1], 0,               0,
            0,               0,               square_scale[2], 0,
            0,               0,               0,               1

        );

        float square_rotation_in_radians_x = glm::radians(square_rotation[0]);

        glm::mat4 square_rotation_matrix_x(

            1,  0,                                 0,                                 0,
            0,  cos(square_rotation_in_radians_x), sin(square_rotation_in_radians_x), 0,
            0, -sin(square_rotation_in_radians_x), cos(square_rotation_in_radians_x), 0,
            0,  0,                                 0,                                 1

        );

        float square_rotation_in_radians_y = glm::radians(square_rotation[1]);

        glm::mat4 square_rotation_matrix_y(

            cos(square_rotation_in_radians_y), 0, -sin(square_rotation_in_radians_y), 0,
            0,                                 1,  0,                                 0,
            sin(square_rotation_in_radians_y), 0,  cos(square_rotation_in_radians_y), 0,
            0,                                 0,  0,                                 1

        );

        float square_rotation_in_radians_z = glm::radians(square_rotation[2]);

        glm::mat4 square_rotation_matrix_z(

             cos(square_rotation_in_radians_z), sin(square_rotation_in_radians_z), 0, 0,
            -sin(square_rotation_in_radians_z), cos(square_rotation_in_radians_z), 0, 0,
             0,                                 0,                                 1, 0,
             0,                                 0,                                 0, 1

        );

        glm::mat4 square_translation_matrix(

            1,                     0,                     0,                     0,
            0,                     1,                     0,                     0,
            0,                     0,                     1,                     0,
            square_translation[0], square_translation[1], square_translation[2], 1

        );

        glm::mat4 square_model_matrix = square_translation_matrix * square_rotation_matrix_z * square_rotation_matrix_y * square_rotation_matrix_x * square_scale_matrix;
        
        Renderer::set_clear_color(
            
            m_background_color[0],
            m_background_color[1],
            m_background_color[2],
            1
        
        );

        Renderer::clear();
        p_shader_program->bind();

        p_shader_program->setMatrix4(
            
            "model_matrix",
            square_model_matrix
        
        );

        camera.set_position_rotation(
        
            glm::vec3(
                
                camera_position[0],
                camera_position[1],
                camera_position[2]
            
            ),

            glm::vec3(
                
                camera_rotation[0],
                camera_rotation[1],
                camera_rotation[2]
            
            )
        
        );

        p_shader_program->setMatrix4(
            
            "view_projection_matrix",
            camera.get_projection_matrix() * camera.get_view_matrix()
        
        );

        Renderer::draw(*p_vao);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
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
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();

	}

}
