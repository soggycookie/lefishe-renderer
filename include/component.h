#pragma once
#include "global_header.h"


namespace Lefishe {

	class BaseComponent {
	public:
		virtual int getID() = 0;

	protected:
		int m_id = 0;
	};


#define TRANSFORM_ID 0

	class TransformComponent : public BaseComponent {
	public:
		const VEC3& position() const;
		const VEC3& rotation() const;

		VEC3& position();
		VEC3& rotation();
	
		const MAT4& rotationMtx() const;
		const MAT4& localToWorldMtx() const;
		const MAT4& worldToLocalMtx() const;

		const VEC3& right() const;
		const VEC3& forward() const;
		const VEC3& up() const;

		void constructMatrix();

		int getID() override;

	private:
		VEC3 m_position = glm::vec3(0.0f);
		VEC3 m_rotation = glm::vec3(0.0f);

		VEC3 m_right = glm::vec3(1.0f, 0.0f, 0.0f);
		VEC3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
		VEC3 m_forward = glm::vec3(0.0f, 0.0f, 1.0f);

		MAT4 m_rot_mtx = glm::mat4(1.0f);
		MAT4 m_local_to_world_mtx = glm::mat4(1.0f);
		MAT4 m_world_to_local_mtx = glm::mat4(1.0f);

		bool is_dirty = false;
	};



#define MAX_FAR_PLANE 10000.0f
#define DEFAULT_FAR_PLANE 1000.0f
#define DEFAULT_NEAR_PLANE 0.1f

#define CAMERA_ID 1

	struct CameraInfo {
			FLOAT fov = 90.0f;
			FLOAT near_plane = 0.1f;
			FLOAT far_plane = 1000.0f;
			FLOAT aspect = 16.0f / 9.0f;
			FLOAT pixel_width = 1280;
			FLOAT pixel_height = 720;

		};

	class CameraComponent : public BaseComponent {
	public:


		static std::shared_ptr<CameraComponent> getMainCamera();
		static void setMainCamera(std::shared_ptr<CameraComponent> main);

		const MAT4& perspectiveMtx() const;

		void fov(const FLOAT& fov);
		const FLOAT& fov() const;
		FLOAT& fov();

		void nearPlane(FLOAT n);
		const FLOAT& nearPlane() const;
		FLOAT& nearPlane();

		void farPlane(FLOAT f);
		const FLOAT& farPlane() const;
		FLOAT& farPlane();

		void constructMatrix();

		int getID() override;

	private:
		void check();

	private:
		static std::shared_ptr<CameraComponent> m_main_cam;

		MAT4 m_perspective_mtx = glm::mat4(1.0f);
		CameraInfo m_camera_info;
		bool is_dirty = false;
	};



#define MESH_ID 2

	struct MeshData{
		std::vector<VEC3> vertices;
		std::vector<VEC2> uv;
		std::vector<VEC3> vertex_colors;
		std::vector<VEC3> normals;
		std::vector<VEC3> tangents;
		std::vector<UINT> indices;
	};

	class MeshComponent : public BaseComponent{
	public:
		
		MeshComponent(const MeshData& data);
		MeshComponent(MeshData&& data);

		const std::vector<VEC3>& vertices() const;
		const std::vector<VEC2>& uv() const;
		const std::vector<VEC3>& vertexColors() const;
		const std::vector<VEC3>& normals() const;
		const std::vector<VEC3>& tangents() const;
		const std::vector<UINT>& indices() const;
		const MeshData& data() const;

		void vertices(const std::vector<VEC3>& vertices);
		void uv(const std::vector<VEC2>& uv);
		void vertexColors(const std::vector<VEC3>& vertex_colors);
		void normals(const std::vector<VEC3>& normals);
		void tangents(const std::vector<VEC3>& tangents);
		void indices(const std::vector<UINT>& indices);
		void data(const MeshData& data);

		void vertices(std::vector<VEC3>&& vertices);
		void uv(std::vector<VEC2>&& uv);
		void vertexColors(std::vector<VEC3>&& vertex_colors);
		void normals(std::vector<VEC3>&& normals);
		void tangents(std::vector<VEC3>&& tangents);
		void indices(std::vector<UINT>&& indices);
		void data(MeshData&& data);

		int vertexSize() const;

		int getID() override;

	private:
		MeshData m_data;
	};

}