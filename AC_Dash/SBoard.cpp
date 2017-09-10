// include
#include <Siv3D.hpp>
#include "SceneMgr.h"
#include "SBoard.h"
#include "Game.h"

// define
#define MAX_CELL_NUM 5
#define MOVE_X_PER_SEC 300

struct cell_data
{
	String name;
	int64 score;
};

// グローバル変数
static std::vector<cell_data>data;
static Texture ac, wa;
static Font titleFont, cellFont, textFont, font1, font2;
static Triangle goUp({ 480,101 }, { 510,111 }, { 450,111 });
static Triangle goDown({ 450,415 }, { 510,415 }, { 480,425 });
static RoundRect button;
static String userName = L"";
static int64 nowTime;
static int drawCellBegin;
static double draw_ac_x, draw_wa_x;

// スコアボード 初期化
void SBoard_Init()
{
	if (data.size() == 0)
	{
		ac = Texture(L"data\\Menu\\ac.png");
		wa = Texture(L"data\\Menu\\wa.png");
		draw_ac_x = -ac.width;
		draw_wa_x = Window::Width();
		titleFont = Font(48);
		cellFont = Font(32);
		textFont = Font(24);
		font1 = Font(24);
		font2 = Font(18);
		button.w = textFont(L"メニューに戻る").region().w + textFont.height;
		button.h = textFont.height;
		button.x = Window::Width() / 2 - button.w / 2;
		button.y = Window::Height() - textFont.height - 5;
		button.r = 5;
		const CSVReader csv(L"data\\Sboard\\saveData.csv");
		for (int i = 0; i < (signed)csv.rows; ++i)
		{
			data.push_back({ csv.get<String>(i,0),csv.get<int64>(i,1) });
		}
	}
	if (getPrevScene() == Scene_Game) { SceneMgr_ChangeScene(Scene_Record); }
	if (getPrevScene() == Scene_Record)
	{
		userName.erase(userName.length - 1);
		CSVWriter csv(L"data\\SBoard\\saveData.csv");
		bool isOverWrite = false;
		for (auto& i : data)
		{
			if (i.name == userName)
			{
				i.score = Max(i.score, Game_getScore());
				isOverWrite = true;
				break;
			}
		}
		if (!isOverWrite) { data.push_back({ userName,Game_getScore() }); }
		std::sort(data.begin(), data.end(), [](const auto&l, const auto&r) {return l.score > r.score; });
		for (auto i : data) { csv.writeRow(i.name, i.score); }
	}
	drawCellBegin = 0;
	nowTime = Time::GetMillisec64();
}

// スコアボード 更新
void SBoard_Update()
{
	if (button.leftClicked) { SceneMgr_ChangeScene(Scene_Menu); }
	draw_ac_x = (draw_ac_x >= Window::Width() ? -ac.width : draw_ac_x + (double)MOVE_X_PER_SEC*(Time::GetMillisec64() - nowTime) / 1000);
	draw_wa_x = (draw_wa_x <= -wa.width ? Window::Width() : draw_wa_x - (double)MOVE_X_PER_SEC*(Time::GetMillisec64() - nowTime) / 1000);
	nowTime = Time::GetMillisec64();
	if (goUp.leftClicked) { --drawCellBegin; }
	if (goDown.leftClicked) { ++drawCellBegin; }
	drawCellBegin += Mouse::Wheel();
	drawCellBegin = Min<int>(drawCellBegin, (int)data.size() - MAX_CELL_NUM);
	drawCellBegin = Max(drawCellBegin, 0);
}

// スコアボード 描画
void SBoard_Draw()
{
	ac.draw(draw_ac_x, Window::Center().y - ac.height);
	wa.draw(draw_wa_x, Window::Center().y);
	titleFont(L"スコアボード").drawCenter(5);
	if (drawCellBegin > 0) { goUp.draw(goUp.mouseOver ? Palette::Orange : Palette::White); }
	if (drawCellBegin + MAX_CELL_NUM < (signed)data.size()) { goDown.draw(goDown.mouseOver ? Palette::Orange : Palette::White); }
	for (int i = 0; i < Min<int>(MAX_CELL_NUM, (int)data.size() - drawCellBegin); ++i)
	{
		auto num = i + drawCellBegin;
		auto text = Format(num + 1, L"位　", data[num].name, L"　", data[num].score, L"点");
		Color color;
		switch (num)
		{
		case 0:
			color = Palette::Gold;
			break;
		case 1:
			color = Palette::Silver;
			break;
		case 2:
			color = Color(196, 112, 34);
			break;
		default:
			color = Palette::White;
			break;
		}
		if (data[num].name == userName) { color = Palette::Orange; }
		cellFont(text).drawCenter(5 + titleFont.height + i * cellFont.height, color);
	}
	button.draw(button.mouseOver ? Palette::Orange : Palette::White);
	textFont(L"メニューに戻る").drawCenter(Window::Height() - textFont.height - 5, Palette::Black);
}

// スコアボード 記録
void SBoard_Record()
{
	font1(L"スコアボードに記録する名前を入力してください！").drawCenter(25);
	Input::GetCharsHelper(userName);
	if (userName.length > 10 && userName[userName.length - 1] != L'\n') { userName.erase(10, userName.length - 1); }
	font2(userName).draw(25, 25 + font1.height);
	if (userName.length > 0 && userName[userName.length - 1] == L'\n') { SceneMgr_ChangeScene(Scene_SBoard); }
}