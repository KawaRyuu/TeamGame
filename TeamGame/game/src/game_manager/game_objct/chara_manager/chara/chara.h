/**
*	@file	chara.h
*	@brief	キャラクターのベースクラス
*	@author	Ryuuki.Kawakami
*/
#pragma once

#include "vivid.h"
#include "chara_id.h"
#include "chara_state.h"
#include "contol.h"

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

	/**
	*	@brief		向きの取得 (右向きならtrue)
	*/
	virtual bool IsFacingRight() const { return m_FacingRight; }

	/**
	*	@brief		向きの取得 (右向きならtrue)
	*/
	virtual vivid::Rect GetHurtbox() const;

	/**
	*	@brief		向きの取得 (右向きならtrue)
	*/
	virtual bool IsInvulnerable() const { return false; }

	/**
	*	@brief		向きの取得 (右向きならtrue)
	*/
	virtual void ApplyHit(int, const vivid::Vector2&, CHARA_ID);

	void SetPlayerID(contol::PLAYER_ID pid) { m_PlayerID = pid; }
	contol::PLAYER_ID GetPlayerID() const { return m_PlayerID; }

protected:
	//// 関数 ////
	void SetParameter(int max_hp, int attack, float defence, float speed);
	void StateChange(CHARA_STATE state);

	/**
	*	@brief		キャラクターの待機
	*/
	void Wait();

	/**
	*	@brief		キャラクターの移動
	*/
	void Move();

	/**
	*	@brief		キャラクターのガード
	*/
	void Guard();

	/**
	*	@brief		キャラクターの特性
	*/
	void Skill();

	/**
	*	@brief		キャラクターがアイテムを使用する
	*/
	void UseItem();

	/**
	*	@brief		キャラクターの通常攻撃
	*/
	void NormalAttack();

	/**
	*	@brief		キャラクターの必殺
	*/
	void SureKill();

	/**
	*	@brief		キャラクター_復活
	*/
	void Resurrection();

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

	int				m_MaxHp;			// 最大体力
	int				m_CurrentHp;		// 現在の体力
	int				m_BaseAttack;		// 攻撃力の基準
	float			m_Defence;			// 防御力
	float			m_Speed;			// 速度
	CHARA_STATE		m_CharaState;		// キャラクターの状
	bool m_FacingRight = true;	// 右向きかどうかのフラグ

	contol::PLAYER_ID m_PlayerID = contol::PLAYER_ID::P1;

	//// デバック用 ////
	vivid::Vector2	 m_DebugAttackCenter = vivid::Vector2::ZERO;
	float			 m_DebugAttackRadius = 0.0f;
	int				 m_DebugAttackTimer = 0;
};