
#pragma once

#include <glm/mat3x3.hpp>

namespace TheExplosion {

	class ShaderProgram {

	public:

		ShaderProgram(
			
			const char* vertex_shader_src,
			const char* fragment_shader_src
		
		);

		ShaderProgram(ShaderProgram&&) noexcept;
		ShaderProgram& operator = (ShaderProgram&&) noexcept;
		~ShaderProgram();
		ShaderProgram() = delete;
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator = (const ShaderProgram&) = delete;
		void bind() const;
		static void unbind();

		void setMatrix4(
			
			const char* name,
			const glm::mat4& matrix
		
		) const;

		void set_int(
			
			const char* name,
			const int value
		
		) const;

	private:

		unsigned int m_id = 0;

	};

}
