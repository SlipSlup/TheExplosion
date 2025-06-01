
#include "renderer.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "vertex_array.hpp"

namespace TheExplosion {

	void Renderer::init(GLFWwindow* pWindow) {

		glfwMakeContextCurrent(pWindow);
		gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

	}

	void Renderer::draw(const VertexArray& vertex_array) {

		vertex_array.bind();

		glDrawElements(

			GL_TRIANGLES,
			static_cast<GLsizei>(vertex_array.get_indices_count()),
			GL_UNSIGNED_INT,
			nullptr

		);

	}

	void Renderer::set_clear_color(

		const float r,
		const float g,
		const float b,
		const float a

	) {
		
		glClearColor(
			
			r,
			g,
			b,
			a
		
		);
	
	}

	void Renderer::clear() { glClear(GL_COLOR_BUFFER_BIT); }

	void Renderer::set_viewport(

		const unsigned int width,
		const unsigned int height,
		const unsigned int left_offset,
		const unsigned int bottom_offset

	) {

		glViewport(
			
			left_offset,
			bottom_offset,
			width,
			height
		
		);

	}

}