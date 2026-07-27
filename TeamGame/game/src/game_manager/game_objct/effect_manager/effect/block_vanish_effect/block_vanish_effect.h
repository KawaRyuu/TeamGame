/**
*	@file	block_vanish_effect.h
*	@brief	ブロック削除のエフェクト
*	@author	K.Kawakami
*/
#pragma once

#include "vivid.h"
#include "../effect.h"

class CBlockVanishEffect : public IEffect
{
public:
	//// 関数 //// 

	/**
	*	@brief		コンストラクタ
	*/
	CBlockVanishEffect();

	/**
	*	@brief		デストラクタ
	*/
	~CBlockVanishEffect() = default;

	/**
	*	@brief		初期化
	*
	*	@param[in]	pos			位置
	*	@param[in]	id			生成したエフェクトのID
	*	@param[in]	color		色
	*/
	void Initialize(const vivid::Vector2& pos, EFFECT_ID id, unsigned int color) override;

	/**
	*	@brief		更新
	*/
	void Update() override;

	/**
	*	@brief		描画
	*/
	void Draw() override;


private:
	//// 関数 ////


	//// 定数 //// 	

	static const int	m_max_particle = 8;	// 表示パーティクル数
	static const float	m_speed;			// 基本速度（初速）
	static const float	m_decelerator;		// 減速度
	static const float	m_scale_rate;		// 縮小率

	
	//// 変数 //// 

	vivid::Vector2	m_Positions[m_max_particle];	// 各パーティクルの基準点からの位置
	vivid::Vector2	m_Velocitys[m_max_particle];	// 各パーティクルの速度

};