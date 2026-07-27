/**
*	@file	volume_slider_ui.h
*	@brief	音量変更スライダーUI
*	@author	K.Kawakami
*/
#pragma once

#include "vivid.h"
#include "../bar_ui.h"
#include "../../../../sound_manager/sound_id.h"

class CVolumeSliderUi : public CBarUi
{
public:
	//// 関数 //// 

	/**
	*	@brief		コンストラクタ
	*/
	CVolumeSliderUi();

	/**
	*	@brief		デストラクタ
	*/
	~CVolumeSliderUi() = default;

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
	*	@brief		バー割合の設定
	*
	*	@param[in]	割合(0.0f～1.0f)
	*/
	void SetBarRate(float rate) override;

	/**
	*	@brief		自身のサウンドの種類の設定
	* 
	*	@param[in]	sounds	サウンドの種類
	*/
	void SetSoundType(SOUND_TYPE sounds);

	/**
	*	@brief		自身の色の設定
	*
	*	@param[in]	color	色
	*/
	void SetSliderColor(unsigned int color);

	/**
	*	@brief		自身のスライダーアクティブ状態の設定
	*
	*	@param[in]	flag	アクティブフラグ（default = true）
	*/
	void SetSliderActive(bool flag = true);


private:
	//// 定数 ////

	static const std::string	m_bar_data;				// バー画像のファイル名
	static const std::string	m_bar_frame_data;		// バー枠画像のファイル名
	static const std::string	m_bar_point_data;		// バー点のファイル名
	static const std::string	m_bar_point_back_data;	// バー点枠のファイル名
	static const unsigned int	m_bar_point_color[2];	// バー点の色
	static const int			m_bar_point_size;		// バー点の画像サイズ


	//// 変数 ////

	SOUND_TYPE		m_SoundType;		// 自身のサウンドの種類
	bool			m_SliderActive;		// スライダーのアクティブフラグ
	unsigned int	m_BarPointColor;	// バー点の色
	vivid::Vector2	m_BarPointPos;		// バー点の描画位置

};