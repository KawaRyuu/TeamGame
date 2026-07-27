/**
*	@file	game_main.h
*	@brief	ゲームメインシーン
*	@author	K.Kawakami
*/
#pragma once

#include "../scene.h"

class CGameMain : public IScene
{
public:
	//// 関数 ////

	/**
	*	@brief		コンストラクタ
	*/
	CGameMain();

	/**
	*	@brief		デストラクタ
	*/
	~CGameMain() = default;

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

	static const int	m_view_winner_time;		// 勝者UI表示時間


	//// 変数 ////

	bool	m_GameSetFlag;			// ゲーム終了フラグ
	int		m_ViewWinnerTimer;		// 勝者UI表示タイマー

};
