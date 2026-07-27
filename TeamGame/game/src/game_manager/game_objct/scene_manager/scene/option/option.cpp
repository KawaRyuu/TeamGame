#include "option.h"
#include "contol.h"
#include "../../../sound_manager/sound_manager.h"
#include "../../../ui_manager/ui_manager.h"
#include "../../../ui_manager/ui/bar_ui/volume_slider_ui/volume_slider_ui.h"

const int	COption::m_input_time = 15;

COption::COption()
	: m_VolumeChange(SOUND_TYPE::BGM)
	, m_Volume{ 0,0 }
	, m_InputTimer(0)
	, m_VolumeBar{ nullptr,nullptr }
{
}

void COption::Initialize()
{
	m_Volume[(int)SOUND_TYPE::BGM] = SOUNDS.GetSoundsVolume(SOUND_TYPE::BGM);
	m_Volume[(int)SOUND_TYPE::SE] = SOUNDS.GetSoundsVolume(SOUND_TYPE::SE);

	// UIの生成
	m_VolumeBar[0] = UIS.CreateBar(UI_ID::VOLUME_SLIDER_UI,
		{ (float)vivid::WINDOW_WIDTH / 2.0f - 540.0f,(float)vivid::WINDOW_HEIGHT / 4.0f });
	m_VolumeBar[1] = UIS.CreateBar(UI_ID::VOLUME_SLIDER_UI,
		{ (float)vivid::WINDOW_WIDTH / 2.0f - 540.0f,(float)vivid::WINDOW_HEIGHT / 4.0f * 2.0f });

	// UI情報設定
	for (int i = 0; i < (int)SOUND_TYPE::MAX; i++)
	{
		// スライダーの割合値の初期化
		m_VolumeBar[i]->SetBarRate((float)m_Volume[i] / 10);

		// サウンドの種類の設定
		if (auto bar = dynamic_cast<CVolumeSliderUi*>(m_VolumeBar[i]))
		{
			bar->SetSoundType((SOUND_TYPE)i);
			bar->SetSliderColor(0xff80c0ff);
		}
	}
	if (auto bar = dynamic_cast<CVolumeSliderUi*>(m_VolumeBar[0]))	bar->SetSliderActive();

	// BGMの再生
	//SOUNDS.Play(SOUND_ID::OPTION_BGM, true);
}

void COption::Update()
{
	namespace key = vivid::keyboard;

	// 右ボタンでモード選択シーンに戻る
	if (contol::InputEnd(contol::PLAYER_ID::ALL, contol::BUTTON_ID::ACTION_RIGHT)
		|| key::Trigger(key::KEY_ID::RETURN))
	{
		SCENES.SetNextScene(SCENE_ID::MODE);

		// SEの再生
		//SOUNDS.Play(SOUND_ID::CANCEL);
	}

	if (contol::InputStickStart(contol::PLAYER_ID::ALL, contol::LR_ID::L, contol::STICK_DIR::UP)
		|| contol::InputDPadStart(contol::PLAYER_ID::ALL, contol::D_PAD_ID::UP))
	{
		m_VolumeChange = SOUND_TYPE::BGM;

		if (auto bar = dynamic_cast<CVolumeSliderUi*>(m_VolumeBar[(int)SOUND_TYPE::BGM]))	bar->SetSliderActive();
		if (auto bar = dynamic_cast<CVolumeSliderUi*>(m_VolumeBar[(int)SOUND_TYPE::SE]))	bar->SetSliderActive(false);

		// SEの再生
		//SOUNDS.Play(SOUND_ID::MOVE_CURSOR);
	}
	if (contol::InputStickStart(contol::PLAYER_ID::ALL, contol::LR_ID::L, contol::STICK_DIR::DOWN)
		|| contol::InputDPadStart(contol::PLAYER_ID::ALL, contol::D_PAD_ID::DOWN))
	{
		m_VolumeChange = SOUND_TYPE::SE;

		if (auto bar = dynamic_cast<CVolumeSliderUi*>(m_VolumeBar[(int)SOUND_TYPE::BGM]))	bar->SetSliderActive(false);
		if (auto bar = dynamic_cast<CVolumeSliderUi*>(m_VolumeBar[(int)SOUND_TYPE::SE]))	bar->SetSliderActive();

		// SEの再生
		//SOUNDS.Play(SOUND_ID::MOVE_CURSOR);
	}

	// 音量の変更
	this->ChangeVolume();


#ifdef VIVID_DEBUG
#endif // DEBUG
}

void COption::Draw()
{

#ifdef VIVID_DEBUG
	vivid::DrawText(30, "Option", vivid::Vector2::ZERO);
#endif // DEBUG

}

void COption::Finalize()
{
	// BGMの停止
	//SOUNDS.Stop(SOUND_ID::OPTION_BGM);
}

void COption::ChangeVolume()
{
	// ボリュームダウン
	if ((contol::InputStick(contol::PLAYER_ID::ALL, contol::LR_ID::L, contol::STICK_DIR::LEFT)
		|| contol::InputDPad(contol::PLAYER_ID::ALL, contol::D_PAD_ID::LEFT))
		&& m_InputTimer++ % m_input_time == 0)
	{
		if (m_Volume[(int)m_VolumeChange] > 0)
		{
			// 種類ごとに音量を変更する
			m_Volume[(int)m_VolumeChange]--;
			SOUNDS.ChangeVolume(m_VolumeChange, m_Volume[(int)m_VolumeChange]);

			// UIに反映させる
			m_VolumeBar[(int)m_VolumeChange]->SetBarRate((float)m_Volume[(int)m_VolumeChange] / 10);

			// SEの再生
			//SOUNDS.Play(SOUND_ID::MOVE_CURSOR);
		}
		else
		{
			// SEの再生
			//SOUNDS.Play(SOUND_ID::NO);
		}
	}
	else if (contol::InputStickEnd(contol::PLAYER_ID::ALL, contol::LR_ID::L, contol::STICK_DIR::LEFT)
		|| contol::InputDPadEnd(contol::PLAYER_ID::ALL, contol::D_PAD_ID::LEFT))
	{
		m_InputTimer = 0;
	}

	// ボリュームアップ
	if ((contol::InputStick(contol::PLAYER_ID::ALL, contol::LR_ID::L, contol::STICK_DIR::RIGHT)
		|| contol::InputDPad(contol::PLAYER_ID::ALL, contol::D_PAD_ID::RIGHT))
		&& m_InputTimer++ % m_input_time == 0)
	{
		if (m_Volume[(int)m_VolumeChange] < 10)
		{
			// 種類ごとに音量を変更する
			m_Volume[(int)m_VolumeChange]++;
			SOUNDS.ChangeVolume(m_VolumeChange, m_Volume[(int)m_VolumeChange]);

			// UIに反映させる
			m_VolumeBar[(int)m_VolumeChange]->SetBarRate((float)m_Volume[(int)m_VolumeChange] / 10);

			// SEの再生
			//SOUNDS.Play(SOUND_ID::MOVE_CURSOR);
		}
		else
		{
			// SEの再生
			//SOUNDS.Play(SOUND_ID::NO);
		}
	}
	else if (contol::InputStickEnd(contol::PLAYER_ID::ALL, contol::LR_ID::L, contol::STICK_DIR::RIGHT)
		|| contol::InputDPadEnd(contol::PLAYER_ID::ALL, contol::D_PAD_ID::RIGHT))
	{
		m_InputTimer = 0;
	}
}
