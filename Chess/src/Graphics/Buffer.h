#pragma once
#include "Graphics.h"

class VertexBuffer
{
public:
	VertexBuffer();
	VertexBuffer(uint32_t numOfVertices, bool dynamic = false);
	VertexBuffer(std::vector<Vertex>& data, uint32_t numOfVertices, bool dynamic = false);
	VertexBuffer(const Vertex* data, uint32_t numOfVertices, bool dynamic = false);
	~VertexBuffer();
	void Load(std::vector<Vertex>& data);
	void Load(const Vertex* data, int size);
	void LoadPart(std::vector<Vertex>& data, size_t offset);
	void Create(std::vector<Vertex>& data, uint32_t numOfVertices, bool dynamic = false);
	void Bind();
private:
	ID3D11Buffer* vertex_buffer_ptr = nullptr;
	UINT vertex_stride;
	UINT vertex_offset;
};

class IndexBuffer
{
public:
	IndexBuffer();
	IndexBuffer(std::vector<unsigned int>& data);
	IndexBuffer(const unsigned int* data);
	~IndexBuffer();
	//may be used once
	void Set(std::vector<unsigned int>& data);
	//may be used once
	void Set(const unsigned int* data);
	void Bind();
private:
	ID3D11Buffer* index_buffer_ptr = nullptr;
};

class ConstantBuffer
{
public:
	ConstantBuffer();
	ConstantBuffer(void* data, const int size, int slot);
	//may only be used once
	void Set(void* data, const int size, int slot);
	~ConstantBuffer();
	void Load(void* data, const int size);
	void Bind(int slot);
private:
	ID3D11Buffer* const_buffer_ptr = nullptr;
};
