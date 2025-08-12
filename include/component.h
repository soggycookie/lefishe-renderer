#pragma once
#include "global_header.h"
#include "material.h"
#include "id_generator.h"

namespace Lefishe {
	class Object;

	class BaseComponent {
	public:
		//virtual const Component id() const;
		BaseComponent();
		virtual ~BaseComponent() = default;

		virtual std::type_index getType() const = 0;
		virtual void update() = 0;

		long id() const;

	protected:
		long m_id = 0;
	};

	struct TransformData{
		VEC3 m_position = glm::vec3(0.0f);
		VEC3 m_rotation = glm::vec3(0.0f);
		VEC3 m_scale	= glm::vec3(1.0f);
	};

	class TransformComponent : public BaseComponent {
	public:
		TransformComponent(std::shared_ptr<Object> owner, TransformData data = TransformData{});

		const VEC3& position() const;
		const VEC3& rotation() const;
		const VEC3& scale() const;

		VEC3& position();
		VEC3& rotation();
		VEC3& scale();
	
		const MAT4& rotationMtx() const;
		const MAT4& localToWorldMtx() const;
		const MAT4& worldToLocalMtx() const;

		const VEC3& right() const;
		const VEC3& forward() const;
		const VEC3& up() const;

		const MAT4& localMtx() const;
		const MAT4& globalMtx() const;

		void localMtx(const MAT4& mtx);
		void globalMtx(const MAT4& mtx);

		void markDirty();

		std::type_index getType() const override;
		void update() override;
		//const Component id() const override;
	private:
		void constructMatrix();


	private:
		TransformData m_data;
		std::weak_ptr<Object> m_owner;

		VEC3 m_right   = VEC3(1.0f, 0.0f, 0.0f);
		VEC3 m_up      = VEC3(0.0f, 1.0f, 0.0f);
		VEC3 m_forward = VEC3(0.0f, 0.0f, 1.0f);

		MAT4 m_rot_mtx = MAT4(1.0f);

		MAT4 m_local_mtx    = MAT4(1.0f);
		MAT4 m_global_mtx   = MAT4(1.0f);

		MAT4 m_local_to_world_mtx = MAT4(1.0f);
		MAT4 m_world_to_local_mtx = MAT4(1.0f);

		bool is_dirty = true;
	};



#define MAX_FAR_PLANE 10000.0f
#define DEFAULT_FAR_PLANE 1000.0f
#define DEFAULT_NEAR_PLANE 0.1f


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
		CameraComponent(CameraInfo info = CameraInfo{});

		static std::shared_ptr<CameraComponent> main();
		static void main(std::shared_ptr<CameraComponent> main);

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


		std::type_index getType() const override;
		//const Component id() const override;
		void update() override;

	private:
		void constructMatrix();
		void check();

	private:
		static std::shared_ptr<CameraComponent> m_main_cam;

		MAT4 m_perspective_mtx = glm::mat4(1.0f);
		CameraInfo m_camera_info;
		bool is_dirty = true;
	};




	struct MeshData{
		std::vector<VEC3> vertices;
		std::vector<VEC3> normals;
		std::vector<VEC3> tangents;
		std::vector<VEC4> vertex_colors;
		std::vector<VEC2> uv;
		std::vector<UINT> indices;


		void clear();
	};

	class MeshComponent : public BaseComponent{
	public:
		
		MeshComponent(const MeshData& data);
		MeshComponent(MeshData&& data);

		const std::vector<VEC3>& vertices() const;
		const std::vector<VEC2>& uv() const;
		const std::vector<VEC4>& vertexColors() const;
		const std::vector<VEC3>& normals() const;
		const std::vector<VEC3>& tangents() const;
		const std::vector<UINT>& indices() const;
		const MeshData& data() const;

		void vertices(const std::vector<VEC3>& vertices);
		void uv(const std::vector<VEC2>& uv);
		void vertexColors(const std::vector<VEC4>& vertex_colors);
		void normals(const std::vector<VEC3>& normals);
		void tangents(const std::vector<VEC3>& tangents);
		void indices(const std::vector<UINT>& indices);
		void data(const MeshData& data);

		void vertices(std::vector<VEC3>&& vertices);
		void uv(std::vector<VEC2>&& uv);
		void vertexColors(std::vector<VEC4>&& vertex_colors);
		void normals(std::vector<VEC3>&& normals);
		void tangents(std::vector<VEC3>&& tangents);
		void indices(std::vector<UINT>&& indices);
		void data(MeshData&& data);

		UINT vertexSize() const;
		UINT normalSize() const;
		UINT tangentSize() const;
		UINT vertexColorSize() const;
		UINT uvSize() const;
		UINT indexSize() const;
		UINT vertexDataSize() const;

		SIZE_T vertexByteSize() const;
		SIZE_T normalByteSize() const;
		SIZE_T tangentByteSize() const;
		SIZE_T vertexColorByteSize() const;
		SIZE_T uvByteSize() const;
		SIZE_T indexByteSize() const;
		SIZE_T vertexDataByteSize() const;

		SIZE_T vertexByteOffset() const;
		SIZE_T normalByteOffset() const;
		SIZE_T tangentByteOffset() const;
		SIZE_T vertexColorByteOffset() const;
		SIZE_T uvByteOffset() const;
		SIZE_T indexByteOffset() const;




		std::type_index getType() const override;
		//const Component id() const override;
		void update() override;

	private:
		MeshData m_data;

	};


	class MeshRendererComponent : public BaseComponent{
	public:
		MeshRendererComponent(std::shared_ptr<MeshComponent> mesh, Material material);
		MeshRendererComponent(std::shared_ptr<MeshComponent> mesh);
		
		void material(Material material);
		Material& material();
		const Material& material() const;

		std::shared_ptr<MeshComponent> mesh();

		std::type_index getType() const override;
		void update() override;

	private:
		Material m_material;
		std::weak_ptr<MeshComponent> m_mesh;
	};

}