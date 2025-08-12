#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include "component.h"
#include "object.h"


using namespace Lefishe;

BaseComponent::BaseComponent()
	:m_id(IDGenerator::Generate())
{
}

long BaseComponent::id() const{
	return m_id;
}

//const Component BaseComponent::id() const{
//	return Component::NONE;
//}

//Transform Component
TransformComponent::TransformComponent(std::shared_ptr<Object> owner, TransformData data)
	: m_owner(owner), m_data(data)
{
	//update();
}

const VEC3& TransformComponent::position() const {
	return m_data.m_position;
}

const VEC3& TransformComponent::rotation() const {
	return m_data.m_rotation;
}

const VEC3& TransformComponent::scale() const{
	return m_data.m_scale;
}

VEC3& TransformComponent::position() {
	is_dirty = true;
	return m_data.m_position;
}

VEC3& TransformComponent::rotation() {
	is_dirty = true;
	return m_data.m_rotation;
}

VEC3& TransformComponent::scale(){
	is_dirty = true;
	return m_data.m_scale;
}

const VEC3& TransformComponent::right() const {
	return m_right;
}

const VEC3& TransformComponent::forward() const {
	return m_forward;
}

const VEC3& TransformComponent::up() const {
	return m_up;
}

const MAT4& TransformComponent::rotationMtx() const {
	return m_rot_mtx;
}

const MAT4& TransformComponent::localToWorldMtx() const {
	return m_local_to_world_mtx;
}

const MAT4& TransformComponent::worldToLocalMtx() const {
	return m_world_to_local_mtx;
}


const MAT4& TransformComponent::localMtx() const{
	return m_local_mtx;
}

const MAT4& TransformComponent::globalMtx() const{
	return m_global_mtx;
}

void TransformComponent::localMtx(const MAT4& mtx){
	is_dirty = true;
	m_local_mtx = mtx;
}

void TransformComponent::globalMtx(const MAT4& mtx){
	is_dirty = true;
	m_global_mtx = mtx;
}

void TransformComponent::markDirty(){
	LOG_WARN("MARK DIRTY\n");
	is_dirty = true;
}

void TransformComponent::constructMatrix() {
	if (!is_dirty) {
		return;
	}

	if(m_data.m_rotation.x >= 360){
		m_data.m_rotation.x = m_data.m_rotation.x - 360;
	}

	if(m_data.m_rotation.y >= 360){
		m_data.m_rotation.y = m_data.m_rotation.y - 360;
	}

	if(m_data.m_rotation.z >= 360){
		m_data.m_rotation.z = m_data.m_rotation.z - 360;
	}


	float cx = glm::cos(glm::radians(m_data.m_rotation.x)), sx = glm::sin(glm::radians(m_data.m_rotation.x));
	float cy = glm::cos(glm::radians(m_data.m_rotation.y)), sy = glm::sin(glm::radians(m_data.m_rotation.y));
	float cz = glm::cos(glm::radians(m_data.m_rotation.z)), sz = glm::sin(glm::radians(m_data.m_rotation.z));

	//Z * Y * X order -> X -> Y -> Z
	//TODO: support quaternion

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

	glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_data.m_position);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_data.m_scale);

	m_local_mtx = translation * m_rot_mtx * scale;
	

	if(auto owner = m_owner.lock()){
		auto parent = owner->parent();

		if(parent != nullptr){
			auto pt = parent->getComponent<TransformComponent>();
			m_global_mtx = pt->globalMtx() * m_local_mtx;
			
		}else{
			m_global_mtx = m_local_mtx;		
		}
		owner->onTransformChanged();
	}


	m_local_to_world_mtx = m_global_mtx ;
	m_world_to_local_mtx = glm::inverse(m_local_to_world_mtx);

	//LOG_TRACE("global\n {0} \n", glm::to_string(m_global_mtx));
	//LOG_TRACE("local\n {0} \n", glm::to_string(m_local_mtx));
	
	is_dirty = false;
}

void TransformComponent::update(){
	constructMatrix();
}

//const Component TransformComponent::id() const{
//	return Component::TRANSFORM;
//}

std::type_index TransformComponent::getType() const {
	return std::type_index(typeid(TransformComponent));
}

//Camera Component

CameraComponent::CameraComponent(CameraInfo info)
	: m_camera_info(info)
{
	update();
}

std::shared_ptr<CameraComponent> CameraComponent::m_main_cam = nullptr;

std::shared_ptr<CameraComponent> CameraComponent::main() {
	return m_main_cam;
}

void CameraComponent::main(std::shared_ptr<CameraComponent> main) {
	m_main_cam = main;
}

void CameraComponent::fov(const FLOAT& fov) {
	m_camera_info.fov = fov;
	is_dirty = true;
}

const FLOAT& CameraComponent::fov() const {
	return m_camera_info.fov;
}

FLOAT& CameraComponent::fov() {
	is_dirty = true;
	return m_camera_info.fov;
}

void CameraComponent::nearPlane(FLOAT n) {
	m_camera_info.near_plane = n;
}

const FLOAT& CameraComponent::nearPlane() const {
	return m_camera_info.near_plane;
}

FLOAT& CameraComponent::nearPlane() {
	return m_camera_info.near_plane;
}

void CameraComponent::farPlane(FLOAT f) {
	m_camera_info.far_plane = f;
}

const FLOAT& CameraComponent::farPlane() const {
	return m_camera_info.far_plane;
}

FLOAT& CameraComponent::farPlane() {
	return m_camera_info.far_plane;
}

const MAT4& CameraComponent::perspectiveMtx() const{
	return m_perspective_mtx;
}

void CameraComponent::check() {
	if (m_camera_info.far_plane > MAX_FAR_PLANE) {
		m_camera_info.far_plane = MAX_FAR_PLANE;
	}

	if (m_camera_info.far_plane <= 0) {
		m_camera_info.far_plane = DEFAULT_FAR_PLANE;
	}

	if (m_camera_info.near_plane <= 0) {
		m_camera_info.near_plane = DEFAULT_NEAR_PLANE;
	}

	if (m_camera_info.near_plane >= m_camera_info.far_plane) {
		m_camera_info.near_plane = DEFAULT_NEAR_PLANE;
		m_camera_info.far_plane  = DEFAULT_FAR_PLANE;
	}
}

void CameraComponent::constructMatrix(){

	if(!is_dirty){
		return;
	}

	check();

	m_perspective_mtx =
		glm::perspective(glm::radians(m_camera_info.fov), m_camera_info.aspect, 
			m_camera_info.near_plane, m_camera_info.far_plane);

	is_dirty = false;
}

//const Component CameraComponent::id() const{
//	return Component::CAMERA;
//}

std::type_index CameraComponent::getType() const {
	return std::type_index(typeid(CameraComponent));
}

void CameraComponent::update(){
	constructMatrix();
}

// Mesh Component

void MeshData::clear() {
	indices.clear();
	vertices.clear();
	normals.clear();
	tangents.clear();
	uv.clear();
	vertex_colors.clear();
}

//const Component MeshComponent::id() const{
//	return Component::MESH;
//}

MeshComponent::MeshComponent(const MeshData& data)
	: m_data(data)
{
}

MeshComponent::MeshComponent(MeshData&& data)
	: m_data(std::move(data))
{
}

const std::vector<VEC3>& MeshComponent::vertices() const{
	return m_data.vertices;
}

const std::vector<VEC2>& MeshComponent::uv() const{
	return m_data.uv;
}

const std::vector<VEC4>& MeshComponent::vertexColors() const{
	return m_data.vertex_colors;
}

const std::vector<VEC3>& MeshComponent::normals() const{
	return m_data.normals;
}

const std::vector<VEC3>& MeshComponent::tangents() const{
	return m_data.tangents;
}

const std::vector<UINT>& MeshComponent::indices() const{
	return m_data.indices;
}

const MeshData& MeshComponent::data() const{
	return m_data;
}

void MeshComponent::vertices(const std::vector<VEC3>& vertices){
	m_data.vertices = vertices;
}

void MeshComponent::uv(const std::vector<VEC2>& uv){
	m_data.uv = uv;
}

void MeshComponent::vertexColors(const std::vector<VEC4>& vertex_colors){
	m_data.vertex_colors = vertex_colors;
}

void MeshComponent::normals(const std::vector<VEC3>& normals){
	m_data.normals = normals;
}

void MeshComponent::tangents(const std::vector<VEC3>& tangents){
	m_data.tangents = tangents;
}

void MeshComponent::indices(const std::vector<UINT>& indices){
	m_data.indices = indices;
}

void MeshComponent::data(const MeshData& data){
	m_data = data;
}

void MeshComponent::vertices(std::vector<VEC3>&& vertices){
	m_data.vertices = std::move(vertices);
}

void MeshComponent::uv(std::vector<VEC2>&& uv){
	m_data.uv = std::move(uv);
}

void MeshComponent::vertexColors(std::vector<VEC4>&& vertex_colors){
	m_data.vertex_colors = std::move(vertex_colors);
}

void MeshComponent::normals(std::vector<VEC3>&& normals){
	m_data.normals = std::move(normals);
}

void MeshComponent::tangents(std::vector<VEC3>&& tangents){
	m_data.tangents = std::move(tangents);
}

void MeshComponent::indices(std::vector<UINT>&& indices){
	m_data.indices = std::move(indices);
}

void MeshComponent::data(MeshData&& data){
	m_data = std::move(data);
}

UINT MeshComponent::vertexSize() const{
	return vertices().size();
}

UINT MeshComponent::normalSize() const{
	return normals().size();
}

UINT MeshComponent::tangentSize() const{
	return tangents().size();
}

UINT MeshComponent::vertexColorSize() const{
	return vertexColors().size();
}

UINT MeshComponent::uvSize() const{
	return uv().size();
}

UINT MeshComponent::vertexDataSize() const{
	return vertexSize() + normalSize() + tangentSize() + vertexColorSize() + uvSize();
}

UINT MeshComponent::indexSize() const{
	return indices().size();
}

SIZE_T MeshComponent::vertexByteSize() const{
	return vertexSize() * sizeof(VEC3);
}

SIZE_T MeshComponent::normalByteSize() const{
	return normalSize() * sizeof(VEC3);
}

SIZE_T MeshComponent::tangentByteSize() const{
	return tangentSize() * sizeof(VEC3);
}

SIZE_T MeshComponent::vertexColorByteSize() const{
	return vertexColorSize() * sizeof(VEC4);
}

SIZE_T MeshComponent::uvByteSize() const{
	return uvSize() * sizeof(VEC2);
}

SIZE_T MeshComponent::indexByteSize() const{
	return indexSize() * sizeof(UINT);
}

SIZE_T MeshComponent::vertexDataByteSize() const{
	return vertexByteSize() + normalByteSize() + tangentByteSize() + vertexColorByteSize() + uvByteSize();
}

SIZE_T MeshComponent::vertexByteOffset() const{
	return 0;	
}

SIZE_T MeshComponent::normalByteOffset() const{
	return vertexByteOffset() + vertexByteSize();
}

SIZE_T MeshComponent::tangentByteOffset() const{
	return normalByteOffset() + normalByteSize();
}

SIZE_T MeshComponent::vertexColorByteOffset() const{
	return tangentByteOffset() + tangentByteSize();
}

SIZE_T MeshComponent::uvByteOffset() const{
	return vertexColorByteOffset() + vertexColorByteSize();
}
SIZE_T MeshComponent::indexByteOffset() const{
	return uvByteOffset() + uvByteSize();
}

std::type_index MeshComponent::getType() const {
	return std::type_index(typeid(MeshComponent));
}

void MeshComponent::update() {
}

MeshRendererComponent::MeshRendererComponent(std::shared_ptr<MeshComponent> mesh, Material material)
	:	m_mesh(mesh), m_material(material)
{	
}

MeshRendererComponent::MeshRendererComponent(std::shared_ptr<MeshComponent> mesh)
	: m_mesh(mesh)
{
}

void MeshRendererComponent::material(Material material){
	m_material = material; 
}

Material& MeshRendererComponent::material(){
	return m_material;
}

const Material& MeshRendererComponent::material() const{
	return m_material;
}

std::type_index MeshRendererComponent::getType() const{
	return std::type_index(typeid(MeshRendererComponent));
}

std::shared_ptr<MeshComponent> MeshRendererComponent::mesh(){
	return m_mesh.lock();
}

void MeshRendererComponent::update(){
}



