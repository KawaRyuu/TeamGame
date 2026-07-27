/**
*	@file	number_ui.h
*	@brief	数字UIのベースクラス
*	@author	K.Kawakami
*/
#pragma once

#include "vivid.h"
#include "../ui.h"

class CNumberUi : public IUi
{
public:
	//// 関数 //// 

	/**
	*	@brief		コンストラクタ
	*/
	CNumberUi();

	/**
	*	@brief		デストラクタ
	*/
	~CNumberUi() = default;

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
	*	@brief		初期化
	*
	*	@param[in]	pos			位置
	*	@param[in]	id			生成したUIのID
	*	@param[in]	ui_draw		UIを描画する位置（手前か奥か）
	*	@param[in]	size		1文字当たりの縦幅
	*	@param[in]	digit		表示桁数
	*	@param[in]	playre_id	UIを生成したプレイヤーのID（誰のUIか）
	*/
	virtual void Initialize(const vivid::Vector2& pos, UI_ID id, UI_DRAW_POS ui_draw, int size, int digit, contol::PLAYER_ID player_id);

	/**
	*	@brief		更新
	*/
	void Update() override;

	/**
	*	@brief		描画
	*/
	void Draw() override;


protected:
	//// 定数 ////


	//// 変数 ////

	int		m_Value;				// 数値
	int		m_MaxValue;				// 最大値
	int		m_Digit;				// 桁数
	int		m_OneDigitWidth;		// 数字一桁当たりの横幅（画像）
	int		m_DrawOneDigitWidth;	// 数字一桁当たりの横幅（表示上）
	int		m_Size;					// 表示する数値の縦幅
	float	m_DefaultScale;			// UIのデフォルト拡縮率
};