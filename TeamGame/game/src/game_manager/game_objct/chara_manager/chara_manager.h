/**
*	@file	chara_manager.h
*	@brief	生成するキャラクターのマネージャー
*	@author	Ryuuki.Kawakami
*/

#pragma once

#include "vivid.h"
#include "chara/chara_id.h"
#include <list>
#include <map>
#include <functional>

class IChara;

class CCharaManager
{
public:
	/**
	*	@brief		インスタンスの取得
	*
	*	@return		インスタンス
	*/
	static CCharaManager& GetInstance();

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

	/**
	*	@brief		キャラクターの生成
	*
	*	@param[in]	id		キャラクターID
	*	@param[in]	pos		位置
	*/
	void Create(CHARA_ID id, const vivid::Vector2& pos);


private:
	//// 関数 ////

	CCharaManager() = default;									// コンストラクタ
	~CCharaManager() = default;									// デストラクタ
	CCharaManager(const CCharaManager&) = delete;				// コピーコンストラクタ禁止
	CCharaManager& operator=(const CCharaManager&) = delete;	// コピー代入禁止
	CCharaManager(CCharaManager&&) = delete;					// ムーブコンストラクタ禁止
	CCharaManager& operator=(CCharaManager&&) = delete;			// ムーブ代入禁止


	//// 変数 ////

	using CHARA_LIST = std::list<IChara*>;
	CHARA_LIST	m_CharaList;
	std::map<CHARA_ID, std::function<IChara* ()>>	m_Generator;
};

#define CHARAS CCharaManager::GetInstance()