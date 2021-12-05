#pragma once
#include "Graphics.h"

class Shader
{
public:
	Shader(const wchar_t* vertexName, const wchar_t* pixelName);
	~Shader();
	void Bind();
private:

	ID3D11VertexShader* vertex_shader_ptr = {};
	ID3D11PixelShader* pixel_shader_ptr = {};
};

