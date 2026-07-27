#include "entry.h"
#include "contol.h"
#include "../../../sound_manager/sound_manager.h"
#include "../../../ui_manager/ui_manager.h"

const int	CEntry::m__back_mode_scene_time = 60;

CEntry::CEntry()
	: m_BackModeSceneTimer(0)
	, m_BackBarUi(nullptr)
{
}

void CEntry::Initialize()
{
	// BGMの再生
	//SOUNDS.Play(SOUND_ID::ENTRY_BGM, true);

	// 接続コントローラーを全て解除
	contol::RemovePlayerDevice(contol::PLAYER_ID::ALL);

	// ハンデ初期化
	//PLAYS.SetHandicap(contol::PLAYER_ID::ALL, -3);

	// 戻るUIの生成
	m_BackBarUi = UIS.CreateBar(UI_ID::BACK_BAR_UI, { 10.0f,10.0f });
}

void CEntry::Update()
{
	namespace keyboard = vivid::keyboard;

	// ゲームメインへ（下ボタンがおされ、かつ必要人数エントリーしている時）
	if (contol::InputStart(contol::PLAYER_ID::ALL, contol::BUTTON_ID::ACTION_DOWN)
		|| keyboard::Button(keyboard::KEY_ID::SPACE) && keyboard::Button(keyboard::KEY_ID::RETURN))
	{
		SCENES.SetNextScene(SCENE_ID::GAMEMAIN);

		// SEの再生
		//SOUNDS.Play(SOUND_ID::PERFECT_LINK);

		return;
	}

	// デバイスエントリー
	contol::SetPlayerDevice();

	// 全プレイヤー分の入力ループ
	for (int player = 0; player < (int)contol::PLAYER_ID::MAX; player++)
	{
		// エントリーキャンセル入力
		if (contol::InputEnd((contol::PLAYER_ID)player, contol::BUTTON_ID::ACTION_RIGHT))
		{
			contol::RemovePlayerDevice((contol::PLAYER_ID)player);

			// SEの再生
			//SOUNDS.Play(SOUND_ID::CANCEL);
		}

		// ハンデ設定
		//if (PLAYS.GetGameMode() != CPlayManager::GAME_MODE::EVERY_PAIR)
		//{
		//	// レベル+1
		//	if (contol::InputStickStart((contol::PLAYER_ID)player, contol::LR_ID::L, contol::STICK_DIR::UP)
		//		|| contol::InputDPadStart((contol::PLAYER_ID)player, contol::D_PAD_ID::UP))
		//	{
		//		PLAYS.SetHandicap((contol::PLAYER_ID)player, PLAYS.GetHandicap((contol::PLAYER_ID)player) + 1);
		//	}
		//	// レベル-1
		//	if (contol::InputStickStart((contol::PLAYER_ID)player, contol::LR_ID::L, contol::STICK_DIR::DOWN)
		//		|| contol::InputDPadStart((contol::PLAYER_ID)player, contol::D_PAD_ID::DOWN))
		//	{
		//		PLAYS.SetHandicap((contol::PLAYER_ID)player, PLAYS.GetHandicap((contol::PLAYER_ID)player) - 1);
		//	}
		//}
	}

	// 戻るボタン入力
	// 入力した瞬間
	if (contol::InputStart(contol::PLAYER_ID::ALL, contol::BUTTON_ID::ACTION_RIGHT)
		|| keyboard::Trigger(keyboard::KEY_ID::BACK))
	{
		m_BackModeSceneTimer = 0;
	}
	// ボタン入力中
	if (contol::Input(contol::PLAYER_ID::ALL, contol::BUTTON_ID::ACTION_RIGHT)
		|| keyboard::Button(keyboard::KEY_ID::BACK))
	{
		// 特殊入力で少し早く戻る
		if (contol::InputDPad(contol::PLAYER_ID::ALL, contol::D_PAD_ID::LEFT)
			|| contol::InputStick(contol::PLAYER_ID::ALL, contol::LR_ID::L, contol::STICK_DIR::LEFT))
		{
			m_BackModeSceneTimer += 2;
		}

		// 一定時間経過したらモード選択シーンに戻る
		if (m_BackModeSceneTimer++ > m__back_mode_scene_time)
		{
			m_BackModeSceneTimer = m__back_mode_scene_time;
			SCENES.SetNextScene(SCENE_ID::MODE);

			// SEの再生
			//SOUNDS.Play(SOUND_ID::CANCEL);
		}
	}
	else
	{
		m_BackModeSceneTimer = 0;
	}

	// 戻るUIの更新
	m_BackBarUi->SetBarRate((float)m_BackModeSceneTimer / (float)m__back_mode_scene_time);

#ifdef VIVID_DEBUG
#endif // DEBUG
}

void CEntry::Draw()
{

	vivid::DrawText(40, "[ L + R ] ：エントリー", { 100.0f,180.0f });
	vivid::DrawText(40, "下ボタン：ゲームスタート", { 100.0f,230.0f });
	vivid::DrawText(40, "右ボタン：エントリーキャンセル", { 100.0f,280.0f });

	// デバイス仮描画
	for (int i = 0; i < 4; i++)
	{
		contol::PAD_ID  type = contol::GetUseDevice((contol::PLAYER_ID)i);
		std::string     str = std::to_string(i + 1) + "P: ";
		switch (type)
		{
		case contol::PAD_ID::XBOX_360:      str += "Xbox 360";   break;
		case contol::PAD_ID::XBOX_ONE:      str += "Xbox One";   break;
		case contol::PAD_ID::DUAL_SHOCK_3:  str += "DUALSHOCK3"; break;
		case contol::PAD_ID::DUAL_SHOCK_4:  str += "DUALSHOCK4"; break;
		case contol::PAD_ID::DUAL_SENSE:    str += "DualSense";  break;
		case contol::PAD_ID::NS_JOY_CON_L:  str += "Joy-Con L";  break;
		case contol::PAD_ID::NS_JOY_CON_R:  str += "Joy-Con R";  break;
		case contol::PAD_ID::NS_JOY_CON:    str += "Joy-Con";    break;
		case contol::PAD_ID::NS_PRO_CTRL:   str += "ProCtrl";    break;
		case contol::PAD_ID::NS_HORI_PAD:   str += "HoriPad";    break;
		case contol::PAD_ID::OTHER:         str += "other";      break;
		case contol::PAD_ID::NONE:          str += "-------";    break;
		}

		//str += ",　ハンデ: " + std::to_string(PLAYS.GetHandicap((contol::PLAYER_ID)i));

		vivid::DrawText(80, str, { 750.0f,180.0f + (float)(i * 100) });
	}

	vivid::DrawText(50, "空白状態のゲームスタートでCOM乱入", { 0.0f,vivid::WINDOW_HEIGHT - 60.0f });

#ifdef VIVID_DEBUG
	vivid::DrawText(30, "Entry", vivid::Vector2::ZERO);
#endif // DEBUG

}

void CEntry::Finalize()
{
	// BGMの停止
	//SOUNDS.Stop(SOUND_ID::ENTRY_BGM);
}
