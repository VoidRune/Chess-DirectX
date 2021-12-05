#pragma once

constexpr unsigned int maxQuadCount = 1000;
constexpr unsigned int maxVertexCount = maxQuadCount * 4;
constexpr unsigned int maxIndexCount = maxQuadCount * 6;

struct Vertex
{
    float Position[3];
    float TexCoord[2];
    float TexID;
};