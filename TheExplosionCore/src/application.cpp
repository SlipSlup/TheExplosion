
#include "application.hpp"
#include "window.hpp"
#include <windows.h>
#include "shader_program.hpp"
#include "vertex_buffer.hpp"
#include "vertex_array.hpp"
#include "index_buffer.hpp"
#include <GLFW/glfw3.h>
#include "renderer.hpp"
#include <glm/trigonometric.hpp>
#include "UI.hpp"
#include <imgui/imgui.h>
#include "input.hpp"
#include <glad/glad.h>
#include "texture2d.hpp"

namespace TheExplosion {

    double lastFrame = 0;

    GLfloat square_positions_coords[] = {

        -1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f,  1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,

         1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
         1.0f,  1.0f,  1.0f, 0.0f, 1.0f

    };

    GLuint square_indices[] = {
        
        0, 1, 2, 3, 2, 1,
        4, 5, 6, 7, 6, 5,
        0, 4, 6, 0, 2, 6,
        1, 5, 3, 3, 7, 5,
        3, 7, 2, 7, 6, 2,
        1, 5, 0, 5, 0, 4
    
    };

    void generate_quads_texture(

        unsigned char* data,
        const unsigned int width,
        const unsigned int height

    ) {

        for(unsigned int x = 0; x < width; x++) {

            for(unsigned int y = 0; y < height; y++) {

                if((x < width / 2 && y < height / 2) || x >= width / 2 && y >= height / 2) {

                    data[3 * (x + width * y) + 0] = 0;
                    data[3 * (x + width * y) + 1] = 0;
                    data[3 * (x + width * y) + 2] = 0;

                }
                else {

                    data[3 * (x + width * y) + 0] = 255;
                    data[3 * (x + width * y) + 1] = 255;
                    data[3 * (x + width * y) + 2] = 255;

                }

            }

        }

    }

    const char* vertex_shader = R"(
        
        #version 330
        
        layout(location = 0) in vec3 vertex_position;
        layout(location = 1) in vec2 texture_coord;
        uniform mat4 model_matrix;
        uniform mat4 view_projection_matrix;
        uniform int current_frame;
        out vec2 tex_coord;
        
        void main() {
        
            tex_coord = texture_coord + vec2(current_frame / 1000.0f * 0.1f, current_frame / 1000.0f * 0.1f);
            gl_Position = view_projection_matrix * model_matrix * vec4(vertex_position, 1.0);
        
        }
        
    )";

    const char* fragment_shader = R"(
        
        #version 330
        
        in vec2 tex_coord;
        uniform sampler2D tex;
        out vec4 frag_color;
        
        void main() {
        
            frag_color = texture(tex, tex_coord);
        
        }
        
    )";

    std::unique_ptr<ShaderProgram> p_shader_program;
    std::unique_ptr<VertexBuffer> p_positions_colors_vbo;
    std::unique_ptr<IndexBuffer> p_index_buffer;
    std::unique_ptr<VertexArray> p_vao;
    std::unique_ptr<Texture2D> p_texture;
	
	Application::Application() {}
	Application::~Application() {}

	int Application::start(const char* title) {

		m_pWindow = std::make_unique<Window>(
			
			title,
            Application::get_window_width(),
            Application::get_window_height()
		
		);

		m_event_dispatcher.add_event_listener<EventWindowClose>([&](EventWindowClose& event) { close(); });
        m_event_dispatcher.add_event_listener<EventKeyPressed>([&](EventKeyPressed& event) { Input::PressKey(event.key_code); });
        m_event_dispatcher.add_event_listener<EventKeyReleased>([&](EventKeyReleased& event) { Input::ReleaseKey(event.key_code); });
		m_pWindow->set_event_callback([&](BaseEvent& event) { m_event_dispatcher.dispatch(event); });
        
        const unsigned int width = 1000;
        const unsigned int height = 1000;
        auto* data = new unsigned char[width * height * 3];
        generate_quads_texture(data, width, height);
        p_texture = std::make_unique<Texture2D>(data, width, height);
        p_texture -> bind(0);
        delete[] data;

        BufferLayout buffer_layout {

            ShaderDataType::Float3,
            ShaderDataType::Float2

        };

        p_shader_program = std::make_unique<ShaderProgram>(

            vertex_shader,
            fragment_shader

        );

        p_vao = std::make_unique<VertexArray>();

        p_positions_colors_vbo = std::make_unique<VertexBuffer>(

            square_positions_coords,
            sizeof(square_positions_coords),
            buffer_layout

        );

        p_index_buffer = std::make_unique<IndexBuffer>(

            square_indices,
            sizeof(square_indices) / sizeof(GLuint)

        );

        p_vao->add_vertex_buffer(*p_positions_colors_vbo);
        p_vao->set_index_buffer(*p_index_buffer);
        Renderer::enable_depth_test();

		while(!m_bCloseWindow) {

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
            
            p_shader_program->bind();

            p_shader_program->setMatrix4(

                "model_matrix",
                square_model_matrix

            );

            p_shader_program->setMatrix4(

                "view_projection_matrix",
                camera.get_projection_matrix() * camera.get_view_matrix()

            );

            double currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            p_shader_program->set_int(

                "current_frame",
                (int)(lastFrame * 1000)

            );

            Renderer::set_clear_color(

                m_background_color[0],
                m_background_color[1],
                m_background_color[2],
                1

            );

            Renderer::clear();
            Renderer::draw(*p_vao);
            UI::on_ui_draw_begin();
            on_ui_draw();
            UI::on_ui_draw_end();
			m_pWindow->on_update();
			on_update();

		}

        m_pWindow = nullptr;

		return 0;
	
	}

    glm::vec2 Application::get_current_cursor_position() const { return m_pWindow -> get_current_cursor_position(); }

    int Application::get_window_width() {

        RECT desktop;

        GetWindowRect(
            
            GetDesktopWindow(),
            &desktop
        
        );

        return desktop.right;

    }

    int Application::get_window_height() {

        RECT desktop;

        GetWindowRect(
            
            GetDesktopWindow(),
            &desktop
        
        );

        return desktop.bottom;

    }

    void Application::close() { m_bCloseWindow = true; }

}
