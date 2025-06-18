
#pragma once

#include "event.hpp"
#include "camera.hpp"
#include <memory>
#include "vertex_array.hpp"

namespace TheExplosion {
	
	class Application { 
	
	public:

		Application();
		virtual ~Application();
		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator = (const Application&) = delete;
		Application& operator = (Application&&) = delete;
		virtual int start(const char* title);
		void close();
		virtual void on_update() {}
		virtual void on_ui_draw() {}

		float square_scale[3] = {
			
			0.05f,
			0.05f,
			0.05f
		
		};

		float square_rotation[3] = {
			
			 90.0f,
			 0.0f,
			-50.0f
		
		};

		float square_translation[3] = {
			
			2.5f,
			0.0f,
			0.0f
		
		};

		float m_background_color[3] = {
			
			0.0f,
			0.0f,
			0.0f
		
		};

		Camera camera {
			
			glm::vec3(
				
				 0.0f,
				 0.0f,
				 1.5f
			
			)
		
		};

		double deltaTime = 0.0f;
		double currentFrame = 0;
		glm::vec2 get_current_cursor_position() const;
		int get_window_width();
		int get_window_height();
		void play_sound(const char* path);
		std::vector<float> get_model_vertices(const char* modelPath);
		std::vector<unsigned int> get_model_indices(const char* modelPath);

	private:

		std::unique_ptr<class Window> m_pWindow;
		EventDispatcher m_event_dispatcher;
		bool m_bCloseWindow = false;

	};

}
