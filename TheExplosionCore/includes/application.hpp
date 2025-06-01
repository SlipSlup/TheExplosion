
#pragma once

#include "event.hpp"
#include "camera.hpp"
#include <memory>

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
		virtual void on_update() {}
		virtual void on_ui_draw() {}
		float camera_position[3] = { 0.0f, 0.0f, 2.0f };
		float camera_rotation[3] = { 0.0f, 0.0f, 0.0f };
		float square_scale[3] = { 1.0f, 1.0f, 1.0f };
		float square_rotation[3] = { 0.0f, 0.0f, 0.0f };
		float square_translation[3] = { 0.0f, 0.0f, 0.0f };
		float m_background_color[3] = { 0.0f, 0.0f, 0.0f };
		Camera camera;

	private:

		std::unique_ptr<class Window> m_pWindow;
		EventDispatcher m_event_dispatcher;
		bool m_bCloseWindow = false;

	};

}
