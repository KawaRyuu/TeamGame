/**
*	@file	entry.h
*	@brief	プレイヤーエントリーシーン
*	@author	K.Kawakami
*/
#pragma once

#include "../scene.h"
#include "../../../ui_manager/ui/bar_ui/bar_ui.h"

class CEntry : public IScene
{
public:
	//// 関数 ////

	/**
	*	@brief		コンストラクタ
	*/
	CEntry();

	/**
	*	@brief		デストラクタ
	*/
	~CEntry() = default;

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


	//// 定数 ////

	static const int	m__back_mode_scene_time;	// モード選択シーンへ戻るボタン長押しの時間


	//// 変数 ////

	int		m_BackModeSceneTimer;		// モード選択シーンへ戻るボタン長押しのタイマー
	CBarUi* m_BackBarUi;				// 長押しで戻るUI

};
