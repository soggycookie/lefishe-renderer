#include "camera.h"

using namespace Lefishe;

Camera::Camera(CameraParam param)
	: m_camera_params(param)
{
	m_perspective_mtx =
		glm::perspective(glm::radians(m_camera_params.fov), m_camera_params.aspect, 
			m_camera_params.near_plane, m_camera_params.far_plane);
}

void Camera::rotation(const VEC3& rot) {
	m_rotation = rot;
	is_dirty = true;
}

void Camera::position(const VEC3& pos) {
	m_position = pos;
	is_dirty = true;
}

const MAT4& Camera::viewMtx() const {
	return m_view_mtx;
}

const MAT4& Camera::perspectiveMtx() const {
	return m_perspective_mtx;
}

const VEC3& Camera::position() const {
	return m_position;
}

const VEC3& Camera::rotation() const {
	return m_rotation;
}

VEC3& Camera::position() {
	is_dirty = true;
	return m_position;
}

VEC3& Camera::rotation() {
	is_dirty = true;
	return m_rotation;
}


void Camera::CameraParameter(const CameraParam& param) {
	m_camera_params = param;
	is_dirty = true;
}

void Camera::fov(const FLOAT& fov) {
	m_camera_params.fov = fov;
	is_dirty = true;
}

const FLOAT& Camera::fov() const {
	return m_camera_params.fov;
}

FLOAT& Camera::fov() {
	is_dirty = true;
	return m_camera_params.fov;
}

void Camera::constructMtx() {
	if (!is_dirty) {
		return;
	}

	float cx = glm::cos(glm::radians(m_rotation.x)), sx = glm::sin(glm::radians(m_rotation.x));
	float cy = glm::cos(glm::radians(m_rotation.y)), sy = glm::sin(glm::radians(m_rotation.y));
	float cz = glm::cos(glm::radians(m_rotation.z)), sz = glm::sin(glm::radians(m_rotation.z));

	// Column 0
	m_rot_mtx[0][0] = cy * cz;
	m_rot_mtx[0][1] = -cy * sz;
	m_rot_mtx[0][2] = sy;
	m_rot_mtx[0][3] = 0.0f;

	// Column 1
	m_rot_mtx[1][0] = sx * sy * cz + cx * sz;
	m_rot_mtx[1][1] = -sx * sy * sz + cx * cz;
	m_rot_mtx[1][2] = -sx * cy;
	m_rot_mtx[1][3] = 0.0f;

	// Column 2
	m_rot_mtx[2][0] = -cx * sy * cz + sx * sz;
	m_rot_mtx[2][1] = cx * sy * sz + sx * cz;
	m_rot_mtx[2][2] = cx * cy;
	m_rot_mtx[2][3] = 0.0f;

	// Column 3
	m_rot_mtx[3][0] = 0.0f;
	m_rot_mtx[3][1] = 0.0f;
	m_rot_mtx[3][2] = 0.0f;
	m_rot_mtx[3][3] = 1.0f;

	m_right = m_rot_mtx[0];
	m_up = m_rot_mtx[1];
	m_forward = m_rot_mtx[2];

	glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_position);

	m_perspective_mtx =
		glm::perspective(glm::radians(m_camera_params.fov), m_camera_params.aspect,
			m_camera_params.near_plane, m_camera_params.far_plane);

	m_view_mtx = glm::transpose(m_rot_mtx) * glm::inverse(translation);

	is_dirty = false;
}
