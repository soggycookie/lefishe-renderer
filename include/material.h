#pragma once
#include "global_header.h"
#include "id_generator.h"
#include "texture.h"

namespace Lefishe{

	class Program;

	enum class CullMode{
		BACK = GL_BACK,
		FRONT = GL_FRONT,
		FRONT_AND_BACK = GL_FRONT_AND_BACK
	};

	enum class BlendOp{
		ADD = GL_FUNC_ADD,
		SUBTRACT = GL_FUNC_SUBTRACT,
		REVERSE_SUBTRACT = GL_FUNC_REVERSE_SUBTRACT,
		MIN = GL_MIN,
		MAX = GL_MAX
	};

	enum class PolygonMode{
		POINT = GL_POINT,
		LINE = GL_LINE,
		FILL = GL_FILL
	};

	enum class BlendFunc{
		ZERO = GL_ZERO,
		ONE = GL_ONE,
		SRC_ALPHA = GL_SRC_ALPHA,
		ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
		DST_ALPHA = GL_DST_ALPHA,
		ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA
	};

	enum class DepthFunc{
		NEVER = GL_NEVER,
		LESS = GL_LESS,
		EQUAL = GL_EQUAL,
		LESS_EQUAL = GL_LEQUAL,
		GREATER = GL_GREATER,
		NOT_EQUAL = GL_NOTEQUAL,
		GREATER_EQUAL = GL_GEQUAL,
		ALWAYS = GL_ALWAYS
	};

	enum class GeometryType{
		OPAQUE_GEO,
		TRANSPARENT_GEO
	};

	struct MaterialSettings{
		bool depth_written = true;
		DepthFunc depth_func = DepthFunc::LESS;

		bool is_cull = false;
		CullMode cull_mode = CullMode::BACK;

		PolygonMode polygon_mode = PolygonMode::FILL;

		GeometryType geo_type = GeometryType::OPAQUE_GEO;
		
		bool enable_blend = true;
		BlendFunc src_factor = BlendFunc::SRC_ALPHA;
		BlendFunc dst_factor = BlendFunc::ONE_MINUS_SRC_ALPHA;
		BlendOp blend_op = BlendOp::ADD;
		//
		//float alpha_cutoff = 0.0f;
	};

#define DIFFUSE_UNIFORM_NAME "diffuseMap"
#define NORMAL_UNIFORM_NAME "normalMap"
#define METALLIC_UNIFROM_NAME "metallicMap"

	class Material{
	public:
		Material(std::shared_ptr<Program> program, MaterialSettings settings = {});
		~Material();

		UINT id() const;

		const std::shared_ptr<Program> program() const;

		void bindAndSetUniform() const;
		void cleanUpBinding() const;

		void setUniformData(const STRING& name, SIZE_T size, const void* data);

		INT  getTextureSlot(const STRING& name) const;
		
		void assignTexture(const STRING& name, std::shared_ptr<Texture> texture);

		MaterialSettings materialSettings() const;

	private:
		void getUniformInfo();
		void bindSettings() const;

	private:
		UINT m_id = 0;
		std::weak_ptr<Program> m_program;
		std::unordered_map<STRING, void*> m_uniform_data;
		std::unordered_map<STRING, std::weak_ptr<Texture>> m_textures;
		std::unordered_map<STRING, INT> m_texture_slots;
		MaterialSettings m_material_settings;
	};


}