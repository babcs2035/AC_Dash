// include
#include <Siv3D.hpp>
#include "SceneMgr.h"
#include "Game.h"

// グローバル変数
static Texture main, ground, ac, wa;
static Font statsFont;
static int draw_ground_x1, draw_ground_x2, draw_speed;
static int64 score, life;

// ゲーム 初期化
void Game_Init()
{
	// 背景 初期化
	{
		if (!main)
		{
			main = Texture(L"data\\Game\\main.png");
			ground = Texture(L"data\\Game\\ground.png");
		}
		draw_ground_x1 = 0;
		draw_ground_x2 = Window::Width();
		draw_speed = 5;
	}

	// ステータス初期化
	{
		if (!ac)
		{
			ac = Texture(L"data\\Game\\ac.png");
			wa = Texture(L"data\\Game\\wa.png");
			FontManager::Register(L"data\\Game\\scoreboard.ttf");
			statsFont = Font(32, L"Score Board");
			score = 0; life = 5;
		}
	}
}

// ゲーム 更新
void Game_Update()
{
	// 背景 更新
	{
		draw_ground_x1 = (draw_ground_x1 <= -Window::Width() ? Window::Width() : draw_ground_x1 - draw_speed);
		draw_ground_x2 = (draw_ground_x2 <= -Window::Width() ? Window::Width() : draw_ground_x2 - draw_speed);
	}

	// ステータス 更新
	{
		++score;
	}
}

// ゲーム 描画
void Game_Draw()
{
	// 背景 描画
	{
		main.draw();
		ground.draw(draw_ground_x1, 0);
		ground.draw(draw_ground_x2, 0);
	}

	// ステータス 描画
	{
		auto display = Format(L"SCORE:", score, L"\nLIFE :", life);
		statsFont(display).draw(10, 10);
	}
}