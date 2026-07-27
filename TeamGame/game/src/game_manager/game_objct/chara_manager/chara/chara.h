/**
*	@file	chara.h
*	@brief	キャラクターのベースクラス
*	@author	R.Kawakami
*/
#pragma once

#include "vivid.h"
#include "chara_id.h"

class IChara
{
public:
	/**
	*	@brief		コンストラクタ
	*/
	IChara(int width, int height, std::string data);
	IChara() = delete;

	/**
	*	@brief		デストラクタ
	*/
	~IChara() = default;

	/**
	*	@brief		初期化
	*
	*	@param[in]	pos			位置
	*	@param[in]	id			生成したキャラクターのID
	*/
	virtual void Initialize(const vivid::Vector2& pos, CHARA_ID id);

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
	*	@brief		キャラクターIDの取得
	*
	*	@return		自身のキャラクターID
	*/
	CHARA_ID GetCharaID();

protected:
	//// 関数 ////


	//// 定数 ////



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
	CHARA_ID		m_CharaID;			// エフェクトのID
	vivid::Vector2	m_Velocity;			// 速度
};