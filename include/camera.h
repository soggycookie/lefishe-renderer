#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include "global_header.h"

namespace Lefishe {

	struct CameraParam {
		FLOAT fov = 90.0f;
		FLOAT near_plane = 0.1f;
		FLOAT far_plane = 1000.0f;
		FLOAT aspect = 1.0f;
	};

	class Camera {
	public:
		Camera() = default;

		Camera(CameraParam param);

		void CameraParameter(const CameraParam& param);

		const MAT4& viewMtx() const;
		const MAT4& perspectiveMtx() const;

		void rotation(const VEC3& rot);
		void position(const VEC3& pos);

		const VEC3& position() const;
		const VEC3& rotation() const;
		
		VEC3& position();
		VEC3& rotation();


		void fov(const FLOAT& fov);
		const FLOAT& fov() const;
		FLOAT& fov();

		void constructMtx();

	private:
		VEC3 m_position = glm::vec3(0.0f);
		VEC3 m_rotation = glm::vec3(0.0f);

		VEC3 m_right = glm::vec3(1.0f, 0.0f, 0.0f);
		VEC3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
		VEC3 m_forward = glm::vec3(0.0f, 0.0f, 1.0f);


		MAT4 m_view_mtx = glm::mat4(1.0f);
		MAT4 m_rot_mtx = glm::mat4(1.0f);
		MAT4 m_perspective_mtx = glm::mat4(1.0f);
		
		CameraParam m_camera_params;

		BOOL is_dirty = false;
	};

}