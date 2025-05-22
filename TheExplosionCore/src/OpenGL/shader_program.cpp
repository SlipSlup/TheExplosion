
#include "shader_program.hpp"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace TheExplosion {

	bool create_shader(const char* source, const GLenum shader_type, GLuint& shader_id) {

		shader_id = glCreateShader(shader_type);
		glShaderSource(shader_id, 1, &source, nullptr);
		glCompileShader(shader_id);

		return true;

	}

	ShaderProgram::ShaderProgram(const char* vertex_shader_src, const char* fragment_shader_src) {

		GLuint vertex_shader_id = 0;
		create_shader(vertex_shader_src, GL_VERTEX_SHADER, vertex_shader_id);
		GLuint fragment_shader_id = 0;
		create_shader(fragment_shader_src, GL_FRAGMENT_SHADER, fragment_shader_id);
		m_id = glCreateProgram();
		glAttachShader(m_id, vertex_shader_id);
		glAttachShader(m_id, fragment_shader_id);
		glLinkProgram(m_id);
		glDetachShader(m_id, vertex_shader_id);
		glDetachShader(m_id, fragment_shader_id);
		glDeleteShader(vertex_shader_id);
		glDeleteShader(fragment_shader_id);

	}

	ShaderProgram::~ShaderProgram() { glDeleteProgram(m_id); }

	void ShaderProgram::bind() const { glUseProgram(m_id); }

	void ShaderProgram::unbind() { glUseProgram(0); }

	ShaderProgram& ShaderProgram::operator = (ShaderProgram&& shaderProgram) noexcept {

		glDeleteProgram(m_id);
		m_id = shaderProgram.m_id;
		shaderProgram.m_id = 0;
		return *this;

	}

	ShaderProgram::ShaderProgram (ShaderProgram&& shaderProgram) noexcept {

		m_id = shaderProgram.m_id;
		shaderProgram.m_id = 0;

	}

	void ShaderProgram::setMatrix4(const char* name, const glm::mat4& matrix) const { glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, glm::value_ptr(matrix)); }

}
