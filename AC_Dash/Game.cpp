// include
#include <Siv3D.hpp>
#include "SceneMgr.h"
#include "Game.h"

// define
#define CHANGE_SPEED_DISTANCE 3500
#define ITEM_KIND_NUM 5
#define DRAW_STATS_CHANGED_LENGTH 1500

// グローバル変数
static Texture main, ground, item[ITEM_KIND_NUM];
static Font statsFont, font;
static Sound bgm;
static int64 startTime, nowTime;
static int64 score, life;
static int64 draw_stats_startTime, draw_stats_Time, draw_Message_startTime, draw_Message_Time;
static String statsChanged = L"", statsMessage = L"";
static int draw_ground_x1, draw_ground_x2, draw_speed;
static int draw_item_num, draw_item_x;
static bool draw_item_flag, first_flag = true;

// ゲーム 初期化
void Game_Init()
{
	// 背景 初期化
	{
		if (!main)
		{
			main = Texture(L"data\\Game\\main.png");
			ground = Texture(L"data\\Game\\ground.png");
			bgm = Sound(L"data\\Game\\bgm.wav");
		}
		draw_ground_x1 = 0;
		draw_ground_x2 = Window::Width();
		draw_speed = 5;
	}

	// アイテム 初期化
	{
		if (!item[0])
		{
			item[0] = Texture(L"data\\Game\\ac.png");
			item[1] = Texture(L"data\\Game\\wa.png");
			item[2] = Texture(L"data\\Game\\wj.png");
			item[3] = Texture(L"data\\Game\\tle.png");
			item[4] = Texture(L"data\\Game\\re.png");
		}
		draw_item_flag = false;
	}

	// ステータス 初期化
	{
		FontManager::Register(L"data\\Game\\scoreboard.ttf");
		statsFont = Font(32, L"Score Board");
		score = 0; life = 5;
	}
	if (first_flag) { Game_Expl(); }
	startTime = Time::GetMillisec64();
	bgm.setLoop(true);
	bgm.play();
}

// ゲーム 更新
void Game_Update()
{
	// 背景 更新
	{
		nowTime = Time::GetMillisec64();
		if (nowTime - startTime > CHANGE_SPEED_DISTANCE)
		{
			startTime = nowTime;
			++draw_speed;
			statsMessage = L"SPEED UP!";
			draw_Message_Time = draw_Message_startTime = Time::GetMillisec64();
		}
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
					score -= 100;
					--life;
					statsChanged = L"-100\n-1";
					draw_stats_Time = draw_stats_startTime = Time::GetMillisec64();
				}
				draw_item_flag = false;
			}
			draw_item_x -= draw_speed;
		}
		if (!draw_item_flag)
		{
			if (RandomBool((double)1 / (double)3)) { draw_item_num = 0; }
			else { draw_item_num = Random(ITEM_KIND_NUM - 1); }
			draw_item_x = Window::Width();
			draw_item_flag = true;
		}
	}

	// ステータス 更新
	{
		draw_stats_Time = draw_Message_Time = Time::GetMillisec64();
		if (draw_item_flag)
		{
			const Rect tmpRect(draw_item_x, 240, item[draw_item_num].width, item[draw_item_num].height);
			if (tmpRect.leftClicked)
			{
				switch (draw_item_num)
				{
				case 0:
					score += 250;
					++life;
					statsChanged = L"+250\n+1";
					draw_stats_Time = draw_stats_startTime = Time::GetMillisec64();
					break;

				case 1:
					score -= 180;
					--life;
					statsChanged = L"-180\n-1";
					draw_stats_Time = draw_stats_startTime = Time::GetMillisec64();
					break;

				case 2:
					--draw_speed;
					statsMessage = L"SPEED DOWN!";
					draw_Message_Time = draw_Message_startTime = Time::GetMillisec64();
					break;

				case 3:
					++draw_speed;
					statsMessage = L"SPEED UP!";
					draw_Message_Time = draw_Message_startTime = Time::GetMillisec64();
					break;

				case 4:
					score -= 100;
					--life;
					statsChanged = L"-100\n-1";
					draw_stats_Time = draw_stats_startTime = Time::GetMillisec64();
					break;
				}
				draw_item_flag = false;
			}
		}
		score += draw_speed;
		if (life < 1)
		{
			bgm.setVolume(0.5);
			bgm.setLoop(false);
			Game_End();
			bgm.stop();
			SceneMgr_ChangeScene(Scene_SBoard);
		}
		if (score < 0) { score = 0; }
		if (life > 15)
		{
			life = 15;
			statsChanged[statsChanged.length - 2] = L'±';
			statsChanged[statsChanged.length - 1] = L'0';
		}
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
		if (draw_item_flag)
		{
			item[draw_item_num].draw(draw_item_x, 0);
			const Rect tmpRect(draw_item_x, 240, item[draw_item_num].width, 75);
			if (tmpRect.mouseOver) { tmpRect.drawFrame(2, 3, Palette::Yellow); }
		}
	}

	// ステータス 描画
	{
		const auto flag = draw_stats_Time - draw_stats_startTime <= DRAW_STATS_CHANGED_LENGTH;
		const auto display = Format(score, L"\n", life);
		int draw_x = (1 + (int)Log10(score)) * 30;
		statsFont(L"SCORE:\nLIFE :").draw(10, 10);
		statsFont(display).draw(192, 10, Palette::White);
		if (flag) { statsFont(statsChanged).draw(draw_x + 210, 10, Palette::Orange); }
		if (draw_Message_Time - draw_Message_startTime <= DRAW_STATS_CHANGED_LENGTH) { statsFont(statsMessage).drawCenter(10 + statsFont.height * 2, Palette::Orange); }
	}
}

// ゲーム説明 描画
void Game_Expl()
{
	Texture expl(L"data\\Game\\explain1.png");
	expl.draw();
	WaitKey();
	expl = Texture(L"data\\Game\\explain2.png");
	expl.draw();
	WaitKey();
	first_flag = false;
}

// ゲーム終了 描画
void Game_End()
{
	const Rect rect(0, 0, Window::Width(), Window::Height());
	const Font font1(48);
	const Font font2(36);
	const Font font3(24);
	const String text = Format(L"スコアは ", score, L" です！");
	while (!Input::AnyKeyClicked())
	{
		System::Update();
		main.draw(); ground.draw();
		item[Random(ITEM_KIND_NUM - 1)].drawAt(RandomVec2(Window::Width(), Window::Height()));
		rect.draw(Color(64, 64, 64, 200));
		font1(L"ゲームオーバー").drawCenter(25, Palette::Red);
		font2(text).drawCenter(150, Palette::Yellow);
		font3(L"何かキーを押してください...").drawCenter(250);
		item[1].drawAt(Window::Width() / 2, 350);
	}
}

// ゲーム スコア取得
int64 Game_getScore()
{
	return score;
}