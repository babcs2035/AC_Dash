// include
#include <Siv3D.hpp>
#include "SceneMgr.h"

void Main()
{
	Window::SetTitle(L"AC Dash v1.0");
	Window::Resize({ 960,480 }, true);
	Window::Centering();

	SceneMgr_ChangeScene(Scene_Menu);
	while (System::Update())
	{
		SceneMgr_Update();
		SceneMgr_Draw();
	}
}