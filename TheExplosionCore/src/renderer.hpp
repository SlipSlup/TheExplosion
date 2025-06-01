
#pragma once

struct GLFWwindow;

namespace TheExplosion {

	class VertexArray;

	class Renderer {

	public:

		static void init(GLFWwindow* pWindow);
		static void draw(const VertexArray& vertex_array);

		static void set_clear_color(

			const float r,
			const float g,
			const float b,
			const float a

		);

		static void clear();

		static void set_viewport(

			const unsigned int width,
			const unsigned int height,
			const unsigned int left_offset = 0,
			const unsigned int bottom_offset = 0
		
		);

	};

}