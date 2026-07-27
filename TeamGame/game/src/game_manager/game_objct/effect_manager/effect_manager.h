/**
*	@file	effect_manager.h
*	@brief	生成するエフェクトのマネージャー
*	@author	K.Kawakami
*/

#pragma once

#include "vivid.h"
#include "effect/effect_id.h"
#include <list>
#include <map>
#include <functional>

class IEffect;

class CEffectManager
{
public:
	/**
	*	@brief		インスタンスの取得
	*
	*	@return		インスタンス
	*/
	static CEffectManager& GetInstance();

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
	*	@brief		エフェクトの生成
	*
	*	@param[in]	id		アイテムID
	*	@param[in]	pos		位置
	*	@param[in]	color	色（default = 0xffffffff(白)）
	*/
	void Create(EFFECT_ID id, const vivid::Vector2& pos, unsigned int color = 0xffffffff);

	/**
	*	@brief		エフェクトの削除
	*
	*	@param[in]	id		エフェクトID
	*/
	void DeleteEffect(EFFECT_ID id);


private:
	//// 関数 ////

	CEffectManager() = default;									// コンストラクタ
	~CEffectManager() = default;								// デストラクタ
	CEffectManager(const CEffectManager&) = delete;				// コピーコンストラクタ禁止
	CEffectManager& operator=(const CEffectManager&) = delete;	// コピー代入禁止
	CEffectManager(CEffectManager&&) = delete;					// ムーブコンストラクタ禁止
	CEffectManager& operator=(CEffectManager&&) = delete;		// ムーブ代入禁止


	//// 変数 ////

	using EFFECT_LIST = std::list<IEffect*>;
	EFFECT_LIST	m_EffectList;
	std::map<EFFECT_ID, std::function<IEffect* ()>>	m_Generator;
};

#define EFFECTS CEffectManager::GetInstance()