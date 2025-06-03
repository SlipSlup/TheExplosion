
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

namespace TheExplosion {

    float lastFrame = 0.0f;

    GLfloat square_data[] = {

         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
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

    void generate_circle(
        
        unsigned char* data,
        const unsigned int width,
        const unsigned int height,
        const unsigned int center_x,
        const unsigned int center_y,
        const unsigned int radius,
        const unsigned char color_r,
        const unsigned char color_g,
        const unsigned char color_b
    
    ) {

        for(unsigned int x = 0; x < width; ++x) {

            for(unsigned int y = 0; y < height; ++y) {

                if((x - center_x) * (x - center_x) + (y - center_y) * (y - center_y) < radius * radius) {

                    data[3 * (x + width * y) + 0] = color_r;
                    data[3 * (x + width * y) + 1] = color_g;
                    data[3 * (x + width * y) + 2] = color_b;

                }

            }

        }

    }

    void generate_smile_texture(
        
        unsigned char* data,
        const unsigned int width,
        const unsigned int height
    
    ) {
        
        for(unsigned int x = 0; x < width; ++x) {

            for(unsigned int y = 0; y < height; ++y) {

                data[3 * (x + width * y) + 0] = 200;
                data[3 * (x + width * y) + 1] = 191;
                data[3 * (x + width * y) + 2] = 231;

            }

        }

        generate_circle(data, width, height, width * 0.5, height * 0.5, width * 0.4, 255, 255, 0);
        generate_circle(data, width, height, width * 0.5, height * 0.4, width * 0.2, 0, 0, 0);
        generate_circle(data, width, height, width * 0.5, height * 0.45, width * 0.2, 255, 255, 0);
        generate_circle(data, width, height, width * 0.35, height * 0.6, width * 0.07, 255, 0, 255);
        generate_circle(data, width, height, width * 0.65, height * 0.6, width * 0.07, 0, 0, 255);

    }

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
	
	Application::Application() {}
	Application::~Application() {}

	int Application::start(const char* title) {
	
		ShowWindow(
			
			GetConsoleWindow(),
			SW_HIDE
		
		);

		m_pWindow = std::make_unique<Window>(
			
			title,
            Application::get_window_width(),
            Application::get_window_height()
		
		);

		m_event_dispatcher.add_event_listener<EventWindowClose>([&](EventWindowClose& event) { m_bCloseWindow = true; });
        m_event_dispatcher.add_event_listener<EventKeyPressed>([&](EventKeyPressed& event) { Input::PressKey(event.key_code); });
        m_event_dispatcher.add_event_listener<EventKeyReleased>([&](EventKeyReleased& event) { Input::ReleaseKey(event.key_code); });
		m_pWindow->set_event_callback([&](BaseEvent& event) { m_event_dispatcher.dispatch(event); });

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

        on_start();

		while(!m_bCloseWindow) {

            glm::mat4 square_scale_matrix(

                square_scale[0], 0, 0, 0,
                0, square_scale[1], 0, 0,
                0, 0, square_scale[2], 0,
                0, 0, 0, 1

            );

            float square_rotation_in_radians_x = glm::radians(square_rotation[0]);

            glm::mat4 square_rotation_matrix_x(

                1, 0, 0, 0,
                0, cos(square_rotation_in_radians_x), sin(square_rotation_in_radians_x), 0,
                0, -sin(square_rotation_in_radians_x), cos(square_rotation_in_radians_x), 0,
                0, 0, 0, 1

            );

            float square_rotation_in_radians_y = glm::radians(square_rotation[1]);

            glm::mat4 square_rotation_matrix_y(

                cos(square_rotation_in_radians_y), 0, -sin(square_rotation_in_radians_y), 0,
                0, 1, 0, 0,
                sin(square_rotation_in_radians_y), 0, cos(square_rotation_in_radians_y), 0,
                0, 0, 0, 1

            );

            float square_rotation_in_radians_z = glm::radians(square_rotation[2]);

            glm::mat4 square_rotation_matrix_z(

                cos(square_rotation_in_radians_z), sin(square_rotation_in_radians_z), 0, 0,
                -sin(square_rotation_in_radians_z), cos(square_rotation_in_radians_z), 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1

            );

            glm::mat4 square_translation_matrix(

                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
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

            Renderer::set_clear_color(

                m_background_color[0],
                m_background_color[1],
                m_background_color[2],
                1

            );

            double currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
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

    glm::vec2 Application::get_current_cursor_position() const { return m_pWindow->get_current_cursor_position(); }

    int Application::get_window_width() {

        RECT desktop;
        GetWindowRect(GetDesktopWindow(), &desktop);

        return desktop.right;

    }

    int Application::get_window_height() {

        RECT desktop;
        GetWindowRect(GetDesktopWindow(), &desktop);

        return desktop.bottom;

    }

}
