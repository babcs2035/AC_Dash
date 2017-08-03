// include
#include <Siv3D.hpp>
#include "SceneMgr.h"
#include "Menu.h"

// ƒOƒ[ƒoƒ‹•Ï”
static Texture ac, wa;
static int32 draw_ac_x, draw_wa_x;

// ƒƒjƒ…[ ‰Šú‰»
void Menu_Init()
{
	// ”wŒi ‰Šú‰»
	{
		ac = Texture(L"data\\Menu\\ac.png");
		wa = Texture(L"data\\Menu\\wa.png");
		draw_ac_x = -ac.width;
		draw_wa_x = Window::Width() + wa.width;
	}
}

// ƒƒjƒ…[  XV
void Menu_Update()
{
	// ”wŒi XV
	{
		draw_ac_x = (draw_ac_x >= Window::Width() ? -ac.width : draw_ac_x + 3);
		draw_wa_x = (draw_wa_x < -wa.width ? Window::Width() + wa.width : draw_wa_x - 2);
	}
}

// ƒƒjƒ…[  •`‰æ
void Menu_Draw()
{
	// ”wŒi •`‰æ
	{
		ac.draw(draw_ac_x, Window::Center().y - ac.height);
		wa.draw(draw_wa_x, Window::Center().y);
	}
}