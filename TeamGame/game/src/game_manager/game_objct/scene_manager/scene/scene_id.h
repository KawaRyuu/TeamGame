/**
*	@file	scene_id.h
*	@brief	シーンID
*	@author	K.Kawakami
*/
#pragma once

enum class SCENE_ID
{
	DUMMY,
	LOAD,			// データロード
	TITLE,			// タイトル画面
	MODE,			// モードセレクト
	ENTRY,			// プレイヤーエントリー
	GAMEMAIN,		// ゲームメイン
	OPTION,			// 設定

	MAX,

};
