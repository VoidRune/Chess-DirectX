#include "Game.h"
#include "../Window/Window.h"
#include "../Vendor/ImGui/imgui.h"

Game::Game() : 
	renderTex(Window::m_Width, Window::m_Height),
	postProcShader(L"res/Shaders/postProcessing.hlsl", L"res/Shaders/postProcessing.hlsl")
{
	squares = new ChessPiece[64];

	std::string starterFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
	renderer.Initialize(squares, starterFen, true);

	audio.LoadData(placeSound, "res/Audio/lockbox_open.ogg");
	audio.LoadData(backgroundAudio, "res/Audio/Cradle.ogg");
	
	settings::gameChannel = audio.CreateChannel(32);
	settings::backgroundChannel = audio.CreateChannel(4);

	audio.Play3D(backgroundAudio, settings::backgroundChannel, {0, 0, 0});

	moves.emplace_back(PossibleMoves{ false, { -9,-8,-7,-1,1,7,8,9 } });

	renderTex.BindAsTexture(0);

	std::vector<Vertex> postProcessData;

	//postProcessData.emplace_back(Vertex{ 0.5f,  -1      ,0.0f  ,0.0f ,1.0f, 0 });
	//postProcessData.emplace_back(Vertex{ 0.5f,  -0.5f	,0.0f  ,0.0f ,0.0f, 0 });
	//postProcessData.emplace_back(Vertex{ 1   ,	-0.5f	,0.0f  ,1.0f ,0.0f, 0 });
	//postProcessData.emplace_back(Vertex{ 1   ,	-1      ,0.0f  ,1.0f ,1.0f, 0 });

	postProcessData.emplace_back(Vertex{-1, -1  ,0.0f  ,0.0f ,1.0f, 0 });
	postProcessData.emplace_back(Vertex{-1,  1	,0.0f  ,0.0f ,0.0f, 0 });
	postProcessData.emplace_back(Vertex{ 1,	 1	,0.0f  ,1.0f ,0.0f, 0 });
	postProcessData.emplace_back(Vertex{ 1,	-1  ,0.0f  ,1.0f ,1.0f, 0 });

	postProcess.Create(postProcessData, postProcessData.size());

}

Game::~Game()
{
	delete[] squares;
}

void Game::Update()
{

	RECT winPos;
	GetClientRect(Window::GetHandle(), &winPos);
	POINT ClientAreaPos = { winPos.left, winPos.top };
	ClientToScreen(Window::GetHandle(), &ClientAreaPos);
	POINT MousePos = {};
	GetCursorPos(&MousePos);
	POINT ClientPos;
	ClientPos.x = MousePos.x - ClientAreaPos.x;
	ClientPos.y = MousePos.y - ClientAreaPos.y;

	float xP = (ClientPos.x - 420.0f * Window::m_Width / Window::screenWidth) / (135.0f * Window::m_Width / Window::screenWidth);
	float yP = 8.0f - ClientPos.y / (135.0f * Window::m_Height/Window::screenHeight);

	float xCur = xP;
	float yCur = yP;
	if (xP < 0 || xP >= 8)
		xP = -1;
	if (yP < 0 || yP >= 8)
		yP = -1;
	ImGui::Begin("Debug", 0, 0);
	ImGui::Value("x: ", xP);
	ImGui::Value("y: ", yP);
	ImGui::Value("fps: ", ImGui::GetIO().Framerate);
	ImGui::End();

	audio.Update3D(backgroundAudio, settings::backgroundChannel, { 0, yCur-4, xCur-4 });

	if (Window::mouse.LMouseDown())
	{
		int clicked = (int)xP + (7 - (int)yP) * 8;

		if (selected == -1)
		{
			if (xP > -1 && yP > -1 && squares[clicked].type > -1)
				selected = clicked;
		}
		else
		{
			if (CheckLegal(clicked))
			{
				renderer.SetPiece(selected, clicked);
				audio.Play3D(placeSound, settings::gameChannel, {0, yP-4, xP-4});
				selected = -1;
			}
			else
			{
				renderer.SetPiece(selected, selected);
				selected = -1;
			}
		}
	}
	if (selected > -1)
	{
		renderer.MovePiece(xCur, yCur, selected);
	}
}

bool Game::CheckLegal(int destination)
{
	if (destination > -1 && destination < 64)
	{
		return true;

		int n = destination - selected;

		for (size_t i = 0; i < 8; i++)
		{
			if (moves[0].moves[i] == n)
				return true;
		}
	}
	return false;
}

void Game::ResetGame()
{

}

void Game::Draw()
{
	renderer.SetTexture(0);
	
	renderTex.BindAsTarget();

	renderer.DrawBoard();
	renderer.DrawPieces();

	Graphics::SetRenderTarget();
	renderTex.BindAsTexture(0);
	postProcShader.Bind();
	
	postProcess.Bind();
	Graphics::GetDeviceContext()->DrawIndexed(6, 0, 0);
}