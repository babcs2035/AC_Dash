#pragma once

// シーン構造体
typedef enum
{
	Scene_Menu,
	Scene_Game,
	Scene_SBoard,
}Scene_S;

// 更新
void SceneMgr_Update();

// 描画
void SceneMgr_Draw();

// シーン変更
void SceneMgr_ChangeScene(Scene_S);

// 前のシーンを得る
Scene_S getPrevScene();