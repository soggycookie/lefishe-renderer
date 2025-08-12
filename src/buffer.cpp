#include "buffer.h"

using namespace Lefishe;


Buffer::Buffer(UINT total_size, const void* data)
	: m_size(total_size)
{
	create(data);
}

void Buffer::create(const void* data){
	if(m_id > 0){
		LOG_WARN("[BUFFER] Buffer is already created!");
		return;
	}

	glCreateBuffers(1, &m_id);

	glNamedBufferStorage(m_id, m_size, data, GL_DYNAMIC_STORAGE_BIT);
}

void Buffer::create(UINT total_size, const void* data){
	if(m_id > 0){
		LOG_WARN("[BUFFER] Buffer is already created!");
		return;
	}

	if(m_size > 0){
		LOG_WARN("[BUFFER] Buffer's size is already defined!");
		return;
	}

	glCreateBuffers(1, &m_id);


	LOG_TRACE("[BUFFER] ID: {0}", m_id);
	

	glNamedBufferStorage(m_id, total_size, data, GL_DYNAMIC_STORAGE_BIT);
}

void Buffer::clear(){
	glDeleteBuffers(1, &m_id);
	m_id = 0;
}

void Buffer::bindIndex(BufferBindTarget type, int index){
	glBindBufferBase(type, index, m_id);
}

void Buffer::bind(BufferTarget target) const{
	glBindBuffer(target, m_id);
}

void Buffer::subData(UINT size, SIZE_T offset, const void* data){
	glNamedBufferSubData(m_id, offset, size, data);
}

UINT Buffer::id() const{
	return m_id;
}

UINT Buffer::size() const{
	return m_size;
}
