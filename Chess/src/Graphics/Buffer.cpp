#include "Buffer.h"

VertexBuffer::VertexBuffer() {};
VertexBuffer::VertexBuffer(uint32_t numOfVertices, bool dynamic)
{
    vertex_stride = sizeof(Vertex);
    vertex_offset = 0;

    D3D11_BUFFER_DESC vertex_buff_descr = {};
    vertex_buff_descr.ByteWidth = sizeof(Vertex) * numOfVertices;
    vertex_buff_descr.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
    vertex_buff_descr.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertex_buff_descr.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
    D3D11_SUBRESOURCE_DATA sr_data = { 0 };
    HRESULT hr = Graphics::GetDevice()->CreateBuffer(
        &vertex_buff_descr,
        nullptr,
        &vertex_buffer_ptr);
    assert(SUCCEEDED(hr));
}
VertexBuffer::VertexBuffer(std::vector<Vertex>& data, uint32_t numOfVertices, bool dynamic)
{
    vertex_stride = sizeof(Vertex);
    vertex_offset = 0;

    if (data.size() > 0)
    {
        vertex_stride = sizeof(Vertex);
        vertex_offset = 0;

        D3D11_BUFFER_DESC vertex_buff_descr = {};
        vertex_buff_descr.ByteWidth = sizeof(Vertex) * numOfVertices;
        vertex_buff_descr.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
        vertex_buff_descr.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertex_buff_descr.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
        D3D11_SUBRESOURCE_DATA sr_data = { 0 };
        sr_data.pSysMem = data.data();
        HRESULT hr = Graphics::GetDevice()->CreateBuffer(
            &vertex_buff_descr,
            &sr_data,
            &vertex_buffer_ptr);
        assert(SUCCEEDED(hr));
    }
}
VertexBuffer::VertexBuffer(const Vertex* data, uint32_t numOfVertices, bool dynamic)
{
    vertex_stride = sizeof(Vertex);
    vertex_offset = 0;

    D3D11_BUFFER_DESC vertex_buff_descr = {};
    vertex_buff_descr.ByteWidth = sizeof(Vertex) * numOfVertices;
    vertex_buff_descr.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
    vertex_buff_descr.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertex_buff_descr.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
    D3D11_SUBRESOURCE_DATA sr_data = { 0 };
    sr_data.pSysMem = data;
    HRESULT hr = Graphics::GetDevice()->CreateBuffer(
        &vertex_buff_descr,
        &sr_data,
        &vertex_buffer_ptr);
    assert(SUCCEEDED(hr));
}
VertexBuffer::~VertexBuffer()
{
    SafeRelease(&vertex_buffer_ptr);
}
void VertexBuffer::Load(std::vector<Vertex>& data)
{
    if (data.size() > 0)
    {
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

        Graphics::GetDeviceContext()->Map(vertex_buffer_ptr, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        memcpy(mappedResource.pData, data.data(), sizeof(Vertex) * data.size());
        Graphics::GetDeviceContext()->Unmap(vertex_buffer_ptr, 0);
    }
}
void VertexBuffer::Load(const Vertex* data, int size)
{
    if (size > 0)
    {
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

        Graphics::GetDeviceContext()->Map(vertex_buffer_ptr, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        memcpy(mappedResource.pData, data, sizeof(Vertex) * size);
        Graphics::GetDeviceContext()->Unmap(vertex_buffer_ptr, 0);
    }
}
void VertexBuffer::LoadPart(std::vector<Vertex>& data, size_t offset = 0)
{
    if (data.size() > 0)
    {
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

        Graphics::GetDeviceContext()->Map(vertex_buffer_ptr, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource);
        memcpy((Vertex*)mappedResource.pData + offset, data.data(), sizeof(Vertex) * data.size());
        Graphics::GetDeviceContext()->Unmap(vertex_buffer_ptr, 0);
    }
}
void VertexBuffer::Create(std::vector<Vertex>& data, uint32_t numOfVertices, bool dynamic)
{
    vertex_stride = sizeof(Vertex);
    vertex_offset = 0;

    if (data.size() > 0)
    {
        vertex_stride = sizeof(Vertex);
        vertex_offset = 0;

        D3D11_BUFFER_DESC vertex_buff_descr = {};
        vertex_buff_descr.ByteWidth = sizeof(Vertex) * numOfVertices;
        vertex_buff_descr.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
        vertex_buff_descr.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertex_buff_descr.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
        D3D11_SUBRESOURCE_DATA sr_data = { 0 };
        sr_data.pSysMem = data.data();
        HRESULT hr = Graphics::GetDevice()->CreateBuffer(
            &vertex_buff_descr,
            &sr_data,
            &vertex_buffer_ptr);
        assert(SUCCEEDED(hr));
    }
}
void VertexBuffer::Bind()
{
    if (vertex_buffer_ptr != nullptr)
    {
        Graphics::GetDeviceContext()->IASetVertexBuffers(
            0,
            1,
            &vertex_buffer_ptr,
            &vertex_stride,
            &vertex_offset);
    }
}

IndexBuffer::IndexBuffer() {}
IndexBuffer::IndexBuffer(std::vector<unsigned int>& data)
{
    D3D11_BUFFER_DESC index_buff_descr = {};
    index_buff_descr.ByteWidth = sizeof(unsigned int) * maxIndexCount;
    index_buff_descr.Usage = D3D11_USAGE_DEFAULT;
    index_buff_descr.BindFlags = D3D11_BIND_INDEX_BUFFER;
    index_buff_descr.StructureByteStride = sizeof(unsigned int);
    index_buff_descr.CPUAccessFlags = 0;
    index_buff_descr.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA sr_data = { 0 };
    sr_data.pSysMem = data.data();
    sr_data.SysMemPitch = 0;
    sr_data.SysMemSlicePitch = 0;

    HRESULT hr = Graphics::GetDevice()->CreateBuffer(
        &index_buff_descr,
        &sr_data,
        &index_buffer_ptr);
    assert(SUCCEEDED(hr));

}
IndexBuffer::IndexBuffer(const unsigned int* data)
{
    D3D11_BUFFER_DESC index_buff_descr = {};
    index_buff_descr.ByteWidth = sizeof(unsigned int) * maxIndexCount;
    index_buff_descr.Usage = D3D11_USAGE_DEFAULT;
    index_buff_descr.BindFlags = D3D11_BIND_INDEX_BUFFER;
    index_buff_descr.StructureByteStride = sizeof(unsigned int);
    index_buff_descr.CPUAccessFlags = 0;
    index_buff_descr.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA sr_data = { 0 };
    sr_data.pSysMem = data;
    sr_data.SysMemPitch = 0;
    sr_data.SysMemSlicePitch = 0;

    HRESULT hr = Graphics::GetDevice()->CreateBuffer(
        &index_buff_descr,
        &sr_data,
        &index_buffer_ptr);
    assert(SUCCEEDED(hr));

}
IndexBuffer::~IndexBuffer()
{
    SafeRelease(&index_buffer_ptr);
}
void IndexBuffer::Set(std::vector<unsigned int>& data)
{
    if (!index_buffer_ptr)
    {
        D3D11_BUFFER_DESC index_buff_descr = {};
        index_buff_descr.ByteWidth = sizeof(unsigned int) * maxIndexCount;
        index_buff_descr.Usage = D3D11_USAGE_DEFAULT;
        index_buff_descr.BindFlags = D3D11_BIND_INDEX_BUFFER;
        index_buff_descr.StructureByteStride = sizeof(unsigned int);
        index_buff_descr.CPUAccessFlags = 0;
        index_buff_descr.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA sr_data = { 0 };
        sr_data.pSysMem = data.data();
        sr_data.SysMemPitch = 0;
        sr_data.SysMemSlicePitch = 0;

        HRESULT hr = Graphics::GetDevice()->CreateBuffer(
            &index_buff_descr,
            &sr_data,
            &index_buffer_ptr);
        assert(SUCCEEDED(hr));
    }
}
void IndexBuffer::Set(const unsigned int* data)
{
    if (!index_buffer_ptr)
    {
        D3D11_BUFFER_DESC index_buff_descr = {};
        index_buff_descr.ByteWidth = sizeof(unsigned int) * maxIndexCount;
        index_buff_descr.Usage = D3D11_USAGE_DEFAULT;
        index_buff_descr.BindFlags = D3D11_BIND_INDEX_BUFFER;
        index_buff_descr.StructureByteStride = sizeof(unsigned int);
        index_buff_descr.CPUAccessFlags = 0;
        index_buff_descr.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA sr_data = { 0 };
        sr_data.pSysMem = data;
        sr_data.SysMemPitch = 0;
        sr_data.SysMemSlicePitch = 0;

        HRESULT hr = Graphics::GetDevice()->CreateBuffer(
            &index_buff_descr,
            &sr_data,
            &index_buffer_ptr);
        assert(SUCCEEDED(hr));
    }
}
void IndexBuffer::Bind()
{
    Graphics::GetDeviceContext()->IASetIndexBuffer(
        index_buffer_ptr,
        DXGI_FORMAT_R32_UINT,
        0);
}

ConstantBuffer::ConstantBuffer() {}

ConstantBuffer::ConstantBuffer(void* data, const int size, int slot)
{
    D3D11_BUFFER_DESC const_buff_descr = {};
    const_buff_descr.ByteWidth = sizeof(size);
    const_buff_descr.Usage = D3D11_USAGE_DYNAMIC;
    const_buff_descr.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    const_buff_descr.StructureByteStride = 0;
    const_buff_descr.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    const_buff_descr.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA sr_data = { 0 };
    sr_data.pSysMem = data;
    sr_data.SysMemPitch = 0;
    sr_data.SysMemSlicePitch = 0;

    HRESULT hr = Graphics::GetDevice()->CreateBuffer(
        &const_buff_descr,
        &sr_data,
        &const_buffer_ptr);
    assert(SUCCEEDED(hr));

    Graphics::GetDeviceContext()->VSSetConstantBuffers(slot, 1, &const_buffer_ptr);
}
void ConstantBuffer::Set(void* data, const int size, int slot)
{
    if (!const_buffer_ptr)
    {
        D3D11_BUFFER_DESC const_buff_descr = {};
        const_buff_descr.ByteWidth = size;
        const_buff_descr.Usage = D3D11_USAGE_DYNAMIC;
        const_buff_descr.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        const_buff_descr.StructureByteStride = 0;
        const_buff_descr.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        const_buff_descr.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA sr_data = { 0 };
        sr_data.pSysMem = data;
        sr_data.SysMemPitch = 0;
        sr_data.SysMemSlicePitch = 0;

        HRESULT hr = Graphics::GetDevice()->CreateBuffer(
            &const_buff_descr,
            &sr_data,
            &const_buffer_ptr);
        assert(SUCCEEDED(hr));

        Graphics::GetDeviceContext()->VSSetConstantBuffers(slot, 1, &const_buffer_ptr);
    }
}
ConstantBuffer::~ConstantBuffer()
{
    SafeRelease(&const_buffer_ptr);
}
void ConstantBuffer::Load(void* data, const int size)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

    Graphics::GetDeviceContext()->Map(const_buffer_ptr, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy(mappedResource.pData, data, sizeof(Vertex) * size);
    Graphics::GetDeviceContext()->Unmap(const_buffer_ptr, 0);
}
void ConstantBuffer::Bind(int slot)
{
    Graphics::GetDeviceContext()->VSSetConstantBuffers(slot, 1, &const_buffer_ptr);
}