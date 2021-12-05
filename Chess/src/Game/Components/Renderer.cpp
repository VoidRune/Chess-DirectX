#include "Renderer.h"

Renderer::Renderer():
	textures(paths, 14, true),
	shader(L"res/Shaders/shaders.hlsl", L"res/Shaders/shaders.hlsl")
{
	shader.Bind();
	textures.Bind(0);

	struct ConstantBuffer
	{
		DirectX::XMMATRIX transform;
	} cb;
	DirectX::XMMATRIX proj_matrix = DirectX::XMMatrixOrthographicOffCenterLH(0, 1920, 0, 1080, -10, 10);
	cb.transform = DirectX::XMMatrixTranspose(proj_matrix);
	constBuffer.Set(&cb, sizeof(cb), 0);

	unsigned int* indices = new unsigned int[maxIndexCount];
	uint32_t offset = 0;
	for (size_t i = 0; i < maxIndexCount; i += 6)
	{
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 2 + offset;

		indices[i + 3] = 2 + offset;
		indices[i + 4] = 3 + offset;
		indices[i + 5] = 0 + offset;

		offset += 4;
	}

	indexBuffer.Set(indices);
	indexBuffer.Bind();
	delete[] indices;
}

Renderer::~Renderer()
{

}

void Renderer::Initialize(ChessPiece* squares, const std::string& fen, bool isWhite, int width, int height)
{

	this->squares = squares;
	shader.Bind();
	textures.Bind(0);

	m_Width = width;
	m_Height = height;

	boardData.clear();
	boardData.reserve(m_Width * m_Height * 4);
	piecesData.clear();
	piecesData.reserve(m_Width * m_Height * 4);

	for (float x = 0; x < m_Width; x++)
	{
		for (float y = 0; y < m_Height; y++)
		{
			boardData.emplace_back(Vertex{ 420 + x * 135, y * 135            , 5, 0, 0, (float)((int)(x + y) % 2 + 12) });
			boardData.emplace_back(Vertex{ 420 + x * 135, (y + 1) * 135      , 5, 0, 1, (float)((int)(x + y) % 2 + 12) });
			boardData.emplace_back(Vertex{ 420 + (x + 1) * 135, (y + 1) * 135, 5, 1, 1, (float)((int)(x + y) % 2 + 12) });
			boardData.emplace_back(Vertex{ 420 + (x + 1) * 135, y * 135      , 5, 1, 0, (float)((int)(x + y) % 2 + 12) });
		}
	}
	boardBuffer.Create(boardData, m_Width * m_Height * 4, true);
	
	LoadPositionFromFen(fen, isWhite);
	int arrayID = 0;
	for (size_t i = 0; i < m_Width * m_Height; i++)
	{
		if (squares[i].type == -1)
			continue;

		squares[i].arrayID = arrayID;
		arrayID++;
		piecesData.emplace_back(Vertex{ 420.0f + i % m_Width * 135.0f, (m_Width * m_Height - 1 - i) / m_Height * 135.0f            , 0.0f, 0.0f, 0.0f, (float)(squares[i].type) });
		piecesData.emplace_back(Vertex{ 420.0f + i % m_Width * 135.0f, ((m_Width * m_Height - 1 - i) / m_Height + 1) * 135.0f      , 0.0f, 0.0f, 1.0f, (float)(squares[i].type) });
		piecesData.emplace_back(Vertex{ 420.0f + (i % m_Width + 1) * 135.0f, ((m_Width * m_Height - 1 - i) / m_Height + 1) * 135.0f, 0.0f, 1.0f, 1.0f, (float)(squares[i].type) });
		piecesData.emplace_back(Vertex{ 420.0f + (i % m_Width + 1) * 135.0f, (m_Width * m_Height - 1 - i) / m_Height * 135.0f      , 0.0f, 1.0f, 0.0f, (float)(squares[i].type) });
	}
	
	piecesBuffer.Create(piecesData, m_Width * m_Height * 4, true);
}

void Renderer::MovePiece(float x, float y, int num)
{
	if (num > -1 && num < m_Width * m_Height && squares[num].type > -1)
	{
		std::vector<Vertex> tempData;

		tempData.emplace_back(Vertex{ 420.0f + (x - 0.5f) * 135.0f,	(y - 0.5f) * 135.0f ,-1.0f  ,0.0f,0.0f,(float)squares[num].type });
		tempData.emplace_back(Vertex{ 420.0f + (x - 0.5f) * 135.0f,	(y + 0.5f) * 135.0f ,-1.0f  ,0.0f,1.0f,(float)squares[num].type });
		tempData.emplace_back(Vertex{ 420.0f + (x + 0.5f) * 135.0f, (y + 0.5f) * 135.0f	,-1.0f  ,1.0f,1.0f,(float)squares[num].type });
		tempData.emplace_back(Vertex{ 420.0f + (x + 0.5f) * 135.0f, (y - 0.5f) * 135.0f	,-1.0f  ,1.0f,0.0f,(float)squares[num].type });

		piecesBuffer.LoadPart(tempData, squares[num].arrayID * 4);
	}
}

void Renderer::SetPiece(int previous, int current)
{
	if (previous > -1 && previous < 64 && current > -1 && current < 64 && squares[previous].type > -1)
	{
		std::vector<Vertex> tempData;
		tempData.reserve(4);
		if (current != previous)
		{
			//tell the eaten piece where to go
			if (squares[current].type != -1)
			{
				tempData.emplace_back(Vertex{ 0.0f });
				tempData.emplace_back(Vertex{ 0.0f });
				tempData.emplace_back(Vertex{ 0.0f });
				tempData.emplace_back(Vertex{ 0.0f });

				piecesBuffer.LoadPart(tempData, squares[current].arrayID * 4);
				tempData.clear();
				tempData.reserve(4);
			}
			squares[current] = squares[previous];

			squares[previous].type = -1;
			squares[previous].arrayID = -1;
		}

		tempData.emplace_back(Vertex{ 420.0f + current % m_Width * 135.0f, (m_Width * m_Height - 1 - current) / m_Height * 135.0f             ,0.0f  ,0.0f,0.0f,(float)squares[current].type });
		tempData.emplace_back(Vertex{ 420.0f + current % m_Width * 135.0f, ((m_Width * m_Height - 1 - current) / m_Height + 1) * 135.0f       ,0.0f  ,0.0f,1.0f,(float)squares[current].type });
		tempData.emplace_back(Vertex{ 420.0f + (current % m_Width + 1) * 135.0f, ((m_Width * m_Height - 1 - current) / m_Height + 1) * 135.0f ,0.0f  ,1.0f,1.0f,(float)squares[current].type });
		tempData.emplace_back(Vertex{ 420.0f + (current % m_Width + 1) * 135.0f, (m_Width * m_Height - 1 - current) / m_Height * 135.0f       ,0.0f  ,1.0f,0.0f,(float)squares[current].type });

		piecesBuffer.LoadPart(tempData, squares[current].arrayID * 4);
	}
}

void Renderer::LoadPositionFromFen(const std::string& fen, bool isWhite)
{

	std::string copy = fen;
	if (!isWhite)
	{
		std::reverse(copy.begin(), copy.end());
	}

	int pos = 0;
	for (char symbol : copy)
	{
		if (symbol != '/')
		{
			switch (symbol)
			{
			case 'P':
				squares[pos].type = 0;
				break;
			case 'N':
				squares[pos].type = 1;
				break;
			case 'B':
				squares[pos].type = 2;
				break;
			case 'R':
				squares[pos].type = 3;
				break;
			case 'Q':
				squares[pos].type = 4;
				break;
			case 'K':
				squares[pos].type = 5;
				break;
			case 'p':
				squares[pos].type = 6;
				break;
			case 'n':
				squares[pos].type = 7;
				break;
			case 'b':
				squares[pos].type = 8;
				break;
			case 'r':
				squares[pos].type = 9;
				break;
			case 'q':
				squares[pos].type = 10;
				break;
			case 'k':
				squares[pos].type = 11;
				break;
			case '1':
				squares[pos].type = -1;
				break;
			case '2':
				for (size_t i = 0; i < 2; i++)
				{
					squares[pos].type = -1;
					pos++;
				}
				pos--;
				break;
			case '3':
				for (size_t i = 0; i < 3; i++)
				{
					squares[pos].type = -1;
					pos++;
				}
				pos--;
				break;
			case '4':
				for (size_t i = 0; i < 4; i++)
				{
					squares[pos].type = -1;
					pos++;
				}
				pos--;
				break;
			case '5':
				for (size_t i = 0; i < 5; i++)
				{
					squares[pos].type = -1;
					pos++;
				}
				pos--;
				break;
			case '6':
				for (size_t i = 0; i < 6; i++)
				{
					squares[pos].type = -1;
					pos++;
				}
				pos--;
				break;
			case '7':
				for (size_t i = 0; i < 7; i++)
				{
					squares[pos].type = -1;
					pos++;
				}
				pos--;
				break;
			case '8':
				for (size_t i = 0; i < 8; i++)
				{
					squares[pos + i].type = -1;
				}
				pos+=7;
				break;
			}
			pos++;
		}
	}
}

void Renderer::DrawBoard()
{
	boardBuffer.Bind();
	Graphics::GetDeviceContext()->DrawIndexed(m_Width * m_Height * 6, 0, 0);
}

void Renderer::DrawPieces()
{
	piecesBuffer.Bind();
	Graphics::GetDeviceContext()->DrawIndexed(m_Width * m_Height * 6, 0, 0);
}

void Renderer::SetTexture(int slot)
{
	textures.Bind(slot);
	shader.Bind();
}