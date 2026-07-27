/**
*	@file		contol.h
*	@brief		Contol Library (Controller Integration Library)
*	@author		Kouki Kawakami
*	@date		2026/05/25
*	@version	1.0.6
*
*	Copyright (c) 2026 Kouki Kawakami
*	All rights reserved.
*/

#pragma once

#include <DxLib.h>

namespace contol
{
	//// ID系 ////

	/**
	*	@brief		デバイスID
	* 
	*	@note		接続自体は16台まで可能
	*/
	enum class DEVICE_ID
	{
		D1,		D2,		D3,		D4,
		D5,		D6,		D7,		D8,
		D9,		D10,	D11,	D12,
		D13,	D14,	D15,	D16,

		MAX,
	};

	/**
	*	@brief		プレイヤーID
	* 
	*	@note		登録可能な台数は最大8人分
	*/
	enum class PLAYER_ID
	{
		P1,		P2,		P3,		P4,
		P5,		P6,		P7,		P8,
		ALL,

		MAX = 8,
	};

	/**
	*	@brief		ボタンID
	*/
	enum class BUTTON_ID
	{
		/* ボタンID			// x:Xbox,	n:NintendoSwitch,	p:PlayStation */

		START,				// x:メニュー,	n:+,	p:オプション
		BACK,				// x:ビュー,　	n:-,	p:クリエイト

		ACTION_UP,			// x:Y,	n:X,	p:△
		ACTION_DOWN,		// x:A,	n:B,	p:×
		ACTION_LEFT,		// x:X,	n:Y,	p:□
		ACTION_RIGHT,		// x:B,	n:A,	p:〇

		LB,					// x:LB,	n:L,	p:L1
		RB,					// x:RB,	n:R,	p:R1

		L_THUMB,			// 左スティック押し込み
		R_THUMB,			// 右スティック押し込み

		MAX,
	};

	/**
	*	@brief		D-Pad(十字キー)方向ID
	*/
	enum class D_PAD_ID
	{
		UP			= 0,		// 上
		RIGHT_UP	= 4500,		// 右上
		RIGHT		= 9000,		// 右
		RIGHT_DOWN	= 13500,	// 右下
		DOWN		= 18000,	// 下
		LEFT_DOWN	= 22500,	// 左下
		LEFT		= 27000,	// 左
		LEFT_UP		= 31500,	// 左上

		NONE		= -1,		// 無入力
	};

	/**
	*	@brief		左右ID
	*/
	enum class LR_ID
	{
		L,		// 左
		R,		// 右
	};

	/**
	*	@brief		軸ID
	*/
	enum class XY_ID
	{
		X,		// X軸
		Y,		// Y軸
	};

	/**
	*	@brief		スティック入力の向き
	*/
	enum class STICK_DIR
	{
		UP,		// 上
		DOWN,	// 下
		LEFT,	// 左
		RIGHT,	// 右
	};

	/**
	*	@brief		コントローラー種類取得用識別ID
	*/
	enum class PAD_ID
	{
		XBOX_360,
		XBOX_ONE,

		DUAL_SHOCK_3,
		DUAL_SHOCK_4,
		DUAL_SENSE,

		NS_JOY_CON_L,
		NS_JOY_CON_R,
		NS_JOY_CON,
		NS_PRO_CTRL,
		NS_HORI_PAD,

		OTHER,

		NONE,
	};


	//// 構造体 ////

	struct VECTOR2
	{
		float x, y;
	};

	struct INPUT_STATE
	{
		bool			buttons[static_cast<int>(contol::BUTTON_ID::MAX)];	// ボタン群（十字ボタン以外）

		unsigned int	d_pad_value;	// 十字ボタン

		contol::VECTOR2	ls;				// Lスティック
		contol::VECTOR2	rs;				// Rスティック

		float	lt;						// Lトリガー
		float	rt;						// Rトリガー
	};


	//// 通常関数 ////

	/**
	*	@brief		初期化
	*/
	void Initialize();

	/**
	*	@brief		更新
	*/
	void Update();

	
	//// 基本情報設定系関数 ////

	/**
	*	@brief		コントローラーのアクティブ台数の設定
	* 
	*	@param[in]	count	アクティブにする台数（1～8台）
	*/
	void SetActiveCount(int count);

	/**
	*	@brief		コントローラーのアクティブ台数の取得
	* 
	*	@return		アクティブになっている台数
	*/
	int GetActiveCount();

	/**
	*	@brief		先頭から空いているプレイヤー番号にコントローラーを登録する
	*/
	void SetPlayerDevice();

	/**
	*	@brief		登録されているコントローラーをプレイヤーから除外する
	* 
	*	@param[in]	player	除外するプレイヤーのID
	*/
	void RemovePlayerDevice(contol::PLAYER_ID player);

	/**
	*	@brief		全てのプレイヤーがコントローラー未接続時に、デバイス1をプレイヤー1に割り当てる
	*/
	void AutoBindPlayer1();

	/**
	*	@brief		自動でプレイヤーとデバイスを割り当てる
	*/
	void AutoBindPlayer();

	/**
	*	@brief		Joy-Conの横持ちの可否の設定
	* 
	*	@param[in]	can_flag	横持ち可能フラグ
	*/
	void SetJoyconCanHorizontal(bool can_flag);

	/**
	*	@brief		使用デバイスの取得
	*
	*	@param[in]	player	調べるプレイヤーのID
	*	@return		対応するPAD_IDを返す
	*/
	contol::PAD_ID GetUseDevice(contol::PLAYER_ID player);

	/**
	*	@brief		エントリー済みデバイス数の取得
	*
	*	@return		エントリー済みのデバイスの数を返す
	*/
	int GetDeviceEntryCount();


	//// 基本ボタン入力系関数 ////

	/**
	*	@brief		ボタン入力
	* 
	*	@param[in]	player:	取得したいプレイヤーのID
	*	@param[in]	button:	取得したいボタンのID
	*	@return		そのボタンが押されている間はtrueを返す
	*/
	bool Input(contol::PLAYER_ID player, contol::BUTTON_ID button);

	/**
	*	@brief		ボタン押し始め入力
	* 
	*	@param[in]	player:	取得したいプレイヤーのID
	*	@param[in]	button:	取得したいボタンのID
	*	@return		そのボタンが押された瞬間のみtrueを返す
	*/
	bool InputStart(contol::PLAYER_ID player, contol::BUTTON_ID button);

	/**
	*	@brief		ボタン押し終わり入力
	* 
	*	@param[in]	player:	取得したいプレイヤーのID
	*	@param[in]	button:	取得したいボタンのID
	*	@return		そのボタンを離した瞬間のみtrueを返す
	*/
	bool InputEnd(contol::PLAYER_ID player, contol::BUTTON_ID button);


	//// 十字ボタン入力系関数 ////

	/**
	*	@brief		十字ボタン入力
	* 
	*	@param[in]	player:	取得したいプレイヤーのID
	*	@param[in]	d_pad:	取得したい十字ボタン(向き)のID
	*	@return		そのボタンが押されている間はtrueを返す
	*/
	bool InputDPad(contol::PLAYER_ID player, contol::D_PAD_ID d_pad);

	/**
	*	@brief		十字ボタン押し始め入力
	* 
	*	@param[in]	player:	取得したいプレイヤーのID
	*	@param[in]	d_pad:	取得したい十字ボタン(向き)のID
	*	@return		そのボタンが押された瞬間のみtrueを返す
	*/
	bool InputDPadStart(contol::PLAYER_ID player, contol::D_PAD_ID d_pad);

	/**
	*	@brief		十字ボタン押し終わり入力
	* 
	*	@param[in]	player:	取得したいプレイヤーのID
	*	@param[in]	d_pad:	取得したい十字ボタン(向き)のID
	*	@return		そのボタンを離した瞬間のみtrueを返す
	*/
	bool InputDPadEnd(contol::PLAYER_ID player, contol::D_PAD_ID d_pad);


	//// スティック入力系関数 ////

	/**
	*	@brief		スティックのアナログ値の取得
	* 
	*	@param[in]	player:	取得したいプレイヤーのID
	*	@param[in]	lr:		取得したいスティックのID（左右）
	*	@param[in]	xy:		取得したい軸のID（XY）
	*	@return		指定したスティックの値を返す（-1.0f～1.0f）
	*/
	float GetStickValue(contol::PLAYER_ID player, contol::LR_ID lr, contol::XY_ID xy);

	/**
	*	@brief		スティックのデジタル入力
	*
	*	@param[in]	player:	取得したいプレイヤーのID
	*	@param[in]	lr:		取得したいスティックのID（左右）
	*	@param[in]	dir:	取得したいスティックの向きID（上下左右）
	*	@return		指定した方向に入力されている間はtrueを返す
	*/
	bool InputStick(contol::PLAYER_ID player, contol::LR_ID lr, contol::STICK_DIR dir);

	/**
	*	@brief		スティックのデジタル入力（押し始め）
	*
	*	@param[in]	player:	取得したいプレイヤーのID
	*	@param[in]	lr:		取得したいスティックのID（左右）
	*	@param[in]	dir:	取得したいスティックの向きID（上下左右）
	*	@return		指定した方向に入力された瞬間のみtrueを返す
	*/
	bool InputStickStart(contol::PLAYER_ID player, contol::LR_ID lr, contol::STICK_DIR dir);

	/**
	*	@brief		スティックのデジタル入力（押し終わり）
	*
	*	@param[in]	player:	取得したいプレイヤーのID
	*	@param[in]	lr:		取得したいスティックのID（左右）
	*	@param[in]	dir:	取得したいスティックの向きID（上下左右）
	*	@return		指定した方向の入力をやめた瞬間のみtrueを返す
	*/
	bool InputStickEnd(contol::PLAYER_ID player, contol::LR_ID lr, contol::STICK_DIR dir);


	//// トリガー入力系関数 ////

	/**
	*	@brief		トリガーのアナログ値の取得
	* 
	*	@param[in]	player:	取得したいプレイヤーのID
	*	@param[in]	lr:		取得したいトリガーのID（左右）
	*	@return		指定したトリガーの値を返す（0.0f～1.0f）
	*/
	float GetTriggerValue(contol::PLAYER_ID player, contol::LR_ID lr);

	/**
	*	@brief		トリガーのデジタル入力
	*
	*	@param[in]	player:	取得したいプレイヤーのID
	*	@param[in]	lr:		取得したいトリガーのID（左右）
	*	@return		指定トリガーが入力されている間はtrueを返す
	*/
	bool InputTrigger(contol::PLAYER_ID player, contol::LR_ID lr);

	/**
	*	@brief		トリガーのデジタル入力（押し始め）
	*
	*	@param[in]	player:	取得したいプレイヤーのID
	*	@param[in]	lr:		取得したいトリガーのID（左右）
	*	@return		指定トリガーが入力された瞬間のみtrueを返す
	*/
	bool InputTriggerStart(contol::PLAYER_ID player, contol::LR_ID lr);

	/**
	*	@brief		トリガーのデジタル入力（押し終わり）
	*
	*	@param[in]	player:	取得したいプレイヤーのID
	*	@param[in]	lr:		取得したいトリガーのID（左右）
	*	@return		指定トリガーが入力をやめた瞬間のみtrueを返す
	*/
	bool InputTriggerEnd(contol::PLAYER_ID player, contol::LR_ID lr);


	//// その他入力系関数 ////

	/**
	*	@brief		何かしら入力処理
	*
	*	@param[in]	players:	取得したいプレイヤーのID
	*	@param[in]	buttons:	ボタンを含めるか（default = true）
	*	@param[in]	sticks:		スティックを含めるか（default = true）
	*	@param[in]	triggers:	トリガーを含めるか（default = true）
	*	@return		上記3項目が何かしら入力状態の時trueを返す

	*/
	bool InputAnyKey(contol::PLAYER_ID player, bool buttons = true, bool sticks = true, bool triggers = true);


	//// 振動系関数 ////

	/**
	*	@brief		コントローラーの振動
	*
	*	@param[in]	player:	振動させるコントローラーのプレイヤーID
	*	@param[in]	power:	振動の強さ（0～1000）
	*	@param[in]	time:	振動の時間（秒）
	*	@param[in]	index:	振動させるモーター番号（default = -1(自動)）
	* 
	*	@note		Xbox系コントローラーのみ対応
	*/
	void VibrationStart(contol::PLAYER_ID player, int power, float time, int index = -1);

	/**
	*	@brief		コントローラー振動の停止
	*
	*	@param[in]	player:	停止させるコントローラーのプレイヤーID
	*	@param[in]	index:	停止させるモーター番号（default = -1(自動)）
	* 
	*	@note		モーター番号を指定した際はその番号で停止
	*/
	void VibrationStop(contol::PLAYER_ID player, int index = -1);


	//// その他システム系関数 ////
	
	/**
	*	@brief		コントローラー状態の更新
	*
	*	@note		基本contol内で使用
	*/
	void UpdateStates();

	/**
	*	@brief		コントローラー全体状態の更新
	*
	*	@note		基本contol内で使用
	*/
	void UpdateAllState();

	/**
	*	@brief		コントローラー別ボタン番号の取得
	* 
	*	@param[in]	player:	取得したいプレイヤーのID
	*	@param[in]	button:	取得したいボタンのID
	*	@param[in]	second:	2台目フラグ（default = false）
	*	@return		対応するボタン番号を返す
	* 
	*	@note		基本contol内で使用
	*/
	int GetButtonNum(contol::PLAYER_ID player, contol::BUTTON_ID button, bool second = false);

	/**
	*	@brief		DirectInput上のスティックのアナログ値の取得
	* 
	*	@param[in]	player:	取得したいプレイヤーのID
	*	@param[in]	lr:		取得したいスティックのID（左右）
	*	@param[in]	xy:		取得したい軸のID（XY）
	*	@param[in]	state:	取得したいデバイスのDirectInput状態
	*	@param[in]	second:	2台目フラグ（default = false）
	*	@return		指定したスティックの値を返す（-1.0f～1.0f）
	* 
	*	@note		基本contol内で使用
	*/
	float GetStickDirect(contol::PLAYER_ID player, contol::LR_ID lr, contol::XY_ID xy, DxLib::DINPUT_JOYSTATE& state, bool second = false);

	/**
	*	@brief		DirectInput上のトリガーのアナログ値の取得
	* 
	*	@param[in]	player:	取得したいプレイヤーのID
	*	@param[in]	lr:		取得したいトリガーのID（左右）
	*	@param[in]	state:	取得したいデバイスのDirectInput状態
	*	@param[in]	second:	2台目フラグ（default = false）
	*	@return		指定したトリガーの値を返す（0.0f～1.0f）
	* 
	*	@note		基本contol内で使用
	*/
	float GetTriggerDirect(contol::PLAYER_ID player, contol::LR_ID lr, DxLib::DINPUT_JOYSTATE& state, bool second = false);

	/**
	*	@brief		DirectInput上のスティックのアナログ値の取得
	*
	*	@param[in]	have_two:	2本持ちフラグ（false:1本持ち）
	*	@param[in]	lr:			Joy-Conの左右
	*	@param[in]	xy:			取得したい軸のID（XY）
	*	@param[in]	state:		取得したいデバイスのDirectInput状態
	*	@return		Joy-Conスティックの値をunsigned intからfloatに変換して返す（-1.0f～1.0f）
	*
	*	@note		基本contol内、Joy-Conの時のみ使用
	*/
	float GetJoyconStickValue(bool have_two, contol::LR_ID lr, contol::XY_ID xy, DxLib::DINPUT_JOYSTATE& state);

	/**
	*	@brief		各デバイスのエントリー
	* 
	*	@param[in]	empty	空いているプレイヤー番号
	* 
	*	@note		基本contol内で使用
	*/
	void EntryDevice(int empty);

	/**
	*	@brief		Joy-Con2本持ちの整合性チェック
	* 
	*	@note		基本contol内で使用
	*/
	void CheckSecondJoycon();

	/**
	*	@brief		デバイスとIDの整合性チェック
	* 
	*	@note		基本contol内で使用
	*/
	void CheckDeviceAndID();

}