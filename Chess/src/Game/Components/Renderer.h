#pragma once
#include "Graphics/Buffer.h"
#include "Graphics/Texture.h"
#include "Graphics/Shader.h"
#include <string>

struct ChessPiece
{
	int type = -1;
	int arrayID = -1;
};

class Renderer
{
public:
	Renderer();
	~Renderer();
	void Initialize(ChessPiece* squares, const std::string& fen, bool isWhite, int width = 8, int height = 8);
	void MovePiece(float x, float y, int num);
	void SetPiece(int previous, int current);
	void DrawBoard();
	void DrawPieces();

	void SetTexture(int slot);
private:
	void LoadPositionFromFen(const std::string& fen, bool isWhite);

	int m_Width = 0;
	int m_Height = 0;

	ChessPiece* squares;

	const char* paths[14] =
	{
		"res/Textures/Tiles/Pawn_White.png",
		"res/Textures/Tiles/Knight_White.png",
		"res/Textures/Tiles/Bishop_White.png",
		"res/Textures/Tiles/Rook_White.png",
		"res/Textures/Tiles/Queen_White.png",
		"res/Textures/Tiles/king_White.png",
		"res/Textures/Tiles/Pawn_Black.png",
		"res/Textures/Tiles/Knight_Black.png",
		"res/Textures/Tiles/Bishop_Black.png",
		"res/Textures/Tiles/Rook_Black.png",
		"res/Textures/Tiles/Queen_Black.png",
		"res/Textures/Tiles/king_Black.png",
		"res/Textures/Tiles/Tile_Black.png",
		"res/Textures/Tiles/Tile_White.png"
	};

	TextureArray textures;
	Shader shader;

	ConstantBuffer constBuffer;

	std::vector<Vertex> piecesData;
	std::vector<Vertex> boardData;

	VertexBuffer piecesBuffer;
	VertexBuffer boardBuffer;

	IndexBuffer indexBuffer;

};