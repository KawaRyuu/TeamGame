/**
*	@file	game_manager.h
*	@brief	ゲーム全体のマネージャー
*	@author	K.Kawakami
*	@date	2026/01/08
*/
#pragma once

class CGameManager
{
public:
    //// 関数 //// 

	/**
	*	@brief		インスタンスの取得
	*
	*	@return		インスタンス
	*/
	static CGameManager& GetInstance();

	/**
	*	@brief		初期化
	*/
	void Initialize();

	/**
	*	@brief		更新
	*/
	void Update();

	/**
	*	@brief		描画
	*/
	void Draw();

	/**
	*	@brief		解放
	*/
	void Finalize();


private:
	//// 関数 //// 

	CGameManager() = default;								// コンストラクタ
	~CGameManager() = default;								// デストラクタ
	CGameManager(const CGameManager&) = delete;				// コピーコンストラクタ禁止
	CGameManager& operator=(const CGameManager&) = delete;	// コピー代入禁止
	CGameManager(CGameManager&&) = delete;					// ムーブコンストラクタ禁止
	CGameManager& operator=(CGameManager&&) = delete;		// ムーブ代入禁止

};

#define GAMES CGameManager::GetInstance()
