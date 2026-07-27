/**
*	@file	ui.h
*	@brief	UIのベースクラス
*	@author	K.Kawakami
*/
#pragma once

#include "contol.h"
#include "vivid.h"
#include "ui_id.h"

class IUi
{
public:
	//// 関数 //// 

	/**
	*	@brief		コンストラクタ
	*	
	*	@param[in]	width	幅
	*	@param[in]	height	高さ
	*	@param[in]	data	描画ファイル名
	*/
	IUi(int width, int height, std::string data);
	IUi() = delete;

	/**
	*	@brief		デストラクタ
	*/
	~IUi() = default;

	/**
	*	@brief		初期化
	*
	*	@param[in]	pos			位置
	*	@param[in]	id			生成したUIのID
	*	@param[in]	ui_draw		UIを描画する位置（手前か奥か）
	*	@param[in]	playre_id	UIを生成したプレイヤーのID（誰のUIか）
	*/
	virtual void Initialize(const vivid::Vector2& pos, UI_ID id, UI_DRAW_POS ui_draw, contol::PLAYER_ID player_id);

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
	virtual bool GetActive();

	/**
	*	@brief		アクティブ判定の設定
	*
	*	@param[in]	active	アクティブフラグ
	*/
	virtual void SetActive(bool active);

	/**
	*	@brief		UIIDの取得
	*
	*	@return		自身のUIID
	*/
	virtual UI_ID GetUiID();

	/**
	*	@brief		UIの表示位置の取得
	*
	*	@return		表示位置ID
	*/
	virtual UI_DRAW_POS GetUiDrawPos();


protected:
	//// 変数 ////

	int					m_Width;			// 幅
	int					m_Height;			// 高さ
	std::string			m_DrawData;			// 描画データ
	vivid::Vector2		m_Position;			// 位置
	unsigned int		m_Color;			// 色
	vivid::Rect			m_Rect;				// 描画範囲
	vivid::Vector2		m_Anchor;			// 基準点
	vivid::Vector2		m_Scale;			// 拡大率
	float				m_Rotation;			// 回転値
	bool				m_Active;			// アクティブ判定
	UI_ID				m_UiID;				// UIのID
	vivid::Vector2		m_Velocity;			// 速度
	int					m_OutOfRangeTimer;	// 範囲外にいられる時間のタイマー
	UI_DRAW_POS			m_UiDrawPos;		// UIの表示位置(UI)
	contol::PLAYER_ID	m_PlayerID;			// プレイヤーID（誰のUIか、不要の場合はALLを使う）
};