
#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

namespace TheExplosion {

	class Camera {

	public:

		Camera(

			const glm::vec3& position = { 0, 0, 0 },
			const glm::vec3& rotation = { 0, 0, 0 }
		
		);

		void set_position_rotation(

			const glm::vec3& position,
			const glm::vec3& rotation

		);

		glm::mat4 get_view_matrix();
		glm::mat4 get_projection_matrix() const { return m_projection_matrix; }

		void add_movement_rotation(

			const glm::vec3& movement_delta,
			const glm::vec3& rotation_delta

		);

		const glm::vec3& get_camera_position() const { return m_position; }
		const glm::vec3& get_camera_rotation() const { return m_rotation; }

	private:

		void update_view_matrix();
		void update_projection_matrix();
		glm::vec3 m_position;
		glm::vec3 m_rotation;
		glm::mat4 m_view_matrix;
		glm::mat4 m_projection_matrix;
		glm::vec3 m_direction;
		glm::vec3 m_right;
		glm::vec3 m_up;
		static constexpr glm::vec3 s_world_up{ 0.0f, 0.0f, 1.0f };
		static constexpr glm::vec3 s_world_right{ 0.0f, -1.0f, 0.0f };
		static constexpr glm::vec3 s_world_forward{ 1.0f, 0.0f, 0.0f };
		bool m_update_view_matrix = false;

	};

}
