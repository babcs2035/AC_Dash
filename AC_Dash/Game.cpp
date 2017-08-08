// include
#include <Siv3D.hpp>
#include "SceneMgr.h"
#include "Game.h"

// define
#define MAKE_ITEM_DISTANCE 3250
#define ITEM_KIND_NUM 2

// グローバル変数
static Texture main, ground, item[ITEM_KIND_NUM];
static Font statsFont;
static int64 score, life;
static int draw_ground_x1, draw_ground_x2, draw_speed;
static int draw_item_num, draw_item_x;
static bool draw_item_flag;

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

	// アイテム 初期化
	{
		draw_item_flag = false;
	}

	// ステータス 初期化
	{
		if (!item[0])
		{
			item[0] = Texture(L"data\\Game\\ac.png");
			item[1] = Texture(L"data\\Game\\wa.png");
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

	// アイテム 更新
	{
		if (draw_item_flag)
		{
			if (draw_item_x < -item[draw_item_num].width)
			{
				if (draw_item_num == 0)
				{
					score -= 1800;
					--life;
				}
				draw_item_flag = false;
			}
			draw_item_x -= draw_speed;
		}
		if (!draw_item_flag)
		{
			draw_item_num = Random(ITEM_KIND_NUM - 1);
			draw_item_x = Window::Width();
			draw_item_flag = true;
		}
	}

	// ステータス 更新
	{
		if (draw_item_flag)
		{
			const Rect tmpRect(draw_item_x, 240, item[draw_item_num].width, item[draw_item_num].height);
			if (tmpRect.leftClicked)
			{
				switch (draw_item_num)
				{
				case 0:
					score += 2500;
					++life;
					break;

				case 1:
					score -= 1800;
					--life;
					break;
				}
				draw_item_flag = false;
			}
		}
		++score;
		if (life < 1) { SceneMgr_ChangeScene(Scene_Result); }
		if (score < 0) { score = 0; }
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

	// アイテム 描画
	{
		if (draw_item_flag) { item[draw_item_num].draw(draw_item_x, 0); }
	}

	// ステータス 描画
	{
		auto display = Format(L"SCORE:", score, L"\nLIFE :", life);
		statsFont(display).draw(10, 10);
	}
}