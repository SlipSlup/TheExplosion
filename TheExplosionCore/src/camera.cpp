
#include "camera.hpp"
#include <glm/trigonometric.hpp>

namespace TheExplosion {

	Camera::Camera(

		const glm::vec3& position,
		const glm::vec3& rotation

	) :

		m_position(position),
		m_rotation(rotation)
	
	{

		update_view_matrix();
		update_projection_matrix();

	}

	void Camera::update_view_matrix() {

		float rotation_in_radians_x = glm::radians(-m_rotation.x);

		glm::mat4 rotation_matrix_x(

			1,  0,							0,							0,
			0,  cos(rotation_in_radians_x), sin(rotation_in_radians_x), 0,
			0, -sin(rotation_in_radians_x), cos(rotation_in_radians_x), 0,
			0,  0,							0,							1

		);
		
		float rotation_in_radians_y = glm::radians(-m_rotation.y);

		glm::mat4 rotation_matrix_y(

			cos(rotation_in_radians_y), 0, -sin(rotation_in_radians_y), 0,
			0,							1,  0,							0,
			sin(rotation_in_radians_y), 0,  cos(rotation_in_radians_y), 0,
			0,							0,  0,							1

		);
		
		float rotation_in_radians_z = glm::radians(-m_rotation.z);

		glm::mat4 rotation_matrix_z(

			 cos(rotation_in_radians_z), sin(rotation_in_radians_z), 0, 0,
			-sin(rotation_in_radians_z), cos(rotation_in_radians_z), 0, 0,
			 0,							 0,							 1, 0,
			 0,							 0,							 0, 1

		);
		
		glm::mat4 translation_matrix(

			 1,				 0,				 0,				0,
			 0,				 1,				 0,				0,
			 0,			     0,			     1,			    0,
			-m_position[0], -m_position[1], -m_position[2], 1

		);

		m_view_matrix = rotation_matrix_z * rotation_matrix_y * rotation_matrix_x * translation_matrix;

	}

	void Camera::update_projection_matrix() {

		float r = 0.1f;
		float t = 0.1f;
		float f = 10;
		float n = 0.1f;

		m_projection_matrix = glm::mat4(

			n / r, 0,	   0,					 0,
			0,	   n / t,  0,					 0,
			0,	   0,	  -(f + n) / (f - n),   -1,
			0,	   0,	  -2 * f * n / (f - n),  0

		);

	}

	void Camera::set_position_rotation(
		
		const glm::vec3& position,
		const glm::vec3& rotation
	
	) {

		m_position = position;
		m_rotation = rotation;
		update_view_matrix();

	}

}