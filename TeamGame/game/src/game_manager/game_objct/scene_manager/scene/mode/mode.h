/**
*	@file	mode.h
*	@brief	モード選択シーン
*	@author	K.Kawakami
*/
#pragma once

#include "../scene.h"
#include "../../../ui_manager/ui_manager.h"

class CMode : public IScene
{
public:
	//// 関数 ////

	/**
	*	@brief		コンストラクタ
	*/
	CMode();

	/**
	*	@brief		デストラクタ
	*/
	~CMode() = default;

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

	//// ID ////

	/**
	*	@brief		モードセレクトシーンの状態
	*/
	enum class MODE_STATE
	{
		MAIN,	// メインメニュー
		SOLO,	// ひとりで
		EVERY,	// みんなで
	};

	
	//// 関数 ////

	/**
	*	@brief		戻る入力時の処理
	*/
	void InputBackKey();

	/**
	*	@brief		メインメニューボタン群の生成
	*/
	void CreateMainMenuButton();

	/**
	*	@brief		「ひとりで」メニューボタン群の生成
	*/
	void CreateSoloMenuButton();

	/**
	*	@brief		「みんなで」メニューボタン群の生成
	*/
	void CreateEveryMenuButton();

	/**
	*	@brief		プレイヤー数変更
	*/
	void ChangePlayerCount();


	//// 変数 ////

	MODE_STATE	m_ModeState;
	bool		m_ToMainMenu;
	bool		m_ToSoloMenu;
	bool		m_ToEveryMenu;
	CButtonUi*	m_BackButton;
	int			m_PlayerCount;
};
