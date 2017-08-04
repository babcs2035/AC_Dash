// include
#include <Siv3D.hpp>
#include "SceneMgr.h"
#include "Game.h"

// ƒOƒ[ƒoƒ‹•Ï”
static Texture main, ground;
static int draw_ground_x1, draw_ground_x2, draw_speed;

// ƒQ[ƒ€ ‰Šú‰»
void Game_Init()
{
	// ”wŒi ‰Šú‰»
	{
		main = Texture(L"data\\Game\\main.png");
		ground = Texture(L"data\\Game\\ground.png");
		draw_ground_x1 = 0;
		draw_ground_x2 = Window::Width();
		draw_speed = 5;
	}
}

// ƒQ[ƒ€ XV
void Game_Update()
{
	// ”wŒi XV
	{
		draw_ground_x1 = (draw_ground_x1 <= -Window::Width() ? Window::Width() : draw_ground_x1 - draw_speed);
		draw_ground_x2 = (draw_ground_x2 <= -Window::Width() ? Window::Width() : draw_ground_x2 - draw_speed);
	}
}

// ƒQ[ƒ€ •`‰æ
void Game_Draw()
{
	// ”wŒi •`‰æ
	{
		main.draw();
		ground.draw(draw_ground_x1, 0);
		ground.draw(draw_ground_x2, 0);
	}
}