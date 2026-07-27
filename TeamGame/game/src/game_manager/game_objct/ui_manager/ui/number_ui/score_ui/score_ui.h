/**
*	@file	score_ui.h
*	@brief	フィールドのスコアUI
*	@author	K.Kawakami
*/
#pragma once

#include "vivid.h"
#include "../number_ui.h"

class CScoreUi : public CNumberUi
{
public:
	//// 関数 //// 

	/**
	*	@brief		コンストラクタ
	*/
	CScoreUi();

	/**
	*	@brief		デストラクタ
	*/
	~CScoreUi() = default;

	/**
	*	@brief		初期化
	*
	*	@param[in]	pos			位置
	*	@param[in]	id			生成したUIのID
	*	@param[in]	ui_draw		UIを描画する位置（手前か奥か）
	*	@param[in]	size		1文字当たりの縦幅
	*	@param[in]	digit		表示桁数
	*	@param[in]	playre_id	UIを生成したプレイヤーのID（誰のUIか）
	*/
	void Initialize(const vivid::Vector2& pos, UI_ID id, UI_DRAW_POS ui_draw, int size, int digit, contol::PLAYER_ID player_id) override;

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

	int		m_OldScore;			// 1F前のスコア
	int		m_ScoreDigit;		// スコアの桁数
	bool	m_ExpandFlag;		// 文字拡大フラグ
};