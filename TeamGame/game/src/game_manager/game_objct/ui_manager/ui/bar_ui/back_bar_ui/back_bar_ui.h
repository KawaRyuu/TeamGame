/**
*	@file	back_bar_ui.h
*	@brief	長押しで戻るUI
*	@author	K.Kawakami
*/
#pragma once

#include "vivid.h"
#include "../bar_ui.h"

class CBackBarUi : public CBarUi
{
public:
	//// 関数 //// 

	/**
	*	@brief		コンストラクタ
	*/
	CBackBarUi();

	/**
	*	@brief		デストラクタ
	*/
	~CBackBarUi() = default;

	/**
	*	@brief		初期化
	*
	*	@param[in]	pos			位置
	*	@param[in]	id			生成したUIのID
	*	@param[in]	ui_draw		UIを描画する位置（手前か奥か）
	*	@param[in]	playre_id	UIを生成したプレイヤーのID（誰のUIか）
	*/
	void Initialize(const vivid::Vector2& pos, UI_ID id, UI_DRAW_POS ui_draw, contol::PLAYER_ID player_id) override;

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

	static const std::string	m_bar_data;			// バー画像のファイル名
	static const std::string	m_bar_frame_data;	// バー枠画像のファイル名


	//// 変数 ////


};