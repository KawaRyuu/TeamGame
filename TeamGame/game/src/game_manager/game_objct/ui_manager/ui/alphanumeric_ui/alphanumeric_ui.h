/**
*	@file	alphanumeric_ui.h
*	@brief	英数字UI
*	@author	K.Kawakami
*/
#pragma once

#include "vivid.h"
#include "../ui.h"

class CAlphanumericUi : public IUi
{
public:
	//// 関数 //// 

	/**
	*	@brief		コンストラクタ
	*/
	CAlphanumericUi();

	/**
	*	@brief		デストラクタ
	*/
	~CAlphanumericUi() = default;

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

	/**
	*	@brief		英数字番号の設定
	* 
	*	@param[in]	num	英数字の文字番号
	*/
	void SetAlphanumericNum(int num);

	/**
	*	@brief		フォントサイズの設定
	*
	*	@param[in]	size	文字の縦幅	
	*/
	void SetFontSize(int size);

	/**
	*	@brief		描画位置の取得
	*
	*	@return		描画位置（左上の座標）を返す
	*/
	inline vivid::Vector2 GetPosition() const { return m_Position; }


private:
	//// 関数 ////


	//// 定数 ////


	//// 変数 ////

	int		m_AlphanumericNum;	// 英数字番号

};