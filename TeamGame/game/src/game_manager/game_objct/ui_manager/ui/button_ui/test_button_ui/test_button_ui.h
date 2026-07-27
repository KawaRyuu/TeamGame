/**
*	@file	test_button_ui.h
*	@brief	ボタンUIのテスト
*	@author	K.Kawakami
*/
#pragma once

#include "vivid.h"
#include "../button_ui.h"

class CTestButtonUi : public CButtonUi
{
public:
	//// 関数 //// 

	/**
	*	@brief		コンストラクタ
	*/
	CTestButtonUi();

	/**
	*	@brief		デストラクタ
	*/
	~CTestButtonUi() = default;

	/**
	*	@brief		初期化
	*
	*	@param[in]	pos			位置
	*	@param[in]	id			生成したUIのID
	*	@param[in]	width		横幅
	*	@param[in]	height		縦幅
	*	@param[in]	image_data	表示画像名
	*	@param[in]	image_scale	表示画像のデフォルトサイズ
	*	@param[in]	color		色
	*	@param[in]	ui_draw		UIを描画する位置（手前か奥か）
	*	@param[in]	playre_id	UIを生成したプレイヤーのID（誰のUIか）
	*	@param[in]	on_click	決定ボタンを押した時の処理
	*/
	void Initialize(const vivid::Vector2& pos, UI_ID id, int width, int height,
		std::string image_data, float image_scale, unsigned int color, UI_DRAW_POS ui_draw, contol::PLAYER_ID player_id,
		std::function<void()> on_click) override;

	/**
	*	@brief		更新
	*/
	void Update() override;

	/**
	*	@brief		描画
	*/
	void Draw() override;


private:
	//// 定数 ////


	//// 変数 ////


};