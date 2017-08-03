// include
#include <Siv3D.hpp>
#include "SceneMgr.h"

void Main()
{
	Window::SetStyle(WindowStyle::NonFrame);
	Window::Resize({ 860,480 }, true);
	Window::SetTitle(L"AC Dash v1.0");
	Window::Centering();

	SceneMgr_ChangeScene(Scene_Menu);
	while (System::Update())
	{
		SceneMgr_Update();
		SceneMgr_Draw();
	}
}