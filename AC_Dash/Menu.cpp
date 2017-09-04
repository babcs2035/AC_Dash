// include
#include <Siv3D.hpp>
#include "SceneMgr.h"
#include "Menu.h"

// define
#define DRAW_AC_WA_SPEED 3

// グローバル変数
static Texture ac, wa;
static Sound bgm;
static Font titleFont, choiceFont;
static int32 draw_ac_x, draw_wa_x;
static Circle SBoardCircle, playCircle, exitCircle;

// メニュー 初期化
void Menu_Init()
{
	// 背景 初期化
	{
		if (!ac)
		{
			ac = Texture(L"data\\Menu\\ac.png");
			wa = Texture(L"data\\Menu\\wa.png");
			bgm = Sound(L"data\\Menu\\bgm.ogg");
		}
		draw_ac_x = -ac.width;
		draw_wa_x = Window::Width();
	}

	// 選択肢 初期化
	{
		if (!titleFont)
		{
			titleFont = Font(64);
			choiceFont = Font(24);
			SBoardCircle = playCircle = exitCircle = Circle(128);
			SBoardCircle.x = Window::Center().x - SBoardCircle.r * 2 - 25;
			playCircle.x = Window::Center().x;
			exitCircle.x = Window::Center().x + exitCircle.r * 2 + 25;
			SBoardCircle.y = playCircle.y = exitCircle.y = 25 * 2 + titleFont.height + exitCircle.r;
		}
	}

	bgm.setLoop(true);
	bgm.play();
}

// メニュー  更新
void Menu_Update()
{
	// 背景 更新
	{
		draw_ac_x = (draw_ac_x >= Window::Width() ? -ac.width : draw_ac_x + DRAW_AC_WA_SPEED);
		draw_wa_x = (draw_wa_x <= -wa.width ? Window::Width() : draw_wa_x - DRAW_AC_WA_SPEED);
	}

	// 選択肢 更新
	{
		if (SBoardCircle.leftClicked)
		{
			bgm.stop();
			SceneMgr_ChangeScene(Scene_SBoard);
		}
		if (playCircle.leftClicked)
		{
			bgm.stop();
			SceneMgr_ChangeScene(Scene_Game);
		}
		if (exitCircle.leftClicked) { System::Exit(); }
	}
}

// メニュー  描画
void Menu_Draw()
{
	// 背景 描画
	{
		ac.draw(draw_ac_x, Window::Center().y - ac.height);
		wa.draw(draw_wa_x, Window::Center().y);
	}

	// 選択肢 描画
	{
		titleFont(L"AC Dash v1.0").drawCenter(25);
		SBoardCircle.drawShadow({ 0, 8 }, 28, 6);
		SBoardCircle.draw(Palette::Gold);
		if (SBoardCircle.mouseOver) { SBoardCircle.drawFrame(5, 5, Palette::Red); }
		choiceFont(L"スコアボード\n　 を見る").drawCenter(SBoardCircle.x, SBoardCircle.y, Palette::Black);
		playCircle.drawShadow({ 0, 8 }, 28, 6);
		playCircle.draw(Palette::Lightskyblue);
		if (playCircle.mouseOver) { playCircle.drawFrame(5, 5, Palette::Red); }
		choiceFont(L"ゲーム開始！").drawCenter(playCircle.x, playCircle.y, Palette::Black);
		exitCircle.drawShadow({ 0, 8 }, 28, 6);
		exitCircle.draw(Palette::Orange);
		if (exitCircle.mouseOver) { exitCircle.drawFrame(5, 5, Palette::Red); }
		choiceFont(L"終了する").drawCenter(exitCircle.x, exitCircle.y, Palette::Black);
	}
}