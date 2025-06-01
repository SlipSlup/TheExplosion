
#include "window.hpp"
#include <GLFW/glfw3.h>
#include "UI.hpp"
#include "renderer.hpp"

namespace TheExplosion {

    Window::Window(
        
        std::string title,
        const unsigned int width,
        const unsigned int height
    
    ) : m_data({ 
        
        std::move(title),
        width,
        height
        
    }) { init(); }

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

        glfwSetKeyCallback(

            m_pWindow,

            [](

                GLFWwindow* pWindow,
                int key,
                int scancode,
                int action,
                int mods

            ) {

                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

                switch(action) {

                    case GLFW_PRESS : {

                        EventKeyPressed event(static_cast<KeyCode>(key), false);
                        data.eventCallbackFn(event);

                        break;

                    }

                    case GLFW_RELEASE : {

                        EventKeyReleased event(static_cast<KeyCode>(key));
                        data.eventCallbackFn(event);

                        break;

                    }

                    case GLFW_REPEAT : {

                        EventKeyPressed event(static_cast<KeyCode>(key), true);
                        data.eventCallbackFn(event);

                        break;

                    }

                }

            }

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

        UI::on_window_create(m_pWindow);

        return 0;

	}

	void Window::shutdown() {
        
        UI::on_window_close();
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();

	}

	void Window::on_update() {
        
        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();

	}

}
