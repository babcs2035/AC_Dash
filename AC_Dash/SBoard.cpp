// include
#include <Siv3D.hpp>
#include "SceneMgr.h"
#include "SBoard.h"
#include "Game.h"

// define
#define MAX_CELL_NUM 5

struct cell_data
{
	String name;
	int64 score;
};

// グローバル変数
static std::vector<cell_data>data;
static Font titleFont, cellFont;
static int drawCellBegin;

// スコアボード 初期化
void SBoard_Init()
{
	titleFont = Font(56);
	cellFont = Font(32);
	drawCellBegin = 0;
	if (getPrevScene() == Scene_Game) { SBoard_Record(); }
}

// スコアボード 更新
void SBoard_Update()
{
	if (Input::KeyB.clicked) { SceneMgr_ChangeScene(Scene_Menu); }
	drawCellBegin += Mouse::Wheel();
	drawCellBegin = Min<int>(drawCellBegin, data.size() - MAX_CELL_NUM);
	drawCellBegin = Max(drawCellBegin, 0);
}

// スコアボード 描画
void SBoard_Draw()
{
	titleFont(L"スコアボード").drawCenter(10);
	for (int i = 0; i < Min<int>(MAX_CELL_NUM, data.size() - drawCellBegin); ++i)
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
			color = Palette::Brown;
			break;
		default:
			color = Palette::White;
			break;
		}
		cellFont(text).drawCenter(10 + titleFont.height + i * cellFont.height, color);
	}
}

// スコアボード 記録
void SBoard_Record()
{
	const Font font1(24), font2(18);
	String text;
	while (System::Update())
	{
		if (text.length > 0 && text[text.length - 1] == L'\n') { break; }
		font1(L"スコアボードに記録する名前を入力してください！").drawCenter(25);
		Input::GetCharsHelper(text);
		font2(text).draw(25, 25 + font1.height);
	}
	text[text.length - 1] = L'\0';
	data.push_back({ text,Game_getScore() });
	std::sort(data.begin(), data.end(), [](const auto&l, const auto&r) {return l.score > r.score; });
}