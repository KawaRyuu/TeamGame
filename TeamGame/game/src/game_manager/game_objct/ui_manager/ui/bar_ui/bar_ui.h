/**
*	@file	bar_ui.h
*	@brief	バーUIのベースクラス
*	@author	K.Kawakami
*/
#pragma once

#include "vivid.h"
#include "../ui.h"

class CBarUi : public IUi
{
public:
	//// 関数 //// 

	/**
	*	@brief		コンストラクタ
	*/
	CBarUi(int width, int height, std::string data);
	CBarUi() = delete;

	/**
	*	@brief		デストラクタ
	*/
	~CBarUi() = default;

	/**
	*	@brief		初期化
	*
	*	@param[in]	pos			位置
	*	@param[in]	id			生成したUIのID
	*	@param[in]	ui_draw		UIを描画する位置（手前か奥か）
	*	@param[in]	playre_id	UIを生成したプレイヤーのID（誰のUIか）
	*/
	virtual void Initialize(const vivid::Vector2& pos, UI_ID id, UI_DRAW_POS ui_draw, contol::PLAYER_ID player_id) override;

	/**
	*	@brief		更新
	*/
	void Update() override;

	/**
	*	@brief		描画
	*/
	void Draw() override;

	/**
	*	@brief		バー割合の設定
	* 
	*	@param[in]	割合(0.0f～1.0f)
	*/
	virtual void SetBarRate(float rate);


protected:
	//// 関数 ////


	//// 変数 ////

	float	m_Rate;		// バーの割合

};