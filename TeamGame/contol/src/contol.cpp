/**
*	@file		contol.cpp
*	@brief		Contol Library (Controller Integration Library)
*	@author		Kouki Kawakami
*	@date		2026/05/25
*	@version	1.0.6
*
*	Copyright (c) 2026 Kouki Kawakami
*	All rights reserved.
*/

#include "pch.h"
#include "contol.h"

namespace contol
{
	//// 定数 ////

	static const int	g_max_analog_value = 1000;		// アナログ値の基本最大値
	static const int	g_max_x_trigger = 255;			// XInputのトリガーの最大値
	static const float	g_analog_once_value = 0.25f;	// デフォルトのアナログ値のデジタル入力境界値
	static const int	g_default_active_count = 4;		// コントローラーアクティブ台数のデフォルト値


	//// 変数 ////

	contol::INPUT_STATE		g_InputState[static_cast<int>(contol::PLAYER_ID::MAX) + 1];			// 入力状態
	contol::INPUT_STATE		g_PrevInputState[static_cast<int>(contol::PLAYER_ID::MAX) + 1];		// 1F前の入力状態
	contol::DEVICE_ID		g_DeviceID[static_cast<int>(contol::PLAYER_ID::MAX)];			// 各プレイヤーのデバイスID格納変数
	contol::DEVICE_ID		g_SecondDeviceID[static_cast<int>(contol::PLAYER_ID::MAX)];		// 各プレイヤーの2つ目のデバイスID格納変数（Joy-Con2つ持ち用）
	float	g_AnalogOnceDeadValue = g_analog_once_value;		// アナログ値のデジタル入力境界値
	int		g_ActiveCount = g_default_active_count;				// コントローラーアクティブ台数
	bool	g_CanHorizontal = true;

}

void contol::Initialize()
{
	// コントローラーの状態を初期化
	memset((g_InputState), 0, (sizeof(contol::INPUT_STATE) * (static_cast<int>(contol::PLAYER_ID::MAX) + 1)));
	memset((g_PrevInputState), 0, (sizeof(contol::INPUT_STATE) * (static_cast<int>(contol::PLAYER_ID::MAX) + 1)));

	// 全てのデバイスIDをMAXに設定
	for (int player = 0; player < static_cast<int>(contol::PLAYER_ID::MAX); player++)
	{
		g_DeviceID[player] = contol::DEVICE_ID::MAX;
		g_SecondDeviceID[player] = contol::DEVICE_ID::MAX;
	}
}

void contol::Update()
{
	// コントローラー状態の更新
	contol::UpdateStates();

	// デバイスとIDの整合性チェック
	contol::CheckDeviceAndID();
}

void contol::SetActiveCount(int count)
{
	// 受け取った値を1～8に丸めて代入
	g_ActiveCount = (count > static_cast<int>(contol::PLAYER_ID::MAX)) ? static_cast<int>(contol::PLAYER_ID::MAX)
		: ((count < 1) ? 1 : count);
}

int contol::GetActiveCount()
{
	return g_ActiveCount;
}

void contol::SetPlayerDevice()
{
	// 空いているプレイヤー番号を先頭からチェックする
	int empty = 0;
	for (empty = 0; empty < g_ActiveCount; empty++)
	{
		if (g_DeviceID[empty] == contol::DEVICE_ID::MAX)	break;
	}

	// 全て埋まっている時は設定しない
	if (empty >= g_ActiveCount)		return;

	contol::EntryDevice(empty);

	return;
}

void contol::RemovePlayerDevice(contol::PLAYER_ID player)
{
	// ALL指定(または範囲外)の場合は全てリセット
	if (static_cast<int>(player) >= static_cast<int>(contol::PLAYER_ID::ALL) || static_cast<int>(player) < 0)
	{
		for (int num = 0; num < static_cast<int>(contol::PLAYER_ID::MAX); num++)
		{
			g_DeviceID[num] = contol::DEVICE_ID::MAX;
			g_SecondDeviceID[num] = contol::DEVICE_ID::MAX;
		}

		return;
	}

	// それ以外（プレイヤー指定）
	g_DeviceID[static_cast<int>(player)] = contol::DEVICE_ID::MAX;
	g_SecondDeviceID[static_cast<int>(player)] = contol::DEVICE_ID::MAX;
}

void contol::AutoBindPlayer1()
{
	// アクティブなプレイヤーが一人でもコントローラー接続されている場合は処理しない
	for (int player = 0; player < g_ActiveCount; player++)
	{
		if (g_DeviceID[static_cast<int>(player)] != contol::DEVICE_ID::MAX)	return;
	}

	// プレイヤー1にデバイス1を割り当てる
	g_DeviceID[static_cast<int>(contol::PLAYER_ID::P1)] = contol::DEVICE_ID::D1;
}

void contol::AutoBindPlayer()
{
	// IDのリセット
	for (int player = 0; player < static_cast<int>(contol::PLAYER_ID::MAX); player++)
	{
		g_DeviceID[static_cast<int>(player)] = contol::DEVICE_ID::MAX;
	}

	// [プレイヤー1←デバイス1][プレイヤー2←デバイス2]…のように割り当てる
	for (int player = 0; player < g_ActiveCount; player++)
	{
		g_DeviceID[static_cast<int>(player)] = static_cast<contol::DEVICE_ID>(player);
	}
}

void contol::SetJoyconCanHorizontal(bool can_flag)
{
	g_CanHorizontal = can_flag;
}

contol::PAD_ID contol::GetUseDevice(contol::PLAYER_ID player)
{
	// 対応するPAD_IDを返す
	switch (DxLib::GetJoypadType(static_cast<int>(g_DeviceID[static_cast<int>(player)]) + DX_INPUT_PAD1))
	{
	case DX_PADTYPE_OTHER:				return contol::PAD_ID::OTHER;

	case DX_PADTYPE_XBOX_360:			return contol::PAD_ID::XBOX_360;
	case DX_PADTYPE_XBOX_ONE:			return contol::PAD_ID::XBOX_ONE;

	case DX_PADTYPE_DUAL_SHOCK_3:		return contol::PAD_ID::DUAL_SHOCK_3;
	case DX_PADTYPE_DUAL_SHOCK_4:		return contol::PAD_ID::DUAL_SHOCK_4;
	case DX_PADTYPE_DUAL_SENSE:			return contol::PAD_ID::DUAL_SENSE;

	case DX_PADTYPE_SWITCH_JOY_CON_L:
		if (DxLib::GetJoypadType(static_cast<int>(g_SecondDeviceID[static_cast<int>(player)]) + DX_INPUT_PAD1) == DX_PADTYPE_SWITCH_JOY_CON_R)
										return contol::PAD_ID::NS_JOY_CON;
		else							return contol::PAD_ID::NS_JOY_CON_L;
	case DX_PADTYPE_SWITCH_JOY_CON_R:	return contol::PAD_ID::NS_JOY_CON_R;
	case DX_PADTYPE_SWITCH_PRO_CTRL:	return contol::PAD_ID::NS_PRO_CTRL;
	case DX_PADTYPE_SWITCH_HORI_PAD:	return contol::PAD_ID::NS_HORI_PAD;
	}

	return contol::PAD_ID::NONE;
}

int contol::GetDeviceEntryCount()
{
	int count = 0;

	// エントリー済みのデバイスの数を数える
	for (int player = 0; player < g_ActiveCount; player++)
	{
		if (g_DeviceID[player] != contol::DEVICE_ID::MAX)
			count++;
	}

	return count;
}

bool contol::Input(contol::PLAYER_ID player, contol::BUTTON_ID button)
{
	return g_InputState[static_cast<int>(player)].buttons[static_cast<int>(button)];
}

bool contol::InputStart(contol::PLAYER_ID player, contol::BUTTON_ID button)
{
	// 現在入力中で1F前未入力の時はtrue
	return (g_InputState[static_cast<int>(player)].buttons[static_cast<int>(button)])
		&& !(g_PrevInputState[static_cast<int>(player)].buttons[static_cast<int>(button)]);
}

bool contol::InputEnd(contol::PLAYER_ID player, contol::BUTTON_ID button)
{
	// 現在未入力で1F前入力していた時はtrue
	return !(g_InputState[static_cast<int>(player)].buttons[static_cast<int>(button)])
		&& (g_PrevInputState[static_cast<int>(player)].buttons[static_cast<int>(button)]);
}

bool contol::InputDPad(contol::PLAYER_ID player, contol::D_PAD_ID d_pad)
{
	return (g_InputState[static_cast<int>(player)].d_pad_value == static_cast<unsigned int>(d_pad));
}

bool contol::InputDPadStart(contol::PLAYER_ID player, contol::D_PAD_ID d_pad)
{
	// 現在入力中で1F前未入力の時はtrue
	return (g_InputState[static_cast<int>(player)].d_pad_value == static_cast<unsigned int>(d_pad))
		&& !(g_PrevInputState[static_cast<int>(player)].d_pad_value == static_cast<unsigned int>(d_pad));
}

bool contol::InputDPadEnd(contol::PLAYER_ID player, contol::D_PAD_ID d_pad)
{
	// 現在未入力で1F前入力していた時はtrue
	return !(g_InputState[static_cast<int>(player)].d_pad_value == static_cast<unsigned int>(d_pad))
		&& (g_PrevInputState[static_cast<int>(player)].d_pad_value == static_cast<unsigned int>(d_pad));
}

float contol::GetStickValue(contol::PLAYER_ID player, contol::LR_ID lr, contol::XY_ID xy)
{
	float value = 0.0f;

	if (lr == LR_ID::L && xy == XY_ID::X)		value = g_InputState[static_cast<int>(player)].ls.x;	// 左スティックのX軸
	else if (lr == LR_ID::L && xy == XY_ID::Y)	value = g_InputState[static_cast<int>(player)].ls.y;	// 左スティックのY軸
	else if (lr == LR_ID::R && xy == XY_ID::X)	value = g_InputState[static_cast<int>(player)].rs.x;	// 右スティックのX軸
	else if (lr == LR_ID::R && xy == XY_ID::Y)	value = g_InputState[static_cast<int>(player)].rs.y;	// 右スティックのY軸

	return value;
}

bool contol::InputStick(contol::PLAYER_ID player, contol::LR_ID lr, contol::STICK_DIR dir)
{
	bool flag = false;

	// 左スティック
	if (lr == LR_ID::L)
	{
		switch (dir)
		{
		case contol::STICK_DIR::UP:
			flag = g_InputState[static_cast<int>(player)].ls.y < -g_analog_once_value;
			break;
		case contol::STICK_DIR::DOWN:
			flag = g_InputState[static_cast<int>(player)].ls.y > g_analog_once_value;
			break;
		case contol::STICK_DIR::LEFT:
			flag = g_InputState[static_cast<int>(player)].ls.x < -g_analog_once_value;
			break;
		case contol::STICK_DIR::RIGHT:
			flag = g_InputState[static_cast<int>(player)].ls.x > g_analog_once_value;
			break;
		}
	}
	// 右スティック
	else
	{
		switch (dir)
		{
		case contol::STICK_DIR::UP:
			flag = g_InputState[static_cast<int>(player)].rs.y < -g_analog_once_value;
			break;
		case contol::STICK_DIR::DOWN:
			flag = g_InputState[static_cast<int>(player)].rs.y > g_analog_once_value;
			break;
		case contol::STICK_DIR::LEFT:
			flag = g_InputState[static_cast<int>(player)].rs.x < -g_analog_once_value;
			break;
		case contol::STICK_DIR::RIGHT:
			flag = g_InputState[static_cast<int>(player)].rs.x > g_analog_once_value;
			break;
		}
	}

	return flag;
}

bool contol::InputStickStart(contol::PLAYER_ID player, contol::LR_ID lr, contol::STICK_DIR dir)
{
	bool flag = false;

	// 左スティック
	if (lr == LR_ID::L)
	{
		switch (dir)
		{
		case contol::STICK_DIR::UP:
			flag = g_InputState[static_cast<int>(player)].ls.y < -g_analog_once_value
				&& g_PrevInputState[static_cast<int>(player)].ls.y >= -g_analog_once_value;
			break;
		case contol::STICK_DIR::DOWN:
			flag = g_InputState[static_cast<int>(player)].ls.y > g_analog_once_value
				&& g_PrevInputState[static_cast<int>(player)].ls.y <= g_analog_once_value;
			break;
		case contol::STICK_DIR::LEFT:
			flag = g_InputState[static_cast<int>(player)].ls.x < -g_analog_once_value
				&& g_PrevInputState[static_cast<int>(player)].ls.x >= -g_analog_once_value;
			break;
		case contol::STICK_DIR::RIGHT:
			flag = g_InputState[static_cast<int>(player)].ls.x > g_analog_once_value
				&& g_PrevInputState[static_cast<int>(player)].ls.x <= g_analog_once_value;
			break;
		}
	}
	// 右スティック
	else
	{
		switch (dir)
		{
		case contol::STICK_DIR::UP:
			flag = g_InputState[static_cast<int>(player)].rs.y < -g_analog_once_value
				&& g_PrevInputState[static_cast<int>(player)].rs.y >= -g_analog_once_value;
			break;
		case contol::STICK_DIR::DOWN:
			flag = g_InputState[static_cast<int>(player)].rs.y > g_analog_once_value
				&& g_PrevInputState[static_cast<int>(player)].rs.y <= g_analog_once_value;
			break;
		case contol::STICK_DIR::LEFT:
			flag = g_InputState[static_cast<int>(player)].rs.x < -g_analog_once_value
				&& g_PrevInputState[static_cast<int>(player)].rs.x >= -g_analog_once_value;
			break;
		case contol::STICK_DIR::RIGHT:
			flag = g_InputState[static_cast<int>(player)].rs.x > g_analog_once_value
				&& g_PrevInputState[static_cast<int>(player)].rs.x <= g_analog_once_value;
			break;
		}
	}

	// 現在入力中で1F前未入力の時はtrue
	return flag;
}

bool contol::InputStickEnd(contol::PLAYER_ID player, contol::LR_ID lr, contol::STICK_DIR dir)
{
	bool flag = false;

	// 左スティック
	if (lr == LR_ID::L)
	{
		switch (dir)
		{
		case contol::STICK_DIR::UP:
			flag = g_InputState[static_cast<int>(player)].ls.y >= -g_analog_once_value
				&& g_PrevInputState[static_cast<int>(player)].ls.y < -g_analog_once_value;
			break;
		case contol::STICK_DIR::DOWN:
			flag = g_InputState[static_cast<int>(player)].ls.y <= g_analog_once_value
				&& g_PrevInputState[static_cast<int>(player)].ls.y > g_analog_once_value;
			break;
		case contol::STICK_DIR::LEFT:
			flag = g_InputState[static_cast<int>(player)].ls.x >= -g_analog_once_value
				&& g_PrevInputState[static_cast<int>(player)].ls.x < -g_analog_once_value;
			break;
		case contol::STICK_DIR::RIGHT:
			flag = g_InputState[static_cast<int>(player)].ls.x <= g_analog_once_value
				&& g_PrevInputState[static_cast<int>(player)].ls.x > g_analog_once_value;
			break;
		}
	}
	// 右スティック
	else
	{
		switch (dir)
		{
		case contol::STICK_DIR::UP:
			flag = g_InputState[static_cast<int>(player)].rs.y >= -g_analog_once_value
				&& g_PrevInputState[static_cast<int>(player)].rs.y < -g_analog_once_value;
			break;
		case contol::STICK_DIR::DOWN:
			flag = g_InputState[static_cast<int>(player)].rs.y <= g_analog_once_value
				&& g_PrevInputState[static_cast<int>(player)].rs.y > g_analog_once_value;
			break;
		case contol::STICK_DIR::LEFT:
			flag = g_InputState[static_cast<int>(player)].rs.x >= -g_analog_once_value
				&& g_PrevInputState[static_cast<int>(player)].rs.x < -g_analog_once_value;
			break;
		case contol::STICK_DIR::RIGHT:
			flag = g_InputState[static_cast<int>(player)].rs.x <= g_analog_once_value
				&& g_PrevInputState[static_cast<int>(player)].rs.x > g_analog_once_value;
			break;
		}
	}

	// 現在未入力で1F前入力していた時はtrue
	return flag;
}

float contol::GetTriggerValue(contol::PLAYER_ID player, contol::LR_ID lr)
{
	float value = 0.0f;

	// 左トリガー
	if (lr == LR_ID::L)	value = g_InputState[static_cast<int>(player)].lt;
	// 右トリガー
	else				value = g_InputState[static_cast<int>(player)].rt;

	return value;
}

bool contol::InputTrigger(contol::PLAYER_ID player, contol::LR_ID lr)
{
	bool flag = false;

	// 左トリガー
	if (lr == LR_ID::L)	flag = g_InputState[static_cast<int>(player)].lt > g_analog_once_value;
	// 右トリガー
	else				flag = g_InputState[static_cast<int>(player)].rt > g_analog_once_value;

	return flag;
}

bool contol::InputTriggerStart(contol::PLAYER_ID player, contol::LR_ID lr)
{
	bool flag = false;

	// 左トリガー
	if (lr == LR_ID::L)	flag = g_InputState[static_cast<int>(player)].lt > g_analog_once_value 
							&& g_PrevInputState[static_cast<int>(player)].lt <= g_analog_once_value;
	// 右トリガー
	else				flag = g_InputState[static_cast<int>(player)].rt > g_analog_once_value 
							&& g_PrevInputState[static_cast<int>(player)].rt <= g_analog_once_value;

	// 現在入力中で1F前未入力の時はtrue
	return flag;
}

bool contol::InputTriggerEnd(contol::PLAYER_ID player, contol::LR_ID lr)
{
	bool flag = false;

	// 左トリガー
	if (lr == LR_ID::L)	flag = g_InputState[static_cast<int>(player)].lt <= g_analog_once_value
		&& g_PrevInputState[static_cast<int>(player)].lt > g_analog_once_value;
	// 右トリガー
	else				flag = g_InputState[static_cast<int>(player)].rt <= g_analog_once_value
		&& g_PrevInputState[static_cast<int>(player)].rt > g_analog_once_value;

	// 現在未入力で1F前入力していた時はtrue
	return flag;
}

bool contol::InputAnyKey(contol::PLAYER_ID player, bool buttons, bool sticks, bool triggers)
{
	bool flag = false;

	// ボタン
	if (buttons)
	{
		// 基本ボタン
		for (int button = 0; button < static_cast<int>(contol::BUTTON_ID::MAX); button++)
		{
			flag |= g_InputState[static_cast<int>(player)].buttons[button];
		}
		// 十字ボタン
		flag |= (g_InputState[static_cast<int>(player)].d_pad_value != static_cast<unsigned int>(contol::D_PAD_ID::NONE));
	}
	// スティック
	if (sticks)
	{
		// Lスティック
		flag |= (g_InputState[static_cast<int>(player)].ls.x > g_analog_once_value
			|| g_InputState[static_cast<int>(player)].ls.x < -g_analog_once_value
			|| g_InputState[static_cast<int>(player)].ls.y > g_analog_once_value
			|| g_InputState[static_cast<int>(player)].ls.y < -g_analog_once_value);
		// Rスティック
		flag |= (g_InputState[static_cast<int>(player)].rs.x > g_analog_once_value
			|| g_InputState[static_cast<int>(player)].rs.x < -g_analog_once_value
			|| g_InputState[static_cast<int>(player)].rs.y > g_analog_once_value
			|| g_InputState[static_cast<int>(player)].rs.y < -g_analog_once_value);
	}
	// トリガー
	if (triggers)
	{
		flag |= (g_InputState[static_cast<int>(player)].lt > g_analog_once_value
			|| g_InputState[static_cast<int>(player)].rt > g_analog_once_value);
	}

	return flag;
}

void contol::VibrationStart(contol::PLAYER_ID player, int power, float time, int index)
{
	// 通常のプレイヤーID指定（1P～8P）
	if (static_cast<int>(player) >= 0 && static_cast<int>(player) < static_cast<int>(contol::PLAYER_ID::MAX))
	{
		DxLib::StartJoypadVibration(static_cast<int>(g_DeviceID[static_cast<int>(player)]) + DX_INPUT_PAD1, power, static_cast<int>(time * 1000.0f), index);
		return;
	}

	// その他（指定プレイヤーIDがALL）
	for (int num = 0; num < static_cast<int>(contol::PLAYER_ID::MAX); num++)
	{
		DxLib::StartJoypadVibration(static_cast<int>(g_DeviceID[num]) + DX_INPUT_PAD1, power, static_cast<int>(time * 1000.0f), index);
	}
}

void contol::VibrationStop(contol::PLAYER_ID player, int index)
{
	// 通常のプレイヤーID指定（1P～8P）
	if (static_cast<int>(player) >= 0 && static_cast<int>(player) < static_cast<int>(contol::PLAYER_ID::MAX))
	{
		DxLib::StopJoypadVibration(static_cast<int>(g_DeviceID[static_cast<int>(player)]) + DX_INPUT_PAD1, index);
		return;
	}

	// その他（指定プレイヤーIDがALL）
	for (int num = 0; num < static_cast<int>(contol::PLAYER_ID::MAX); num++)
	{
		DxLib::StopJoypadVibration(static_cast<int>(g_DeviceID[num]) + DX_INPUT_PAD1, index);
	}
}

void contol::UpdateStates()
{
	// アクティブな台数(かつ接続されている台数)分だけ、コントローラーの入力情報取得
	for (int player = 0; player < DxLib::GetJoypadNum() && player < static_cast<int>(contol::PLAYER_ID::MAX) && player < g_ActiveCount; player++)
	{
		// 1フレーム前の状態を保存
		g_PrevInputState[player] = g_InputState[player];

		// 1台の時（通常/Joy-Con片手持ち時）
		if (g_SecondDeviceID[player] == contol::DEVICE_ID::MAX)
		{
			DINPUT_JOYSTATE d_state;	// DirectInputの状態変数
			// 現在の入力状態を取得
			DxLib::GetJoypadDirectInputState(static_cast<int>(g_DeviceID[player]) + DX_INPUT_PAD1, &d_state);

			//// DirectInputの情報をInputStateに変換 ////

			// ボタン
			for (int button = 0; button < static_cast<int>(contol::BUTTON_ID::MAX); button++)
			{
				g_InputState[player].buttons[button] =
					(d_state.Buttons[contol::GetButtonNum(static_cast<contol::PLAYER_ID>(player), static_cast<contol::BUTTON_ID>(button))] != 0);
			}

			// 十字ボタン
			g_InputState[player].d_pad_value = d_state.POV[0];

			// スティック
			g_InputState[player].ls.x = contol::GetStickDirect(static_cast<contol::PLAYER_ID>(player), LR_ID::L, XY_ID::X, d_state);
			g_InputState[player].ls.y = contol::GetStickDirect(static_cast<contol::PLAYER_ID>(player), LR_ID::L, XY_ID::Y, d_state);
			g_InputState[player].rs.x = contol::GetStickDirect(static_cast<contol::PLAYER_ID>(player), LR_ID::R, XY_ID::X, d_state);
			g_InputState[player].rs.y = contol::GetStickDirect(static_cast<contol::PLAYER_ID>(player), LR_ID::R, XY_ID::Y, d_state);

			// トリガー
			g_InputState[player].lt = contol::GetTriggerDirect(static_cast<contol::PLAYER_ID>(player), LR_ID::L, d_state);
			g_InputState[player].rt = contol::GetTriggerDirect(static_cast<contol::PLAYER_ID>(player), LR_ID::R, d_state);
		}
		// 2台の時（Joy-Con両手持ち時）
		else
		{
			DINPUT_JOYSTATE d_state1;	// DirectInputの状態変数
			DINPUT_JOYSTATE d_state2;	// DirectInputの状態変数

			// 現在の入力状態を取得
			DxLib::GetJoypadDirectInputState(static_cast<int>(g_DeviceID[player]) + DX_INPUT_PAD1, &d_state1);
			DxLib::GetJoypadDirectInputState(static_cast<int>(g_SecondDeviceID[player]) + DX_INPUT_PAD1, &d_state2);

			//// DirectInputの情報をInputStateに変換 ////

			// ボタン
			for (int button = 0; button < static_cast<int>(contol::BUTTON_ID::MAX); button++)
			{
				g_InputState[player].buttons[button] =
					(d_state1.Buttons[contol::GetButtonNum(static_cast<contol::PLAYER_ID>(player), static_cast<contol::BUTTON_ID>(button))] != 0);
				if (contol::GetButtonNum(static_cast<contol::PLAYER_ID>(player), static_cast<contol::BUTTON_ID>(button), true) != 31)
					g_InputState[player].buttons[button] =
					(d_state2.Buttons[contol::GetButtonNum(static_cast<contol::PLAYER_ID>(player), static_cast<contol::BUTTON_ID>(button), true)] != 0);
			}

			// 十字ボタン
			if (d_state1.Buttons[2] != 0 && d_state1.Buttons[0] == 0 && d_state1.Buttons[3] == 0)		g_InputState[player].d_pad_value = static_cast<unsigned int>(contol::D_PAD_ID::UP);
			else if (d_state1.Buttons[2] != 0 && d_state1.Buttons[3] != 0)								g_InputState[player].d_pad_value = static_cast<unsigned int>(contol::D_PAD_ID::RIGHT_UP);
			else if (d_state1.Buttons[3] != 0 && d_state1.Buttons[2] == 0 && d_state1.Buttons[1] == 0)	g_InputState[player].d_pad_value = static_cast<unsigned int>(contol::D_PAD_ID::RIGHT);
			else if (d_state1.Buttons[3] != 0 && d_state1.Buttons[1] != 0)								g_InputState[player].d_pad_value = static_cast<unsigned int>(contol::D_PAD_ID::RIGHT_DOWN);
			else if (d_state1.Buttons[1] != 0 && d_state1.Buttons[3] == 0 && d_state1.Buttons[0] == 0)	g_InputState[player].d_pad_value = static_cast<unsigned int>(contol::D_PAD_ID::DOWN);
			else if (d_state1.Buttons[1] != 0 && d_state1.Buttons[0] != 0)								g_InputState[player].d_pad_value = static_cast<unsigned int>(contol::D_PAD_ID::LEFT_DOWN);
			else if (d_state1.Buttons[0] != 0 && d_state1.Buttons[1] == 0 && d_state1.Buttons[2] == 0)	g_InputState[player].d_pad_value = static_cast<unsigned int>(contol::D_PAD_ID::LEFT);
			else if (d_state1.Buttons[0] != 0 && d_state1.Buttons[2] != 0)								g_InputState[player].d_pad_value = static_cast<unsigned int>(contol::D_PAD_ID::LEFT_UP);
			else																						g_InputState[player].d_pad_value = static_cast<unsigned int>(contol::D_PAD_ID::NONE);

			// スティック
			g_InputState[player].ls.x = contol::GetStickDirect(static_cast<contol::PLAYER_ID>(player), LR_ID::L, XY_ID::X, d_state1);
			g_InputState[player].ls.y = contol::GetStickDirect(static_cast<contol::PLAYER_ID>(player), LR_ID::L, XY_ID::Y, d_state1);
			g_InputState[player].rs.x = contol::GetStickDirect(static_cast<contol::PLAYER_ID>(player), LR_ID::R, XY_ID::X, d_state2, true);
			g_InputState[player].rs.y = contol::GetStickDirect(static_cast<contol::PLAYER_ID>(player), LR_ID::R, XY_ID::Y, d_state2, true);

			// トリガー
			g_InputState[player].lt = contol::GetTriggerDirect(static_cast<contol::PLAYER_ID>(player), LR_ID::L, d_state1);
			g_InputState[player].rt = contol::GetTriggerDirect(static_cast<contol::PLAYER_ID>(player), LR_ID::R, d_state2, true);
		}
	}

	// ALLの更新
	contol::UpdateAllState();
}

void contol::UpdateAllState()
{
	const int num = static_cast<int>(contol::PLAYER_ID::ALL);

	// 1フレーム前の状態を保存
	g_PrevInputState[num] = g_InputState[num];

	// 十字ボタンの入力成分
	VECTOR2 d_pad_point = { 0.0f,0.0f };

	// LRスティックのベクトルの合成値
	VECTOR2 stick_l = { 0.0f,0.0f };
	VECTOR2 stick_r = { 0.0f,0.0f };

	// LRスティックを入力中の人数
	int input_sl_count = 0;
	int input_sr_count = 0;

	// ボタンのデータの初期化
	for (int button = 0; button < static_cast<int>(contol::BUTTON_ID::MAX); button++)
	{
		g_InputState[num].buttons[button] = false;
	}
	// 十字ボタンのデータの初期化
	g_InputState[num].d_pad_value = static_cast<unsigned int>(contol::D_PAD_ID::NONE);
	// スティックのデータの初期化
	g_InputState[num].ls = { 0.0f,0.0f };
	g_InputState[num].rs = { 0.0f,0.0f };
	// トリガーのデータの初期化
	g_InputState[num].lt = 0.0f;
	g_InputState[num].rt = 0.0f;

	//// 各プレイヤーの状態を見て、ALLの状態を更新 ////

	for (int player = 0; player < static_cast<int>(contol::PLAYER_ID::MAX); player++)
	{
		// デバイスが未登録の場合はスキップ
		if (g_DeviceID[static_cast<int>(player)] == contol::DEVICE_ID::MAX)	continue;

		// ボタンは各入力を論理和で代入
		for (int button = 0; button < static_cast<int>(contol::BUTTON_ID::MAX); button++)
		{
			g_InputState[num].buttons[button] |= g_InputState[player].buttons[button];
		}

		// 十字ボタンの集計
		switch (g_InputState[player].d_pad_value)
		{
		case static_cast<unsigned int>(contol::D_PAD_ID::UP):									d_pad_point.y -= 1.0f;	break;
		case static_cast<unsigned int>(contol::D_PAD_ID::RIGHT_UP):		d_pad_point.x += 0.7f;	d_pad_point.y -= 0.7f;	break;
		case static_cast<unsigned int>(contol::D_PAD_ID::RIGHT):		d_pad_point.x += 1.0f;							break;
		case static_cast<unsigned int>(contol::D_PAD_ID::RIGHT_DOWN):	d_pad_point.x += 0.7f;	d_pad_point.y += 0.7f;	break;
		case static_cast<unsigned int>(contol::D_PAD_ID::DOWN):									d_pad_point.y += 1.0f;	break;
		case static_cast<unsigned int>(contol::D_PAD_ID::LEFT_DOWN):	d_pad_point.x -= 0.7f;	d_pad_point.y += 0.7f;	break;
		case static_cast<unsigned int>(contol::D_PAD_ID::LEFT):			d_pad_point.x -= 1.0f;							break;
		case static_cast<unsigned int>(contol::D_PAD_ID::LEFT_UP):		d_pad_point.x -= 0.7f;	d_pad_point.y -= 0.7f;	break;
		}

		// スティック
		if (!(g_InputState[player].ls.x == 0.0f && g_InputState[player].ls.y == 0.0f))
		{
			// スティックベクトルの合成
			stick_l = { stick_l.x + g_InputState[player].ls.x, stick_l.y + g_InputState[player].ls.y };
			// 入力人数の加算
			input_sl_count++;
		}
		if (!(g_InputState[player].rs.x == 0.0f && g_InputState[player].rs.y == 0.0f))
		{
			// スティックベクトルの合成
			stick_r = { stick_r.x + g_InputState[player].rs.x, stick_r.y + g_InputState[player].rs.y };
			// 入力人数の加算
			input_sr_count++;
		}

		// トリガーは絶対値が最も大きい入力を取る
		g_InputState[num].lt = (g_InputState[player].lt > g_InputState[num].lt) ? g_InputState[player].lt : g_InputState[num].lt;
		g_InputState[num].rt = (g_InputState[player].rt > g_InputState[num].rt) ? g_InputState[player].rt : g_InputState[num].rt;
	}

	// ALLの十字ボタンを求める
	if (d_pad_point.x == 0.0f && d_pad_point.y < 0.0f)		g_InputState[num].d_pad_value = static_cast<unsigned int>(contol::D_PAD_ID::UP);
	else if (d_pad_point.x > 0.0f && d_pad_point.y < 0.0f)	g_InputState[num].d_pad_value = static_cast<unsigned int>(contol::D_PAD_ID::RIGHT_UP);
	else if (d_pad_point.x > 0.0f && d_pad_point.y == 0.0f)	g_InputState[num].d_pad_value = static_cast<unsigned int>(contol::D_PAD_ID::RIGHT);
	else if (d_pad_point.x > 0.0f && d_pad_point.y > 0.0f)	g_InputState[num].d_pad_value = static_cast<unsigned int>(contol::D_PAD_ID::RIGHT_DOWN);
	else if (d_pad_point.x == 0.0f && d_pad_point.y > 0.0f)	g_InputState[num].d_pad_value = static_cast<unsigned int>(contol::D_PAD_ID::DOWN);
	else if (d_pad_point.x < 0.0f && d_pad_point.y > 0.0f)	g_InputState[num].d_pad_value = static_cast<unsigned int>(contol::D_PAD_ID::LEFT_DOWN);
	else if (d_pad_point.x < 0.0f && d_pad_point.y == 0.0f)	g_InputState[num].d_pad_value = static_cast<unsigned int>(contol::D_PAD_ID::LEFT);
	else if (d_pad_point.x < 0.0f && d_pad_point.y < 0.0f)	g_InputState[num].d_pad_value = static_cast<unsigned int>(contol::D_PAD_ID::LEFT_UP);
	else													g_InputState[num].d_pad_value = static_cast<unsigned int>(contol::D_PAD_ID::NONE);

	if (input_sl_count <= 0)	input_sl_count = 1;
	if (input_sr_count <= 0)	input_sr_count = 1;

	// スティックの値の平均値を格納する
	g_InputState[num].ls = { stick_l.x / static_cast<float>(input_sl_count), stick_l.y / static_cast<float>(input_sl_count) };
	g_InputState[num].rs = { stick_r.x / static_cast<float>(input_sr_count), stick_r.y / static_cast<float>(input_sr_count) };
}

int contol::GetButtonNum(contol::PLAYER_ID player, contol::BUTTON_ID button, bool second)
{
	// player番目のコントローラーのタイプを取得
	int		pad_type = DxLib::GetJoypadType(static_cast<int>(g_DeviceID[static_cast<int>(player)]) + DX_INPUT_PAD1);
	if (second)
			pad_type = DxLib::GetJoypadType(static_cast<int>(g_SecondDeviceID[static_cast<int>(player)]) + DX_INPUT_PAD1);

	// Joy-Con2本持ちフラグ
	bool	have_two = g_SecondDeviceID[static_cast<int>(player)] != contol::DEVICE_ID::MAX;

	switch (button)
	{
	case contol::BUTTON_ID::START:
		if (pad_type == DX_PADTYPE_OTHER)					return 9;
		else if (pad_type == DX_PADTYPE_XBOX_360)			return 7;
		else if (pad_type == DX_PADTYPE_XBOX_ONE)			return 7;
		else if (pad_type == DX_PADTYPE_DUAL_SHOCK_3)		return 8;
		else if (pad_type == DX_PADTYPE_DUAL_SHOCK_4)		return 9;
		else if (pad_type == DX_PADTYPE_DUAL_SENSE)			return 9;
		else if (pad_type == DX_PADTYPE_SWITCH_JOY_CON_L)
		{
			if (!have_two)									return 13;	// 1本持ち
			else											return 31;	// 2本持ち
		}
		else if (pad_type == DX_PADTYPE_SWITCH_JOY_CON_R)	return 9;	// 1,2本持ち同じ
		else if (pad_type == DX_PADTYPE_SWITCH_PRO_CTRL)	return 9;
		else if (pad_type == DX_PADTYPE_SWITCH_HORI_PAD)	return 9;
		break;
	case contol::BUTTON_ID::BACK:
		if (pad_type == DX_PADTYPE_OTHER)					return 8;
		else if (pad_type == DX_PADTYPE_XBOX_360)			return 6;
		else if (pad_type == DX_PADTYPE_XBOX_ONE)			return 6;
		else if (pad_type == DX_PADTYPE_DUAL_SHOCK_3)		return 9;
		else if (pad_type == DX_PADTYPE_DUAL_SHOCK_4)		return 8;
		else if (pad_type == DX_PADTYPE_DUAL_SENSE)			return 8;
		else if (pad_type == DX_PADTYPE_SWITCH_JOY_CON_L)	return 8;	// 1,2本持ち同じ
		else if (pad_type == DX_PADTYPE_SWITCH_JOY_CON_R)
		{
			if (!have_two)									return 12;	// 1本持ち
			else											return 31;	// 2本持ち
		}
		else if (pad_type == DX_PADTYPE_SWITCH_PRO_CTRL)	return 8;
		else if (pad_type == DX_PADTYPE_SWITCH_HORI_PAD)	return 8;
		break;

	case contol::BUTTON_ID::ACTION_UP:
		if (pad_type == DX_PADTYPE_OTHER)					return 3;
		else if (pad_type == DX_PADTYPE_XBOX_360)			return 3;
		else if (pad_type == DX_PADTYPE_XBOX_ONE)			return 3;
		else if (pad_type == DX_PADTYPE_DUAL_SHOCK_3)		return 0;
		else if (pad_type == DX_PADTYPE_DUAL_SHOCK_4)		return 3;
		else if (pad_type == DX_PADTYPE_DUAL_SENSE)			return 3;
		else if (pad_type == DX_PADTYPE_SWITCH_JOY_CON_L)
		{
			if (!have_two)									return 3;	// 1本持ち
			else											return 31;	// 2本持ち
		}
		else if (pad_type == DX_PADTYPE_SWITCH_JOY_CON_R)
		{
			if (!have_two)									return 3;	// 1本持ち
			else											return 1;	// 2本持ち
		}
		else if (pad_type == DX_PADTYPE_SWITCH_PRO_CTRL)	return 3;
		else if (pad_type == DX_PADTYPE_SWITCH_HORI_PAD)	return 3;
		break;
	case contol::BUTTON_ID::ACTION_DOWN:
		if (pad_type == DX_PADTYPE_OTHER)					return 1;
		else if (pad_type == DX_PADTYPE_XBOX_360)			return 0;
		else if (pad_type == DX_PADTYPE_XBOX_ONE)			return 0;
		else if (pad_type == DX_PADTYPE_DUAL_SHOCK_3)		return 2;
		else if (pad_type == DX_PADTYPE_DUAL_SHOCK_4)		return 1;
		else if (pad_type == DX_PADTYPE_DUAL_SENSE)			return 1;
		else if (pad_type == DX_PADTYPE_SWITCH_JOY_CON_L)
		{
			if (!have_two)									return 0;	// 1本持ち
			else											return 31;	// 2本持ち
		}
		else if (pad_type == DX_PADTYPE_SWITCH_JOY_CON_R)
		{
			if (!have_two)									return 0;	// 1本持ち
			else											return 2;	// 2本持ち
		}
		else if (pad_type == DX_PADTYPE_SWITCH_PRO_CTRL)	return 0;
		else if (pad_type == DX_PADTYPE_SWITCH_HORI_PAD)	return 0;
		break;
	case contol::BUTTON_ID::ACTION_LEFT:
		if (pad_type == DX_PADTYPE_OTHER)					return 0;
		else if (pad_type == DX_PADTYPE_XBOX_360)			return 2;
		else if (pad_type == DX_PADTYPE_XBOX_ONE)			return 2;
		else if (pad_type == DX_PADTYPE_DUAL_SHOCK_3)		return 3;
		else if (pad_type == DX_PADTYPE_DUAL_SHOCK_4)		return 0;
		else if (pad_type == DX_PADTYPE_DUAL_SENSE)			return 0;
		else if (pad_type == DX_PADTYPE_SWITCH_JOY_CON_L)
		{
			if (!have_two)									return 2;	// 1本持ち
			else											return 31;	// 2本持ち
		}
		else if (pad_type == DX_PADTYPE_SWITCH_JOY_CON_R)
		{
			if (!have_two)									return 2;	// 1本持ち
			else											return 3;	// 2本持ち
		}
		else if (pad_type == DX_PADTYPE_SWITCH_PRO_CTRL)	return 2;
		else if (pad_type == DX_PADTYPE_SWITCH_HORI_PAD)	return 2;
		break;
	case contol::BUTTON_ID::ACTION_RIGHT:
		if (pad_type == DX_PADTYPE_OTHER)					return 2;
		else if (pad_type == DX_PADTYPE_XBOX_360)			return 1;
		else if (pad_type == DX_PADTYPE_XBOX_ONE)			return 1;
		else if (pad_type == DX_PADTYPE_DUAL_SHOCK_3)		return 1;
		else if (pad_type == DX_PADTYPE_DUAL_SHOCK_4)		return 2;
		else if (pad_type == DX_PADTYPE_DUAL_SENSE)			return 2;
		else if (pad_type == DX_PADTYPE_SWITCH_JOY_CON_L)
		{
			if (!have_two)									return 1;	// 1本持ち
			else											return 31;	// 2本持ち
		}
		else if (pad_type == DX_PADTYPE_SWITCH_JOY_CON_R)
		{
			if (!have_two)									return 1;	// 1本持ち
			else											return 0;	// 2本持ち
		}
		else if (pad_type == DX_PADTYPE_SWITCH_PRO_CTRL)	return 1;
		else if (pad_type == DX_PADTYPE_SWITCH_HORI_PAD)	return 1;
		break;

	case contol::BUTTON_ID::LB:
		if (pad_type == DX_PADTYPE_OTHER)					return 4;
		else if (pad_type == DX_PADTYPE_XBOX_360)			return 4;
		else if (pad_type == DX_PADTYPE_XBOX_ONE)			return 4;
		else if (pad_type == DX_PADTYPE_DUAL_SHOCK_3)		return 4;
		else if (pad_type == DX_PADTYPE_DUAL_SHOCK_4)		return 4;
		else if (pad_type == DX_PADTYPE_DUAL_SENSE)			return 4;
		else if (pad_type == DX_PADTYPE_SWITCH_JOY_CON_L)
		{
			if (!have_two)									return 4;	// 1本持ち
			else											return 14;	// 2本持ち
		}
		else if (pad_type == DX_PADTYPE_SWITCH_JOY_CON_R)
		{
			if (!have_two)									return 4;	// 1本持ち
			else											return 31;	// 2本持ち
		}
		else if (pad_type == DX_PADTYPE_SWITCH_PRO_CTRL)	return 4;
		else if (pad_type == DX_PADTYPE_SWITCH_HORI_PAD)	return 4;
		break;
	case contol::BUTTON_ID::RB:
		if (pad_type == DX_PADTYPE_OTHER)					return 5;
		else if (pad_type == DX_PADTYPE_XBOX_360)			return 5;
		else if (pad_type == DX_PADTYPE_XBOX_ONE)			return 5;
		else if (pad_type == DX_PADTYPE_DUAL_SHOCK_3)		return 5;
		else if (pad_type == DX_PADTYPE_DUAL_SHOCK_4)		return 5;
		else if (pad_type == DX_PADTYPE_DUAL_SENSE)			return 5;
		else if (pad_type == DX_PADTYPE_SWITCH_JOY_CON_L)
		{
			if (!have_two)									return 5;	// 1本持ち
			else											return 31;	// 2本持ち
		}
		else if (pad_type == DX_PADTYPE_SWITCH_JOY_CON_R)
		{
			if (!have_two)									return 5;	// 1本持ち
			else											return 14;	// 2本持ち
		}
		else if (pad_type == DX_PADTYPE_SWITCH_PRO_CTRL)	return 5;
		else if (pad_type == DX_PADTYPE_SWITCH_HORI_PAD)	return 5;
		break;

	case contol::BUTTON_ID::L_THUMB:
		if (pad_type == DX_PADTYPE_OTHER)					return 10;
		else if (pad_type == DX_PADTYPE_XBOX_360)			return 8;
		else if (pad_type == DX_PADTYPE_XBOX_ONE)			return 8;
		else if (pad_type == DX_PADTYPE_DUAL_SHOCK_3)		return 10;
		else if (pad_type == DX_PADTYPE_DUAL_SHOCK_4)		return 10;
		else if (pad_type == DX_PADTYPE_DUAL_SENSE)			return 10;
		else if (pad_type == DX_PADTYPE_SWITCH_JOY_CON_L)	return 10;	// 1,2本持ち同じ
		else if (pad_type == DX_PADTYPE_SWITCH_JOY_CON_R)
		{
			if (!have_two)									return 11;	// 1本持ち
			else											return 31;	// 2本持ち
		}
		else if (pad_type == DX_PADTYPE_SWITCH_PRO_CTRL)	return 10;
		else if (pad_type == DX_PADTYPE_SWITCH_HORI_PAD)	return 10;
		break;
	case contol::BUTTON_ID::R_THUMB:
		if (pad_type == DX_PADTYPE_OTHER)					return 11;
		else if (pad_type == DX_PADTYPE_XBOX_360)			return 9;
		else if (pad_type == DX_PADTYPE_XBOX_ONE)			return 9;
		else if (pad_type == DX_PADTYPE_DUAL_SHOCK_3)		return 11;
		else if (pad_type == DX_PADTYPE_DUAL_SHOCK_4)		return 11;
		else if (pad_type == DX_PADTYPE_DUAL_SENSE)			return 11;
		else if (pad_type == DX_PADTYPE_SWITCH_JOY_CON_L)	return 31;	// 1,2本持ち同じ
		else if (pad_type == DX_PADTYPE_SWITCH_JOY_CON_R)
		{
			if (!have_two)									return 31;	// 1本持ち
			else											return 11;	// 2本持ち
		}
		else if (pad_type == DX_PADTYPE_SWITCH_PRO_CTRL)	return 11;
		else if (pad_type == DX_PADTYPE_SWITCH_HORI_PAD)	return 11;
		break;
	}

	return 0;
}

float contol::GetStickDirect(contol::PLAYER_ID player, contol::LR_ID lr, contol::XY_ID xy, DxLib::DINPUT_JOYSTATE& state, bool second)
{
	// player番目のコントローラーのタイプを取得
	int pad_type = DxLib::GetJoypadType(static_cast<int>(g_DeviceID[static_cast<int>(player)]) + DX_INPUT_PAD1);
	if (second)
		pad_type = DxLib::GetJoypadType(static_cast<int>(g_SecondDeviceID[static_cast<int>(player)]) + DX_INPUT_PAD1);

	int value = 0;

	switch (pad_type)
	{
	case DX_PADTYPE_OTHER:
		if (lr == contol::LR_ID::L && xy == contol::XY_ID::X)		value = state.X;	// LスティックのX軸
		else if (lr == contol::LR_ID::L && xy == contol::XY_ID::Y)	value = state.Y;	// LスティックのY軸
		else if (lr == contol::LR_ID::R && xy == contol::XY_ID::X)	value = state.Z;	// RスティックのX軸
		else if (lr == contol::LR_ID::R && xy == contol::XY_ID::Y)	value = state.Rz;	// RスティックのY軸
		break;
	case DX_PADTYPE_XBOX_360:
		if (lr == contol::LR_ID::L && xy == contol::XY_ID::X)		value = state.X;	// LスティックのX軸
		else if (lr == contol::LR_ID::L && xy == contol::XY_ID::Y)	value = state.Y;	// LスティックのY軸
		else if (lr == contol::LR_ID::R && xy == contol::XY_ID::X)	value = state.Rx;	// RスティックのX軸
		else if (lr == contol::LR_ID::R && xy == contol::XY_ID::Y)	value = state.Ry;	// RスティックのY軸
		break;
	case DX_PADTYPE_XBOX_ONE:
		if (lr == contol::LR_ID::L && xy == contol::XY_ID::X)		value = state.X;	// LスティックのX軸
		else if (lr == contol::LR_ID::L && xy == contol::XY_ID::Y)	value = state.Y;	// LスティックのY軸
		else if (lr == contol::LR_ID::R && xy == contol::XY_ID::X)	value = state.Rx;	// RスティックのX軸
		else if (lr == contol::LR_ID::R && xy == contol::XY_ID::Y)	value = state.Ry;	// RスティックのY軸
		break;
	case DX_PADTYPE_DUAL_SHOCK_3:
		if (lr == contol::LR_ID::L && xy == contol::XY_ID::X)		value = state.X;	// LスティックのX軸
		else if (lr == contol::LR_ID::L && xy == contol::XY_ID::Y)	value = state.Y;	// LスティックのY軸
		else if (lr == contol::LR_ID::R && xy == contol::XY_ID::X)	value = state.Z;	// RスティックのX軸
		else if (lr == contol::LR_ID::R && xy == contol::XY_ID::Y)	value = state.Rz;	// RスティックのY軸
		break;
	case DX_PADTYPE_DUAL_SHOCK_4:
		if (lr == contol::LR_ID::L && xy == contol::XY_ID::X)		value = state.X;	// LスティックのX軸
		else if (lr == contol::LR_ID::L && xy == contol::XY_ID::Y)	value = state.Y;	// LスティックのT軸
		else if (lr == contol::LR_ID::R && xy == contol::XY_ID::X)	value = state.Z;	// RスティックのX軸
		else if (lr == contol::LR_ID::R && xy == contol::XY_ID::Y)	value = state.Rz;	// RスティックのY軸
		break;
	case DX_PADTYPE_DUAL_SENSE:
		if (lr == contol::LR_ID::L && xy == contol::XY_ID::X)		value = state.X;	// LスティックのX軸
		else if (lr == contol::LR_ID::L && xy == contol::XY_ID::Y)	value = state.Y;	// LスティックのY軸
		else if (lr == contol::LR_ID::R && xy == contol::XY_ID::X)	value = state.Z;	// RスティックのX軸
		else if (lr == contol::LR_ID::R && xy == contol::XY_ID::Y)	value = state.Rz;	// RスティックのY軸
		break;
	case DX_PADTYPE_SWITCH_JOY_CON_L:
		// 1台の時（通常/Joy-Con片手持ち時）
		if (g_SecondDeviceID[static_cast<int>(player)] == contol::DEVICE_ID::MAX)
			value = static_cast<int>(contol::GetJoyconStickValue(false, contol::LR_ID::L, xy, state) * g_max_analog_value);
		// 2台の時（Joy-Con両手持ち時）
		else
			value = static_cast<int>(contol::GetJoyconStickValue(true, contol::LR_ID::L, xy, state) * g_max_analog_value);
		break;
	case DX_PADTYPE_SWITCH_JOY_CON_R:
		// 1台の時（通常/Joy-Con片手持ち時）
		if (g_SecondDeviceID[static_cast<int>(player)] == contol::DEVICE_ID::MAX)
			value = static_cast<int>(contol::GetJoyconStickValue(false, contol::LR_ID::R, xy, state) * g_max_analog_value);
		// 2台の時（Joy-Con両手持ち時）
		else
			value = static_cast<int>(contol::GetJoyconStickValue(true, contol::LR_ID::R, xy, state) * g_max_analog_value);
		break;
	case DX_PADTYPE_SWITCH_PRO_CTRL:
		if (lr == contol::LR_ID::L && xy == contol::XY_ID::X)		value = state.X;	// LスティックのX軸
		else if (lr == contol::LR_ID::L && xy == contol::XY_ID::Y)	value = state.Y;	// LスティックのY軸
		else if (lr == contol::LR_ID::R && xy == contol::XY_ID::X)	value = state.Rx;	// RスティックのX軸
		else if (lr == contol::LR_ID::R && xy == contol::XY_ID::Y)	value = state.Ry;	// RスティックのY軸
		break;
	case DX_PADTYPE_SWITCH_HORI_PAD:
		if (lr == contol::LR_ID::L && xy == contol::XY_ID::X)		value = state.X;	// LスティックのX軸
		else if (lr == contol::LR_ID::L && xy == contol::XY_ID::Y)	value = state.Y;	// LスティックのY軸
		else if (lr == contol::LR_ID::R && xy == contol::XY_ID::X)	value = state.Rx;	// RスティックのX軸
		else if (lr == contol::LR_ID::R && xy == contol::XY_ID::Y)	value = state.Ry;	// RスティックのY軸
		break;
	}

	// 取得した値を最大値(1000)で割る
	return static_cast<float>(value) / static_cast<float>(g_max_analog_value);
}

float contol::GetTriggerDirect(contol::PLAYER_ID player, contol::LR_ID lr, DxLib::DINPUT_JOYSTATE& state, bool second)
{
	// player番目のコントローラーのタイプを取得
	int pad_type = DxLib::GetJoypadType(static_cast<int>(g_DeviceID[static_cast<int>(player)]) + DX_INPUT_PAD1);
	if (second)
		pad_type = DxLib::GetJoypadType(static_cast<int>(g_SecondDeviceID[static_cast<int>(player)]) + DX_INPUT_PAD1);


	int value = 0;

	switch (pad_type)
	{
	case DX_PADTYPE_OTHER:				// ※switch2ホリパッドを参考に作成
		if (lr == contol::LR_ID::L)		value = (state.Buttons[6] != 0) * g_max_analog_value;
		else							value = (state.Buttons[7] != 0) * g_max_analog_value;
		break;
	case DX_PADTYPE_XBOX_360:			// DirectInputだとLRトリガーの値が相殺されるため、XInputで取得（値が0~1000になるよう補正）
		DxLib::XINPUT_STATE state_360;
		// 現在の入力状態を取得
		DxLib::GetJoypadXInputState(static_cast<int>(g_DeviceID[static_cast<int>(player)]) + DX_INPUT_PAD1, &state_360);

		if (lr == contol::LR_ID::L)		value = state_360.LeftTrigger * g_max_analog_value / g_max_x_trigger;
		else							value = state_360.RightTrigger * g_max_analog_value / g_max_x_trigger;
		break;
	case DX_PADTYPE_XBOX_ONE:			// DirectInputだとLRトリガーの値が相殺されるため、XInputで取得（値が0~1000になるよう補正）
		DxLib::XINPUT_STATE state_one;
		// 現在の入力状態を取得
		DxLib::GetJoypadXInputState(static_cast<int>(g_DeviceID[static_cast<int>(player)]) + DX_INPUT_PAD1, &state_one);

		if (lr == contol::LR_ID::L)		value = state_one.LeftTrigger * g_max_analog_value / g_max_x_trigger;
		else							value = state_one.RightTrigger * g_max_analog_value / g_max_x_trigger;
		break;
	case DX_PADTYPE_DUAL_SHOCK_3:		// 値が-1000~1000のため、0~1000になるよう補正
		if (lr == contol::LR_ID::L)		value = (state.Rx + g_max_analog_value) / 2;
		else							value = (state.Ry + g_max_analog_value) / 2;
		break;
	case DX_PADTYPE_DUAL_SHOCK_4:		// 値が-1000~1000のため、0~1000になるよう補正
		if (lr == contol::LR_ID::L)		value = (state.Rx + g_max_analog_value) / 2;
		else							value = (state.Ry + g_max_analog_value) / 2;
		break;
	case DX_PADTYPE_DUAL_SENSE:			// 値が-1000~1000のため、0~1000になるよう補正
		if (lr == contol::LR_ID::L)		value = (state.Rx + g_max_analog_value) / 2;
		else							value = (state.Ry + g_max_analog_value) / 2;
		break;
	case DX_PADTYPE_SWITCH_JOY_CON_L:	// アナログトリガーはないため、ボタンで取得し0or1000で出力
		if (lr == contol::LR_ID::L)		value = (state.Buttons[15] != 0) * g_max_analog_value;	// ZL
		else if (g_SecondDeviceID[static_cast<int>(player)] == contol::DEVICE_ID::MAX)
										value = (state.Buttons[14] != 0) * g_max_analog_value;	// 1台の時は[L]を[ZR]とする
		break;
	case DX_PADTYPE_SWITCH_JOY_CON_R:	// アナログトリガーはないため、ボタンで取得し0or1000で出力
		if (lr == contol::LR_ID::R)		value = (state.Buttons[15] != 0) * g_max_analog_value;	// ZR
		else if (g_SecondDeviceID[static_cast<int>(player)] == contol::DEVICE_ID::MAX)
										value = (state.Buttons[14] != 0) * g_max_analog_value;	// 1台の時は[R]を[ZL]とする
		break;
	case DX_PADTYPE_SWITCH_PRO_CTRL:	// アナログトリガーはないため、ボタンで取得し0or1000で出力
		if (lr == contol::LR_ID::L)		value = (state.Buttons[6] != 0) * g_max_analog_value;	// ZL
		else							value = (state.Buttons[7] != 0) * g_max_analog_value;	// ZR
		break;
	case DX_PADTYPE_SWITCH_HORI_PAD:	// アナログトリガーはないため、ボタンで取得し0or1000で出力
		if (lr == contol::LR_ID::L)		value = (state.Buttons[6] != 0) * g_max_analog_value;	// ZL
		else							value = (state.Buttons[7] != 0) * g_max_analog_value;	// ZR
		break;
	}

	// 取得した値を最大値(1000)で割る
	return static_cast<float>(value) / static_cast<float>(g_max_analog_value);
}

float contol::GetJoyconStickValue(bool have_two, contol::LR_ID lr, contol::XY_ID xy, DxLib::DINPUT_JOYSTATE& state)
{
	float x = 0.0f;
	float y = 0.0f;

	// Joy-Conスティックの8方向入力をベクトルに変換
	switch (state.POV[0])
	{
	case static_cast<unsigned int>(contol::D_PAD_ID::UP):						y = -1.0f;	break;
	case static_cast<unsigned int>(contol::D_PAD_ID::RIGHT_UP):		x = 0.7f;	y = -0.7f;	break;
	case static_cast<unsigned int>(contol::D_PAD_ID::RIGHT):		x = 1.0f;				break;
	case static_cast<unsigned int>(contol::D_PAD_ID::RIGHT_DOWN):	x = 0.7f;	y = 0.7f;	break;
	case static_cast<unsigned int>(contol::D_PAD_ID::DOWN):						y = 1.0f;	break;
	case static_cast<unsigned int>(contol::D_PAD_ID::LEFT_DOWN):	x = -0.7f;	y = 0.7f;	break;
	case static_cast<unsigned int>(contol::D_PAD_ID::LEFT):			x = -1.0f;				break;
	case static_cast<unsigned int>(contol::D_PAD_ID::LEFT_UP):		x = -0.7f;	y = -0.7f;	break;
	}
	
	// 2本(縦)持ちの時
	if (have_two)
	{
		float tmp = 0;
		// Joy-Con L は時計回りに90°回転
		if (lr == contol::LR_ID::L)
		{
			tmp = x;
			x = -y;
			y = tmp;
		}
		// Joy-Con R は反時計回りに90°回転
		else
		{
			tmp = y;
			y = -x;
			x = tmp;
		}
	}

	if (xy == contol::XY_ID::X)	return x;
	else						return y;
}

void contol::EntryDevice(int empty)
{
	for (int device = 0; device < static_cast<int>(contol::DEVICE_ID::MAX) && device < DxLib::GetJoypadNum(); device++)
	{
		// 既に登録済みの場合はスキップ
		bool is_skip = false;
		for (int player = 0; player < g_ActiveCount; player++)
		{
			if (g_DeviceID[player] == static_cast<contol::DEVICE_ID>(device)
				|| g_SecondDeviceID[player] == static_cast<contol::DEVICE_ID>(device))
			{
				is_skip = true;
				break;
			}
		}
		if (is_skip)	continue;

		DINPUT_JOYSTATE state;	// DirectInputの状態変数
		// 現在の入力状態を取得
		DxLib::GetJoypadDirectInputState(device + DX_INPUT_PAD1, &state);

		//// 登録ボタンのチェック ////

		// Joy-ConLが[L]を入力している時は、未登録のJoy-ConRを探し[R]入力されている時はエントリー
		if (DxLib::GetJoypadType(device + DX_INPUT_PAD1) == DX_PADTYPE_SWITCH_JOY_CON_L && state.Buttons[14] != 0)
		{
			for (int device2 = 0; device2 < static_cast<int>(contol::DEVICE_ID::MAX) && device2 < DxLib::GetJoypadNum(); device2++)
			{
				// Joy-ConR以外または既に登録済みの場合はスキップ
				if (DxLib::GetJoypadType(device2 + DX_INPUT_PAD1) != DX_PADTYPE_SWITCH_JOY_CON_R)	continue;
				is_skip = false;
				for (int player = 0; player < g_ActiveCount; player++)
				{
					if (g_DeviceID[player] == static_cast<contol::DEVICE_ID>(device2)
						|| g_SecondDeviceID[player] == static_cast<contol::DEVICE_ID>(device2))
					{
						is_skip = true;
						break;
					}
				}
				if (is_skip)	continue;

				DINPUT_JOYSTATE state2;	// DirectInputの状態変数
				// 現在の入力状態を取得
				DxLib::GetJoypadDirectInputState(device2 + DX_INPUT_PAD1, &state2);

				if (state2.Buttons[14] != 0)
				{
					g_DeviceID[empty] = static_cast<contol::DEVICE_ID>(device);
					g_SecondDeviceID[empty] = static_cast<contol::DEVICE_ID>(device2);
				}
			}
		}
		// 上記以外でLB+RBが押されてる時はエントリー
		else
		{
			// Joy-Con(L or R)の時で横持ちが許可されていない時はスキップ
			if ((DxLib::GetJoypadType(device + DX_INPUT_PAD1) == DX_PADTYPE_SWITCH_JOY_CON_L
				|| DxLib::GetJoypadType(device + DX_INPUT_PAD1) == DX_PADTYPE_SWITCH_JOY_CON_L)
				&& !g_CanHorizontal)	break;

			if (state.Buttons[4] != 0 && state.Buttons[5] != 0)	
				g_DeviceID[empty] = static_cast<contol::DEVICE_ID>(device);
		}
	}

	return;
}

void contol::CheckSecondJoycon()
{
	for (int player = 0; player < g_ActiveCount; player++)
	{
		// 第2コントローラーがJoy-ConR以外の時は、リセット(MAXに)してスキップ
		if (DxLib::GetJoypadType(static_cast<int>(g_SecondDeviceID[player]) + DX_INPUT_PAD1) != DX_PADTYPE_SWITCH_JOY_CON_R)
		{
			g_SecondDeviceID[player] = contol::DEVICE_ID::MAX;
			continue;
		}

		//// 第2コントローラーがJoy-ConRの時 ////

		// 第1コントローラーがJoy-ConL以外の時は、リセット(MAXに)
		if (DxLib::GetJoypadType(static_cast<int>(g_DeviceID[player]) + DX_INPUT_PAD1) != DX_PADTYPE_SWITCH_JOY_CON_L)
		{
			g_DeviceID[player] = contol::DEVICE_ID::MAX;
		}
	}
}

void contol::CheckDeviceAndID()
{
	// 8人(最大人数)分コントローラーの入力情報取得
	for (int player = 0; player < static_cast<int>(contol::PLAYER_ID::MAX); player++)
	{
		// コントローラーが接続されている番号はスキップ
		if (DxLib::GetJoypadType(static_cast<int>(g_DeviceID[player]) + DX_INPUT_PAD1) != -1)	continue;

		// 空の場合はIDの初期化(contol::PLAYER_ID::MAX)
		g_DeviceID[player] = contol::DEVICE_ID::MAX;
		g_SecondDeviceID[player] = contol::DEVICE_ID::MAX;
	}

	// Joy-Con2本持ちの整合性チェック
	contol::CheckSecondJoycon();
}
