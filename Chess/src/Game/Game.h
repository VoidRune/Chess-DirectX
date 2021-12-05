#pragma once
#include "Components/Renderer.h"
#include "Components/Audio.h"
#include "Graphics/RenderTarget.h"
#include "Graphics/Text.h"
#include "Settings.h"

struct PossibleMoves
{
	bool sliding = false;
	int moves[8] = {0, 0, 0, 0, 0, 0, 0, 0};
};

class Game
{
public:
	Game();
	~Game();
	void ResetGame();
	void Update();
	void Draw();
private:
	bool CheckLegal(int destination);

	RenderTarget renderTex;
	Shader postProcShader;

	ChessPiece* squares;
	int selected = -1;

	std::vector<PossibleMoves> moves;

	Renderer renderer;

	VertexBuffer postProcess;

	AudioEngine audio;
	Sound placeSound;
	Sound backgroundAudio;
};