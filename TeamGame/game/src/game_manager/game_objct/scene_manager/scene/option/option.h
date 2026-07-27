/**
*	@file	option.h
*	@brief	オプションシーン
*	@author	K.Kawakami
*/
#pragma once

#include "../scene.h"
#include "../../../sound_manager/sound_id.h"
#include "../../../ui_manager/ui/bar_ui/bar_ui.h"

class COption : public IScene
{
public:
	//// 関数 ////

	/**
	*	@brief		コンストラクタ
	*/
	COption();

	/**
	*	@brief		デストラクタ
	*/
	~COption() = default;

	/**
	*	@brief		初期化
	*/
	void Initialize() override;

	/**
	*	@brief		更新
	*/
	void Update() override;

	/**
	*	@brief		描画
	*/
	void Draw() override;

	/**
	*	@brief		解放
	*/
	void Finalize() override;


private:
	//// 関数 ////

	/**
	*	@brief		音量の変更
	*/
	void ChangeVolume();


	//// 定数 ////

	static const int	m_input_time;		// 長押し判定の間隔


	//// 変数 ////

	SOUND_TYPE	m_VolumeChange;						// 音量を変更するサウンドの種類
	int			m_Volume[(int)SOUND_TYPE::MAX];		// 音量
	int			m_InputTimer;						// 長押しタイマー
	CBarUi*		m_VolumeBar[(int)SOUND_TYPE::MAX];	// BGM/SEの音量バー

};
