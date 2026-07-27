/**
*	@file	effect_id.h
*	@brief	UIのID
*	@author	K.Kawakami
*/

#pragma once

enum class UI_ID
{
	// 通常UI
	ALPHANUMERIC_UI,	// 英数字UI

	// 数字UI
	SCORE_UI,			// フィールドスコアUI
	RECORD_SCORE_UI,	// ランキング及び記録閲覧時のスコアUI

	// ボタンUI
	TEST_BUTTON_UI,		// ボタンUIのテスト
	TEST_BUTTON_UI2,	// ボタンUIのテスト2

	// バーUI
	LOAD_PROGRESS_UI,	// ロードシーンのプログレスバーUI
	BACK_BAR_UI,		// 長押しで戻るUI
	VOLUME_SLIDER_UI,	// 音量変更スライダーUI

	DUMMY,
};

/**
*	@brief	UIの描画位置
*/
enum class UI_DRAW_POS
{
	FRONT,		// 一番手前
	MIDDLE,		// 中間
	BACK,		// 後ろ

	DUMMY,
};

/**
*	@brief		UIカテゴリー
*/
enum class UI_CATEGORY
{
	NUMBER,		// 数字UI
	BUTTON,		// ボタンUI
	OTHER,		// その他UI
};
