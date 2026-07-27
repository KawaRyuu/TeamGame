/**
*	@file	effect.h
*	@brief	エフェクトのベースクラス
*	@author	K.Kawakami
*/
#pragma once

#include "vivid.h"
#include "effect_id.h"

class IEffect
{
public:
	/**
	*	@brief		コンストラクタ
	*/
	IEffect(int width, int height, std::string data);
	IEffect() = delete;

	/**
	*	@brief		デストラクタ
	*/
	~IEffect() = default;

	/**
	*	@brief		初期化
	*
	*	@param[in]	pos			位置
	*	@param[in]	id			生成したエフェクトのID
	*	@param[in]	color		色
	*/
	virtual void Initialize(const vivid::Vector2& pos, EFFECT_ID id, unsigned int color);

	/*
	*	@brief		更新
	*/
	virtual void Update();

	/**
	*	@brief		描画
	*/
	virtual void Draw();

	/**
	*	@brief		解放
	*/
	virtual void Finalize();

	/**
	*	@brief		アクティブ判定の取得
	*/
	bool GetActive();

	/**
	*	@brief		アクティブ判定の設定
	*
	*	@param[in]	active	アクティブフラグ
	*/
	void SetActive(bool active);

	/**
	*	@brief		エフェクトIDの取得
	*
	*	@return		自身のエフェクトID
	*/
	EFFECT_ID GetEffectID();

protected:
	//// 関数 ////

	/**
	*	@brief		時間経過によるエフェクトの削除
	*/
	void AutoDeleteEffect();

	/**
	*	@brief		自身の半径の取得（計算）
	*/
	float GetRadius();


	//// 定数 ////

	static const int m_out_of_range_time;	// 範囲外にいられる時間


	//// 変数 ////

	int				m_Width;			// 幅
	int				m_Height;			// 高さ
	std::string		m_DrawData;			// 描画データ
	vivid::Vector2	m_Position;			// 位置
	unsigned int	m_Color;			// 色
	vivid::Rect		m_Rect;				// 描画範囲
	vivid::Vector2	m_Anchor;			// 基準点
	vivid::Vector2	m_Scale;			// 拡大率
	float			m_Rotation;			// 回転値
	bool			m_Active;			// アクティブ判定
	EFFECT_ID		m_EffectID;			// エフェクトのID
	vivid::Vector2	m_Velocity;			// 速度
	int				m_AutoDeleteTimer;	// 削除されるまでの時間のタイマー
};